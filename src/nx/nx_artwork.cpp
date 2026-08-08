#include "nx_artwork.h"
#include "nx_paths.h"

#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#include <curl/curl.h>
#include <switch.h>

#include "stb_image.h"

namespace
{
    static const size_t kMaxArtworkBytes = 8 * 1024 * 1024;

    static const char *kSourceRoot =
        "https://raw.githubusercontent.com/libretro-thumbnails/MAME/master";

    /* switch-curl uses libnx's TLS backend, which deliberately skips the system
       trust store, so CURLOPT_CAINFO is required for peer verification to work
       at all. Keeping the bundle in romfs means downloads do not depend on a
       writable certificate path on the SD card. */
    static const char *kCaBundle = "romfs:/certs/isrg-root-x1.pem";

    /* Resolved on first use - see nx_paths.h. */
    static char kArtDirBuf[288];
    static const char *artDir()
    {
        if (!kArtDirBuf[0]) nx_path(kArtDirBuf, sizeof(kArtDirBuf), "artwork");
        return kArtDirBuf;
    }

    static bool gNetworkReady = false;

    struct DownloadFile
    {
        FILE  *File;
        size_t Bytes;
        bool   TooLarge;
    };

    static bool fileExists(const std::string &Path)
    {
        struct stat info;
        return stat(Path.c_str(), &info) == 0 && S_ISREG(info.st_mode) && info.st_size > 0;
    }

    static void ensureFolders()
    {
        char dir[288];
        mkdir(nx_base_dir(), 0777);
        mkdir(nx_path(dir, sizeof(dir), "artwork"), 0777);
    }

    /* Anything that would make an illegal or surprising filename becomes '_'.
       Driver short names are already tame, but this is the boundary where a
       remote-influenced string could reach the filesystem, so it is enforced
       rather than assumed. */
    static std::string safeStem(const std::string &Value)
    {
        std::string result = Value;

        for (size_t i = 0; i < result.size(); ++i)
        {
            unsigned char c = (unsigned char)result[i];
            if (c < 32 || c == '/' || c == '\\' || c == ':' || c == '*' ||
                c == '?' || c == '"' || c == '<' || c == '>' || c == '|' || c == '.')
                result[i] = '_';
        }
        return result;
    }

    /* libretro-thumbnails names files after the full title with a fixed set of
       characters substituted. */
    static std::string thumbnailName(const std::string &Value)
    {
        std::string result = Value;

        for (size_t i = 0; i < result.size(); ++i)
        {
            if (result[i] == '&' || result[i] == '*' || result[i] == '/' ||
                result[i] == ':' || result[i] == '`' || result[i] == '<' ||
                result[i] == '>' || result[i] == '?' || result[i] == '\\' ||
                result[i] == '|')
                result[i] = '_';
        }
        return result;
    }

    static size_t writeDownload(void *Data, size_t Size, size_t Count, void *User)
    {
        DownloadFile *target = (DownloadFile *)User;
        size_t bytes = Size * Count;
        size_t written;

        if (!target || !target->File || target->Bytes + bytes > kMaxArtworkBytes)
        {
            if (target) target->TooLarge = true;
            return 0;
        }
        written = fwrite(Data, 1, bytes, target->File);
        target->Bytes += written;
        return written;
    }

    /* Never hand an unverified download to the decoder: check the PNG magic and
       make stb agree on sane dimensions first. */
    static bool validPng(const std::string &Path)
    {
        static const unsigned char pngSignature[8] =
            { 0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a };
        unsigned char signature[8];
        FILE *file = fopen(Path.c_str(), "rb");
        size_t read;
        int width = 0, height = 0, components = 0;

        if (!file) return false;
        read = fread(signature, 1, sizeof(signature), file);
        fclose(file);

        if (read != sizeof(signature) ||
            memcmp(signature, pngSignature, sizeof(signature)) != 0)
            return false;

        return stbi_info(Path.c_str(), &width, &height, &components) != 0 &&
               width > 0 && height > 0 && width <= 4096 && height <= 4096;
    }

    static Artwork::Result fetch(const std::string &Url, const std::string &Destination,
                                 std::string &Error)
    {
        const std::string temporary = Destination + ".part";
        FILE *file = fopen(temporary.c_str(), "wb");
        DownloadFile target;
        CURL *curl;
        CURLcode code;
        long status = 0;

        if (!file)
        {
            Error = "Cannot create artwork cache file.";
            return Artwork::RESULT_STORAGE_ERROR;
        }

        target.File = file;
        target.Bytes = 0;
        target.TooLarge = false;

        curl = curl_easy_init();
        if (!curl)
        {
            fclose(file);
            remove(temporary.c_str());
            Error = "Cannot initialise HTTPS client.";
            return Artwork::RESULT_NETWORK_ERROR;
        }

        curl_easy_setopt(curl, CURLOPT_URL, Url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 4L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "mame-nx/1.0");
        curl_easy_setopt(curl, CURLOPT_CAINFO, kCaBundle);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDownload);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &target);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

        code = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
        curl_easy_cleanup(curl);
        fclose(file);

        if (code != CURLE_OK || status != 200)
        {
            remove(temporary.c_str());

            if (status == 404) return Artwork::RESULT_NOT_FOUND;

            if (target.TooLarge)
                Error = "Artwork exceeds the 8 MB limit.";
            else if (code == CURLE_PEER_FAILED_VERIFICATION ||
                     code == CURLE_SSL_CACERT_BADFILE)
                Error = "Artwork HTTPS certificate check failed.";
            else if (code != CURLE_OK)
                Error = curl_easy_strerror(code);
            else
            {
                char message[80];
                snprintf(message, sizeof(message), "Server returned HTTP %ld.", status);
                Error = message;
            }
            return Artwork::RESULT_NETWORK_ERROR;
        }

        if (!validPng(temporary))
        {
            remove(temporary.c_str());
            Error = "Downloaded file is not a usable PNG.";
            return Artwork::RESULT_INVALID_IMAGE;
        }

        /* Only now does the real filename appear, so a half-written or bad file
           is never something the tile loader can pick up. */
        remove(Destination.c_str());
        if (rename(temporary.c_str(), Destination.c_str()) != 0)
        {
            remove(temporary.c_str());
            Error = "Cannot finalise artwork cache file.";
            return Artwork::RESULT_STORAGE_ERROR;
        }
        return Artwork::RESULT_READY;
    }
}

bool Artwork::Init(std::string &Error)
{
    if (gNetworkReady) return true;

    ensureFolders();

    if (R_FAILED(socketInitializeDefault()))
    {
        Error = "Network service unavailable.";
        return false;
    }
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
    {
        socketExit();
        Error = "HTTPS service unavailable.";
        return false;
    }

    gNetworkReady = true;
    return true;
}

void Artwork::Exit()
{
    if (!gNetworkReady) return;
    curl_global_cleanup();
    socketExit();
    gNetworkReady = false;
}

bool Artwork::Ready() { return gNetworkReady; }

std::string Artwork::LocalPath(const std::string &ShortName)
{
    return std::string(artDir()) + "/" + safeStem(ShortName) + ".png";
}

std::string Artwork::ExistingPath(const std::string &ShortName, const std::string &ParentShortName)
{
    std::string path = LocalPath(ShortName);

    if (fileExists(path)) return path;

    /* Clones almost never have their own art; the parent's is correct. */
    if (!ParentShortName.empty())
    {
        path = LocalPath(ParentShortName);
        if (fileExists(path)) return path;
    }
    return "";
}

Artwork::Result Artwork::Download(const std::string &ShortName, const std::string &Title,
                                  const std::string &ParentShortName, std::string &Error)
{
    static const char *collections[] = { "Named_Boxarts", "Named_Titles", "Named_Snaps" };
    std::string destination;
    std::string name;
    CURL *escapeHandle;
    char *escaped;
    Result result = RESULT_NOT_FOUND;

    if (!ExistingPath(ShortName, ParentShortName).empty()) return RESULT_READY;

    if (!gNetworkReady)
    {
        Error = "Connect the Switch to the internet, then try again.";
        return RESULT_NETWORK_ERROR;
    }

    ensureFolders();

    name = thumbnailName(Title.empty() ? ShortName : Title) + ".png";

    escapeHandle = curl_easy_init();
    if (!escapeHandle)
    {
        Error = "Cannot initialise the artwork URL.";
        return RESULT_NETWORK_ERROR;
    }
    escaped = curl_easy_escape(escapeHandle, name.c_str(), (int)name.size());
    if (!escaped)
    {
        curl_easy_cleanup(escapeHandle);
        Error = "Cannot encode the artwork URL.";
        return RESULT_NETWORK_ERROR;
    }

    destination = LocalPath(ShortName);

    for (size_t i = 0; i < sizeof(collections) / sizeof(collections[0]); ++i)
    {
        std::string url = std::string(kSourceRoot) + "/" + collections[i] + "/" + escaped;

        result = fetch(url, destination, Error);

        /* Only a plain 404 is worth trying the next collection for - a network
           or storage failure will fail the same way three times. */
        if (result != RESULT_NOT_FOUND) break;
    }

    curl_free(escaped);
    curl_easy_cleanup(escapeHandle);

    if (result == RESULT_NOT_FOUND)
        Error = "No box art, title screen or snapshot was found for this game.";

    return result;
}

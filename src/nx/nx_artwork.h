#pragma once

#include <string>

/*
 * Cover artwork for the launcher.
 *
 * Files live at sdmc:/switch/mame-nx/artwork/<romset>.png, which is where the
 * library's tile loader already looks, so a hand-copied image and a downloaded
 * one are indistinguishable.
 *
 * Downloading talks to the network, so Init() is only called when the user
 * turns artwork downloading on, and Download() is only ever reached from an
 * explicit action - never from opening the library.
 */
namespace Artwork
{
    enum Result
    {
        RESULT_READY = 0,
        RESULT_NOT_FOUND,
        RESULT_NETWORK_ERROR,
        RESULT_STORAGE_ERROR,
        RESULT_INVALID_IMAGE
    };

    /* Brings up sockets and HTTPS. Safe to call twice. */
    bool Init(std::string &Error);
    void Exit();
    bool Ready();

    /* Where the tile loader expects this game's cover. */
    std::string LocalPath(const std::string &ShortName);

    /* Non-empty when a usable file is already cached, checking the parent set
       for clones. */
    std::string ExistingPath(const std::string &ShortName, const std::string &ParentShortName);

    /* Fetches box art, falling back to title screen then in-game snapshot.
       Returns immediately if the file is already cached. */
    Result Download(const std::string &ShortName, const std::string &Title,
                    const std::string &ParentShortName, std::string &Error);
}

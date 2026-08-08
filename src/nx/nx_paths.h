#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Per-build data directory.

   Everything this app writes - settings, the library index, the artwork cache,
   save states - lives under the folder the .nro was launched from, so a build
   is a folder rather than a file. Two copies in two folders are two completely
   separate installs that happen to share a name, and testing one against the
   other no longer means swapping .nro files in place.

   Call nx_paths_init() once, first thing in main(), with argv[0]. Falls back to
   the historical sdmc:/switch/mame-nx when there is no argv - a forwarder or a
   title takeover - so an install launched that way keeps working. */

void        nx_paths_init(const char *argv0);

/* The resolved base, with no trailing slash. */
const char *nx_base_dir(void);

/* "<base>/<leaf>" into the caller's buffer; leaf may be NULL for the base
   itself. Returns `out`, so it composes inside a call. */
const char *nx_path(char *out, size_t len, const char *leaf);

/* Same, and creates the directory. */
const char *nx_path_mkdir(char *out, size_t len, const char *leaf);

#ifdef __cplusplus
}
#endif

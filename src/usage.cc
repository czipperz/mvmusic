/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "usage.hh"

#include <cstdio>

using namespace std;
void usage(char* progname) {
    std::printf("Usage: %s [options] [files] [directories]\n", progname);
    std::puts("Options, files, and directories can be in any order.");
    std::puts("");
    std::puts("Options:");
    std::puts("  `-h` `--help` - print out usage and exit.");
    std::puts("  `-m` `--mpc`  - after everything else, run `mpc clear` and");
    std::puts("                  `mpc load <playlist>`, reloading the MPD playlist");
    std::puts("                  with the one provided.");
    std::puts("  `-n` `--nono` - print out what will be done and don't do it");
    std::puts("  `-p <playlist>` `--playlist <playlist>` - set the playlist to insert");
    std::puts("      songs into.  Warns the user if the file provided doesn't have a");
    std::puts("      `.m3u` extension.");
    std::puts("");
    std::puts("Arguments:");
    std::puts("  With no arguments, `mvmusic` will scan the downloads directory for");
    std::puts("      `.mp3` files.");
    std::puts("  With arguments, `mvmusic` will only work on the directories and files");
    std::puts("provided.  It will scan the directories given for all `.mp3` files.");
}

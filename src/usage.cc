#include "usage.hh"

#include <cstdio>

using namespace std;
void usage(char* progname) {
    printf("Usage: %s [options] [files] [directories]\n", progname);
    puts("Options, files, and directories can be in any order.");
    puts("");
    puts("Options:");
    puts("  `-h` `--help` - print out usage and exit.");
    puts("  `-m` `--mpc`  - after everything else, run `mpc clear` and\n"
         "                  `mpc load <playlist>`, reloading the MPD playlist\n"
         "                  with the one provided.");
    puts("  `-n` `--nono` - print out what will be done and don't do it");
    puts("  `-p <playlist>` `--playlist <playlist>` - set the playlist to insert\n"
         "      songs into.  Warns the user if the file provided doesn't have a\n"
         "      `.m3u` extension.");
    puts("");
    puts("Arguments:");
    puts("  With no arguments, `mvmusic` will scan the downloads directory for\n"
         "      `.mp3` files.");
    puts("  With arguments, `mvmusic` will only work on the directories and files\n"
         "provided.  It will scan the directories given for all `.mp3` files.");
}

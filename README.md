# mvmusic

This program does the following things:

1. Scan the downloads directory for `.mp3` files.
2. Reformats the names to clean them up.
3. Change how artist features are declared.
4. Moves these files to their new name in the music directory.
5. Split each file into an artist, title pair.  Such that the file
   name is `artist - title`.
6. If the artist and title found in 5. don't match those written in
   the file metadata, set them appropriately.
7. Slurp the playlist file into memory.
8. Sort the playlist file.
9. If the file name isn't a line in the playlist file, add it in the
   correct alphabetical position.
10. Save the playlist file.

## Installation
### Dependencies
* `taglib` - change song metadata
* `boost` - specifically Boost.Filesystem and Boost.System is needed.
* `pthread` (preinstalled on most linux systems)
* `g++` - the GNU C++ compiler.  If you wish to use a different
  compiler, for example clang, invoke make with `CXX=clang++`.

#### Optional Dependencies
* `mpc` - terminal based MPD client.  If this exists, `mvmusic` will
  reload the MPD playlist as the one provided.

### Shell
* Clone the repository.
* `cd` into the clone.
* `./configure`
* `make`
* `make install`

## Usage
`./mvmusic [options] [files] [directories]`

Options, files, and directories can be in any order.

### Options
* `-h` `--help` - print out usage and exit.
* `-m` `--mpc` - after everything else, run `mpc clear` and
  `mpc load <playlist>`, reloading the MPD playlist with the one provided.
* `-n` `--nono` - print out what will be done and don't do it.
* `-p <playlist>` `--playlist <playlist>` - set the playlist to insert
  songs into.  Warns the user if the file provided doesn't have a
  `.m3u` extension.

### Arguments
With no arguments, `mvmusic` will scan the downloads directory for
`.mp3` files.

With arguments, `mvmusic` will only work on the directories and files
provided.  It will scan the directories given for all `.mp3` files.

#include <algorithm>

#include "env.hh"
#include "parse_args.hh"
#include "consume_path.hh"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv) {
    argc--;
    argv++;

    vector<path> files;
    string playlist;
    parse_args(argc, argv, files, playlist);

    Consumer consumer;
    consumer.read_playlist_file(playlist);
    for (const auto& file : files) {
        consumer.consume(file);
    }
}

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>

#include "env.hh"
#include "parse_args.hh"
#include "consumer.hh"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv) {
    argc--;
    argv++;

    vector<path> files;
    string playlist;
    parse_args(argc, argv, files, playlist);

    Consumer consumer(std::move(playlist));
    for (const auto& file : files) {
        consumer.consume(file);
    }
}

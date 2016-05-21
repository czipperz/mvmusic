/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_PARSE_ARGS_H
#define HEADER_GUARD_PARSE_ARGS_H

#include <vector>
#include <boost/filesystem.hpp>

/**
 * @brief Parses each argument, storing the results in global
 * variables and output arguments.
 *
 * @param [in] argc number of arguments to main() without including
 * the function name.
 * @param [in] argv pointer to the first argument, where @c argv[-1]
 * is the name of the program.
 * @param [out] files all the music files to operate on
 * @param [out] playlist the path to the playlist file to add new
 * songs.
 *
 * @see globals.hh
 * @see NONO
 * @see USE_MPC
 * @see usage.hh
 * @see usage()
 */
void parse_args(int argc, char** argv,
                std::vector<boost::filesystem::path>& files,
                std::string& playlist);

#endif

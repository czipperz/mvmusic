#ifndef HEADER_GUARD_PARSE_ARGS_H
#define HEADER_GUARD_PARSE_ARGS_H

#include <vector>
#include <boost/filesystem.hpp>

void parse_args(int argc, char** argv,
                std::vector<boost::filesystem::path>& files,
                std::string& playlist);

#endif

#ifndef HEADER_GUARD_ENV_H
#define HEADER_GUARD_ENV_H

/**
 * @file env.hh
 * @brief Defines @c get_home_dir()
 */

#include <boost/filesystem.hpp>

/**
 * @brief Returns the path of the home directory on this platform.
 */
boost::filesystem::path get_home_dir();

#endif

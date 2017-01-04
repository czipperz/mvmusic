/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_CONSUME_PATH_H
#define HEADER_GUARD_CONSUME_PATH_H

#include <boost/filesystem.hpp>
#include <future>
#include <vector>
#include <set>

/**
 * @brief Wraps the file consumer interface in a class so can save to
 * a playlist later.
 */
class Consumer {
    std::string _playlist_file_;
    std::set<std::string> _playlist_lines_;
    bool _write_to_file_ = false;

    void _thread_consume_();
    void _apply_tags_(const boost::filesystem::path& mp3_file,
                      const std::string& artist,
                      const std::string& title);
    void _sorted_insert_song_(const std::string& str);
    void _create_threads_();
    void _run_mpd_();

public:
    /**
     * @brief Default initializes the @c Consumer.
     */
    Consumer() = default;

    /**
     * @brief Reads the lines of @c _playlist_lines_ from @c
     * playlist_file.
     *
     * @post _playlist_file_ == playlist_file
     * @post _write_to_file_ == true
     */
    explicit Consumer(std::string&& playlist_file);

    /**
     * @brief If @c _write_to_file_, then dump @c _playlist_lines_
     * into @c _playlist_file_.
     */
    ~Consumer();

    /**
     * @brief "compiles" a music file.
     *
     * Renames the file and moves it into the music directory.  This
     * will also assign tags based on the new filename.
     */
    void consume(const boost::filesystem::path& mp3_file);
};

#endif

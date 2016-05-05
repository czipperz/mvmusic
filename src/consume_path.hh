#ifndef HEADER_GUARD_CONSUME_PATH_H
#define HEADER_GUARD_CONSUME_PATH_H

#include <boost/filesystem.hpp>
#include <mutex>
#include <future>
#include <vector>
#include <set>

class Consumer {
    std::string _playlist_file_;
    std::set<std::string> _playlist_lines_;
    std::vector<std::future<void> > _futures_;
    std::mutex _print_mutex_, _playlist_mutex_;
    bool _write_to_file_ = false;

    void _raw_consume_(const boost::filesystem::path& mp3_file);
    void _apply_tags_(const boost::filesystem::path& mp3_file,
                      const std::string& artist,
                      const std::string& title);
    void _sorted_insert_song_(const std::string& str);

public:
    ~Consumer();

    void consume(const boost::filesystem::path& mp3_file);
};

#endif

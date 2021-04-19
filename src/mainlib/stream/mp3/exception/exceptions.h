#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_EXCEPTION_EXCEPTIONS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_EXCEPTION_EXCEPTIONS_H

#include <exception>

class MP3StreamException : public std::runtime_error {
public:
    explicit MP3StreamException(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

class NotSupportedStream : public MP3StreamException {
public:
    explicit NotSupportedStream() : MP3StreamException("Accepted files must have all frames in MP3 format") {}
};

class InvalidStream : public MP3StreamException {
public:
    explicit InvalidStream(const std::string& what_arg) :
        MP3StreamException(std::string("Invalid frame: ") + what_arg) {}
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_EXCEPTION_EXCEPTIONS_H

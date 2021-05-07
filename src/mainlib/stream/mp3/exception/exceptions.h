#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_EXCEPTION_EXCEPTIONS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_EXCEPTION_EXCEPTIONS_H

#include <exception>

class MP3StreamException : public std::runtime_error {
public:
    explicit MP3StreamException(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

class InvalidStream : public MP3StreamException {
public:
    explicit InvalidStream(const std::string& path) : MP3StreamException("Invalid stream: " + path) {}
};

class NotSupportedStream : public MP3StreamException {
public:
    explicit NotSupportedStream() : MP3StreamException("Accepted files must have all frames in MP3 format") {}
};

class InvalidStreamFormat : public MP3StreamException {
public:
    explicit InvalidStreamFormat(const std::string& what_arg) :
        MP3StreamException("Invalid frame: " + what_arg) {}
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_EXCEPTION_EXCEPTIONS_H

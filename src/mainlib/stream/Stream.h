#ifndef AIXA_SRC_MAINLIB_STREAM_STREAM_H
#define AIXA_SRC_MAINLIB_STREAM_STREAM_H

class Stream {
    virtual ~Stream() = default;

    virtual void read() = 0;
    virtual bool finished() = 0;
};

#endif //AIXA_SRC_MAINLIB_STREAM_STREAM_H

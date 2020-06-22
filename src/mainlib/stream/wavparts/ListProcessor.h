#ifndef AIXA_SRC_MAINLIB_STREAM_WAVPARTS_LISTPROCESSOR_H
#define AIXA_SRC_MAINLIB_STREAM_WAVPARTS_LISTPROCESSOR_H


#include <string>
#include <vector>

#include <mainlib/stream/FileReader.h>

class ListProcessor {
public:
    static std::vector<std::string> extractListData(FileReader &f, unsigned int size);
};


#endif //AIXA_SRC_MAINLIB_STREAM_WAVPARTS_LISTPROCESSOR_H

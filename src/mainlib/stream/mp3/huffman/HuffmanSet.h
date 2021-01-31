#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANSET_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANSET_H

#include "Huffman.h"

class HuffmanSet {
public:
    explicit HuffmanSet(std::vector<std::unique_ptr<Huffman>>& tables) : tables(std::move(tables)) {}
    virtual ~HuffmanSet() = default;

    const Huffman& getTable(std::size_t ind) { return *tables[ind]; }

private:
    std::vector<std::unique_ptr<Huffman>> tables;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANSET_H

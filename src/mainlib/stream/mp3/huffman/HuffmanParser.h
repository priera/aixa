#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANPARSER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANPARSER_H

#include <fstream>

#include "Huffman.h"

class HuffmanParser {
public:
    explicit HuffmanParser(const std::filesystem::path& filePath);

    Huffman buildFromFile();

private:
    Huffman::Tree decodeTableHeader(const std::string& def);
    void parseTreeData(Huffman::Tree& tree);
    void buildTree(const std::vector<Huffman::Symbols>& vector);

    std::ifstream f;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANPARSER_H

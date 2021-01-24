#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANPARSER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANPARSER_H

#include <mainlib/stream/in/ByteReader.h>

#include <fstream>

#include "Huffman.h"
#include "HuffmanSet.h"

class HuffmanParser {
public:
    explicit HuffmanParser(const std::filesystem::path& filePath);

    HuffmanSet* build();

private:
    static constexpr unsigned char MAX_OFFSET = 250;
    static constexpr unsigned int MAX_NODES_PER_LINE = 12;

    Huffman::Tree decodeTableHeader(const std::string& def);
    void buildTree(Huffman::Tree& tree);
    std::vector<Huffman::Symbols> extractSerializedTree();
    void buildNodeOfIndex(Huffman::Node& node, std::size_t i,
                          const std::vector<Huffman::Symbols>& serializedTree);
    void buildReferenceTree(const std::vector<std::unique_ptr<Huffman>>& tables, Huffman::Tree& tree,
                            const std::string& referenceId);

    std::ifstream f;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_HUFFMANPARSER_H

#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

#include <mainlib/stream/in/ByteReader.h>

#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

class Huffman {
public:
    using Symbols = std::pair<unsigned char, unsigned char>;

    struct Node {
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Symbols symbols;
    };

    struct Tree {
        int id;
        std::shared_ptr<Node> root;
        unsigned int xLength;
        unsigned int yLength;
        unsigned int linbits;
        unsigned int length;
    };

    static constexpr std::size_t NR_HUFFMAN_TABLES = 34;

    Huffman(std::vector<Tree> tables, ByteReader& reader) : tables(std::move(tables)), reader(&reader) {}
    virtual ~Huffman() = default;

    void decode(std::size_t table, int& x, int& y);

private:
    const Symbols& extractSymbolFromTree(const Node& node);
    int addLinbitsToSymbol(unsigned char symbol, int length, unsigned int linbits);

    std::vector<Tree> tables;
    ByteReader* reader;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

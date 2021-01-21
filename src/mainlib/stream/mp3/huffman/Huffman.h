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

    explicit Huffman(Tree table) : table(std::move(table)) {}
    virtual ~Huffman() = default;

    void decode(ByteReader& reader, int& x, int& y) const;

    const Tree& getTable() const { return table; }

private:
    const Symbols& extractSymbolFromTree(ByteReader& reader, const Node& node) const;
    int addLinbitsToSymbol(ByteReader& reader, unsigned char symbol, int length, unsigned int linbits) const;

    Tree table;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

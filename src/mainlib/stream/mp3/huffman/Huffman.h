#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

#include <mainlib/stream/in/BitInputReader.h>

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

    void decodeBigValues(BitInputReader& reader, int& s1, int& s2) const;

    void decodeCount1(BitInputReader& reader, int& s1, int& s2, int& s3, int& s4) const;

    const Tree& getTable() const { return table; }

private:
    void extractSymbol(BitInputReader& reader, int& s1, int& s2) const;
    const Symbols& extractSymbolFromNode(BitInputReader& reader, const Node& node) const;
    void addLinbitsToSymbol(BitInputReader& reader, int& symbol, int length, unsigned int linbits) const;

    Tree table;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

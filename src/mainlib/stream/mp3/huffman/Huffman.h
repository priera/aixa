#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

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

    explicit Huffman(std::vector<Tree> tables) : tables(std::move(tables)) {}
    virtual ~Huffman() = default;

    Symbols decode(std::size_t table);

private:
    std::vector<Tree> tables;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_HUFFMAN_H

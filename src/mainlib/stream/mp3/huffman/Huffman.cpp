#include "Huffman.h"

void Huffman::decode(std::size_t table, int& x, int& y) {
    if (table == 0) {
        x = 0;
        y = 0;
        return;
    }

    const auto& huffTree = tables[table];
    const auto& symbol = extractSymbolFromTree(*huffTree.root);

    if (huffTree.id == 32 || huffTree.id == 33) throw std::runtime_error("Not supported yet huff decoding");

    x = addLinbitsToSymbol(symbol.first, huffTree.xLength, huffTree.linbits);
    y = addLinbitsToSymbol(symbol.second, huffTree.yLength, huffTree.linbits);
}

const Huffman::Symbols& Huffman::extractSymbolFromTree(const Huffman::Node& node) {
    if (!node.left) {
        return node.symbols;
    } else {
        const auto& nextTree = (reader->nextBit()) ? *node.right : *node.left;
        return extractSymbolFromTree(nextTree);
    }
}

int Huffman::addLinbitsToSymbol(unsigned char symbol, int length, unsigned int linbits) {
    int ret = symbol;
    if (linbits && ret == length - 1) {
        ret += reader->nextNBits(linbits);
    }

    if (ret != 0) {
        ret *= (reader->nextBit()) ? -1 : 1;
    }

    return ret;
}

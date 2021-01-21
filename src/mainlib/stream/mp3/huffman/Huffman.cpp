#include "Huffman.h"

void Huffman::decode(ByteReader& reader, int& x, int& y) const {
    if (table.id == 0) {
        x = 0;
        y = 0;
        return;
    }

    const auto& symbol = extractSymbolFromTree(reader, *table.root);

    if (table.id == 32 || table.id == 33) throw std::runtime_error("Not supported yet huff decoding");

    x = addLinbitsToSymbol(reader, symbol.first, table.xLength, table.linbits);
    y = addLinbitsToSymbol(reader, symbol.second, table.yLength, table.linbits);
}

const Huffman::Symbols& Huffman::extractSymbolFromTree(ByteReader& reader, const Huffman::Node& node) const {
    if (!node.left) {
        return node.symbols;
    } else {
        const auto& nextTree = (reader.nextBit()) ? *node.right : *node.left;
        return extractSymbolFromTree(reader, nextTree);
    }
}

int Huffman::addLinbitsToSymbol(ByteReader& reader, unsigned char symbol, int length,
                                unsigned int linbits) const {
    int ret = symbol;
    if (linbits && ret == length - 1) {
        ret += reader.nextNBits(linbits);
    }

    if (ret != 0) {
        ret *= (reader.nextBit()) ? -1 : 1;
    }

    return ret;
}

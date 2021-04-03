#include "Huffman.h"

void Huffman::decodeBigValues(BitInputReader& reader, int& s1, int& s2) const {
    extractSymbol(reader, s1, s2);

    addLinbitsToSymbol(reader, s1, table.xLength, table.linbits);
    addLinbitsToSymbol(reader, s2, table.yLength, table.linbits);
}

void Huffman::decodeCount1(BitInputReader& reader, int& s1, int& s2, int& s3, int& s4) const {
    auto checkInvertSymbol = [&reader](int& s) {
        if (s != 0)
            s *= (reader.nextBit()) ? -1 : 1;
    };

    int temp;
    extractSymbol(reader, s1, temp);

    s3 = (temp >> 3) & 1;
    s4 = (temp >> 2) & 1;
    s1 = (temp >> 1) & 1;
    s2 = temp & 1;

    checkInvertSymbol(s3);
    checkInvertSymbol(s4);
    checkInvertSymbol(s1);
    checkInvertSymbol(s2);
}

void Huffman::extractSymbol(BitInputReader& reader, int& s1, int& s2) const {
    if (!table.root) {
        s1 = 0;
        s2 = 0;
        return;
    }

    const auto& symbol = extractSymbolFromNode(reader, *table.root);
    s1 = symbol.first;
    s2 = symbol.second;
}

const Huffman::Symbols& Huffman::extractSymbolFromNode(BitInputReader& reader,
                                                       const Huffman::Node& node) const {
    if (!node.left) {
        return node.symbols;
    } else {
        const auto& nextTree = (reader.nextBit()) ? *node.right : *node.left;
        return extractSymbolFromNode(reader, nextTree);
    }
}

void Huffman::addLinbitsToSymbol(BitInputReader& reader,
                                 int& symbol,
                                 int length,
                                 unsigned int linbits) const {
    if (linbits && symbol == length - 1) {
        unsigned short tmp;
        reader.nextNBits(linbits, tmp);
        symbol += tmp;
    }

    if (symbol != 0) {
        symbol *= (reader.nextBit()) ? -1 : 1;
    }
}

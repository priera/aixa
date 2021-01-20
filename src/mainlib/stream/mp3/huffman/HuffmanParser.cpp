#include "HuffmanParser.h"

HuffmanParser::HuffmanParser(const std::filesystem::path& filePath) : f(filePath) {
    if (!f) {
        throw std::runtime_error("Could not find huffman tables file");
    }
}

Huffman HuffmanParser::buildFromFile() {
    std::vector<Huffman::Tree> tables(Huffman::NR_HUFFMAN_TABLES);
    Huffman::Tree lastTree;

    bool fileProcessed = false;
    while (!(fileProcessed || f.eof())) {
        std::string line;
        std::getline(f, line);

        // A line has at least one character: \n. A line of 0 characters would actually be an error
        if (line.size() <= 1 || line[0] == '#') continue;

        bool storeTable = false;

        auto tokenEnd = line.find_first_of(" \n\r");
        auto token = line.substr(1, tokenEnd - 1);
        if (token == "table") {
            lastTree = decodeTableHeader(line.substr(tokenEnd));
        } else if (token == "treedata") {
            parseTreeData(lastTree);
            storeTable = true;
        } else if (token == "reference") {
            storeTable = true;
        } else if (token == "end") {
            fileProcessed = true;
        }

        if (storeTable) {
            tables[lastTree.id] = lastTree;
        }
    }

    if (!fileProcessed) throw std::runtime_error("Invalid huffman file");

    return Huffman(tables);
}

Huffman::Tree HuffmanParser::decodeTableHeader(const std::string& def) {
    std::stringstream s(def);
    Huffman::Tree t;
    s >> t.id >> t.length >> t.xLength >> t.yLength >> t.linbits;

    return t;
}

void HuffmanParser::parseTreeData(Huffman::Tree& tree) {
    std::string line;
    std::getline(f, line);
    std::vector<Huffman::Symbols> serializedTree;

    while (line.size() > 1) {
        std::stringstream s(line);
        auto& hexStream = s >> std::hex;
        while (s) {
            unsigned int s1, s2;
            hexStream >> s1 >> s2;

            serializedTree.emplace_back(s1, s2);
        }

        std::getline(f, line);
    }

    buildTree(serializedTree);
}

void HuffmanParser::buildTree(const std::vector<Huffman::Symbols>& vector) {}

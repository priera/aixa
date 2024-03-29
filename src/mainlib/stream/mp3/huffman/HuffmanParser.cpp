#include "HuffmanParser.h"

HuffmanParser::HuffmanParser(const std::filesystem::path& filePath) : f(filePath) {
    if (!f) {
        throw std::runtime_error("Could not find huffman tables file");
    }
}

HuffmanSet* HuffmanParser::build() {
    std::vector<std::unique_ptr<Huffman>> tables(Huffman::NR_HUFFMAN_TABLES);
    Huffman::Tree lastTree;

    bool fileProcessed = false;
    while (!(fileProcessed || f.eof())) {
        std::string line;
        std::getline(f, line);

        if (line.size() <= 1 || line[0] == '#') continue;

        bool storeTable = false;

        auto tokenEnd = line.find_first_of(" \n\r");
        auto token = line.substr(1, tokenEnd - 1);
        if (token == "table") {
            lastTree = decodeTableHeader(line.substr(tokenEnd));
        } else if (token == "treedata") {
            buildTree(lastTree);
            storeTable = true;
        } else if (token == "reference") {
            buildReferenceTree(tables, lastTree, line.substr((tokenEnd)));
            storeTable = true;
        } else if (token == "end") {
            fileProcessed = true;
        }

        if (storeTable) {
            tables[lastTree.id] = std::make_unique<Huffman>(lastTree);
            lastTree = Huffman::Tree();
        }
    }

    if (!fileProcessed) throw std::runtime_error("Invalid huffman file");

    return new HuffmanSet(tables);
}

Huffman::Tree HuffmanParser::decodeTableHeader(const std::string& def) {
    std::stringstream s(def);
    Huffman::Tree t;
    s >> t.id >> t.length >> t.xLength >> t.yLength >> t.linbits;

    return t;
}

void HuffmanParser::buildTree(Huffman::Tree& tree) {
    auto serializedTree = extractSerializedTree();

    if (!serializedTree.empty()) {
        tree.root = std::make_shared<Huffman::Node>();
        buildNodeOfIndex(*tree.root, 0, serializedTree);
    }
}

std::vector<Huffman::Symbols> HuffmanParser::extractSerializedTree() {
    std::string line;
    std::getline(f, line);
    std::vector<Huffman::Symbols> serializedTree;
    unsigned int nodesCaptured;

    while (line.size() > 1) {
        std::istringstream s(line);
        auto& hexStream = s >> std::hex;
        nodesCaptured = 0;
        while (hexStream && nodesCaptured < MAX_NODES_PER_LINE) {
            int s1, s2;
            hexStream >> s1 >> s2;
            serializedTree.emplace_back(s1, s2);
            nodesCaptured++;
        }
        std::getline(f, line);
    }

    return serializedTree;
}

void HuffmanParser::buildNodeOfIndex(Huffman::Node& parent, std::size_t i,
                                     const std::vector<Huffman::Symbols>& serializedTree) {
    auto chooseValue = [](const Huffman::Symbols& p, bool ones) -> unsigned char {
        return (ones) ? p.second : p.first;
    };

    auto computeNextOffset = [&serializedTree, &chooseValue](std::size_t initial, bool ones) -> std::size_t {
        auto ret = initial;
        auto value = chooseValue(serializedTree[ret], ones);
        while (value >= MAX_OFFSET) {
            ret += value;
            value = chooseValue(serializedTree[ret], ones);
        }
        ret += value;
        return ret;
    };

    const auto& entry = serializedTree[i];

    if (entry.first == 0) {
        unsigned char s1, s2;
        s1 = entry.second >> 4;
        s2 = entry.second & 0x0F;
        parent.symbols = std::make_pair(s1, s2);

    } else {
        parent.left = std::make_unique<Huffman::Node>();
        parent.right = std::make_unique<Huffman::Node>();

        auto zeroesOffset = computeNextOffset(i, false);
        auto onesOffset = computeNextOffset(i, true);

        buildNodeOfIndex(*parent.left, zeroesOffset, serializedTree);
        buildNodeOfIndex(*parent.right, onesOffset, serializedTree);
    }
}

void HuffmanParser::buildReferenceTree(const std::vector<std::unique_ptr<Huffman>>& tables,
                                       Huffman::Tree& tree, const std::string& referenceId) {
    std::stringstream s(referenceId);
    std::size_t id;
    s >> id;

    auto referenced = tables[id]->getTable().root;
    if (!referenced) {
        throw std::runtime_error("Referencing wrong table");
    }
    tree.root = referenced;
}

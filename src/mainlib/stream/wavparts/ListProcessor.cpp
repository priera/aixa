#include "ListProcessor.h"

std::vector<std::string> ListProcessor::extractListData(FileReader &f, unsigned int size) {
    std::vector<std::string> ret;

    std::string infoTag;
    f.nextIdTag(infoTag);
    size -= 4;

    while (size > 0) {
        std::string infoIdTag;
        f.nextIdTag(infoIdTag);

        unsigned int textSize;
        f.nextWord(textSize);

        std::string infoText(textSize, '\0');
        f.readString(infoText, textSize);

        size -= 8 + textSize;
        ret.push_back(infoText);
    }

    return ret;
}
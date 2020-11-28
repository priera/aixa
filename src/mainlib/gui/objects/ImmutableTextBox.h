#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H


#include <mainlib/gui/scene/ShadedRenderableObject.h>

class ImmutableTextBox : public ShadedRenderableObject {
public:
    ImmutableTextBox(std::string text, unsigned int pixelSize, double x, double y);
    ~ImmutableTextBox() override = default;

protected:
    void init() override;

private:
    struct CharDisplayData {
        std::vector<float> vertices;
        unsigned int charTextureId;
    };

    std::string text;
    unsigned int pixelSize;
    double x, y;

    std::vector<CharDisplayData> charData;

};


#endif //AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H

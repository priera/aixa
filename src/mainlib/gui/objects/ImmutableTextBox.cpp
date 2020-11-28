#include "ImmutableTextBox.h"

#include <mainlib/gui/gl/ShadersCollection.h>

ImmutableTextBox::ImmutableTextBox(std::string text, unsigned int pixelSize, double x, double y) :
    ShadedRenderableObject(ShadersCollection::VERTEX_TEXTURED_PLANE,
                           ShadersCollection::FRAG_CHARACTER,
                           Dimensions{0.9f, 1.125f, 0.1f}),
    text(std::move(text)),
    pixelSize(pixelSize),
    x(x),
    y(y) { }

void ImmutableTextBox::init() {
    charData.resize(text.size());

    for (auto c: text) {

    }
}

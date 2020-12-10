#include "FreeTypeUtils.h"

#include <iostream>
#include <sstream>

namespace FreeTypeUtils {
void printCharacter(const FT_Bitmap &glyph) {
    FT_Int i, j;
    FT_Int x_max = glyph.width;
    FT_Int y_max = glyph.rows;

    std::stringstream s;

    for (i = 0; i < x_max; i++) {
        for (j = 0; j < y_max; j++) {
            unsigned char c = glyph.buffer[i * glyph.width + j];
            char toPrint = (c == 0) ? ' ' : ((c < 128) ? '+' : '*');
            s << toPrint;
        }

        s << "\n";
    }
    std::cout << s.str() << std::endl;
}
}  // namespace FreeTypeUtils

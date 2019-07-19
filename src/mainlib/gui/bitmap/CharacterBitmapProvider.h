#ifndef ALSAPLAYGROUND_CHARACTERBITMAPPROVIDER_H
#define ALSAPLAYGROUND_CHARACTERBITMAPPROVIDER_H


class CharacterBitmapProvider {
public:
    struct Character {
        unsigned int    rows;
        unsigned int    width;
        int             bitmap_top;
        int             bitmap_left;
        long int        advance;
        unsigned char*  buffer;
    };


    virtual ~CharacterBitmapProvider();
    virtual Character getCharacter(char c) = 0;

};


#endif //ALSAPLAYGROUND_CHARACTERBITMAPPROVIDER_H

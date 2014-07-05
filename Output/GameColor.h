//
//  GameColor.h
//  SpriteFight
//
//  Created by aleuer2 on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__GameColor__
#define __SpriteFight__GameColor__

#define Byte unsigned char

#include <iostream>

struct GameColor {
    
    Byte r ;
    Byte g ;
    Byte b ;
    Byte a ;
    
    GameColor() :
        r(0x00), g(0x00),
        b(0x00), a(0x00) {}
};

#endif /* defined(__SpriteFight__GameColor__) */

//
//  GameColor.h
//  SpriteFight
//
//  Created by aleuer2 on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__GameColor__
#define __SpriteFight__GameColor__

#include <iostream>

#include <SDL2/SDL_pixels.h>

#include "../Control/Configuration.h"

typedef unsigned char Byte ;

struct GameColor {
    
    Byte red ;
    Byte green ;
    Byte blue ;
    Byte alpha ;
    
    GameColor() :
        red(0x00), green(0x00),
        blue(0x00), alpha(0x00) {}
    
    GameColor(Byte r, Byte g, Byte b, Byte a) :
        red(r), green(g), blue(b), alpha(a)     {}
	
	GameColor(Byte value) :
		red(value), green(value), blue(value), alpha(value)     {}
	
    GameColor(const GameColor & other) :
        red(other.red), green(other.green),
        blue(other.blue), alpha(other.alpha) {}
    
    ~GameColor() {}
    
    GameColor & operator=(const GameColor & rhs) {
        if (this != &rhs) {
            this->red = rhs.red ;
            this->green = rhs.green ;
            this->blue = rhs.blue ;
            this->alpha = rhs.alpha ;
        }
        return *this ;
    }
	
	inline bool operator==(const GameColor & rhs) const {
		if ( (this->red == rhs.red) && (this->green == rhs.green) && (this->blue == rhs.blue) && (this->alpha == rhs.alpha)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	inline bool operator!=(const GameColor & rhs) const {
		return !(operator==(rhs)) ;
	}
    
    inline Color convertToSDL_Color() const {
        Color color({red, green, blue, alpha}) ;
        return color ;
    }
    
    
} ;

#endif /* defined(__SpriteFight__GameColor__) */

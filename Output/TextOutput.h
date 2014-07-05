//
//  TextOutput.h
//  SpriteFight
//
//  Created by Adam James Leuer on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__TextOutput__
#define __SpriteFight__TextOutput__

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <sstream>
#include <exception>

#include <cstdlib>

#include <SDL2_ttf/SDL_ttf.h>

#include "../Util/Size.hpp"

#include "GameColor.h"

using namespace std ;

class TextOutput {
	
protected:
    
	friend class GraphicalOutput ;
    
    static TTF_Font * gameFont ;
    
    static vector<Texture *> allText ;
    
public:
	
	static void init() ;
    
    static Size<unsigned> getSizeOfText(string str) ;
    
    static void outputText(const string & text) ;
	
	static void exit() ;
	
} ;

#endif /* defined(__SpriteFight__TextOutput__) */

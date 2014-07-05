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

#include "OutputData.hpp"
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
    
    template<typename N>
    static void outputText(const string & text, GameColor foreground, GameColor background, const Position<N> pos,
                           const Size<int> * size) ;
    
    static void render() ;
	
	static void exit() ;
	
} ;

template<typename N>
void TextOutput::outputText(const string & text, GameColor foreground, GameColor background, const Position<N> pos,
                            const Size<int> * size)

{
    
    Surface * surface ;

    surface = TTF_RenderUTF8_Shaded(gameFont, text.c_str(), foreground.convertToSDL_Color(), background.convertToSDL_Color()) ;
    
}






#endif /* defined(__SpriteFight__TextOutput__) */

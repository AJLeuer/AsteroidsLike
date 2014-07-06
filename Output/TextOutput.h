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
#include <vector>

#include <cstdlib>
#include <cstring>

#include <SDL2_ttf/SDL_ttf.h>

#include "../Util/Size.hpp"
#include "../Util/BasicConcurrency.h"

#include "OutputData.hpp"
#include "GameColor.h"

#include "../World/GameState.h"

using namespace std ;

class TextOutput {
	
protected:
	
	static vector<TextOutput *> allTextOutput ;
    
    static TTF_Font * gameFont ;
	
	
	bool updateFlag = false ;
	
	string text ;
	
	Texture * texture ;
	
	Position<float> position ;
	
	Size<int> size ;
	
	OutputData data ;
	
	GameColor foreground, background ;
	
	
public:
	
	static void init() ;
    
    static Size<int> getSizeOfText(const string & str) ;
	
	static const vector<TextOutput *> * getAllTextOutput() { return & allTextOutput ; }
	
	static void exit() ;
	

    
    //static void outputText(const string & text, GameColor foreground, GameColor background, const Position<float> pos,
	// const Size<int> * size) ;
	
	/**
	 * Creates a TextOutput object which will automatically output the string text to the screen, at the chosen position position, at the
	 * next output update. TextOutput assumes that all pointers given to it remain valid for calling refresh()
	 */
	TextOutput(const string & text, const Position<float> & pos, GameColor foreground, GameColor background) ;
	
	~TextOutput() { erase() ; }
	
	void update() ;
	
	void updateText(const string & newText) ;
	
	void updatePosition(const Position<float> & pos) ;
	
	void updateForegroundColor(GameColor color) ;
	
	void updateBackgroundColor(GameColor color) ;
	
	OutputData * getOutputData() { update() ; return &(this->data) ; }
	
	void erase() { text = "" ; if (texture != nullptr) SDL_DestroyTexture(texture) ; texture = nullptr ;}
	
	
    
} ;









#endif /* defined(__SpriteFight__TextOutput__) */

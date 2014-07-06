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
	
	
	const string * text ;
	
	OutputData data ;
	
	const GameColor * foreground, * background ;
	
	Texture * texture ;

	
	bool hasNulls() ;
	
	
public:
	
	static void init() ;
    
    static Size<unsigned> getSizeOfText(const string & str) ;
	
	static const vector<TextOutput *> * getAllTextOutput() { return & allTextOutput ; }
	
	static void exit() ;
	

    
    //static void outputText(const string & text, GameColor foreground, GameColor background, const Position<float> pos,
	// const Size<int> * size) ;
	
	/**
	 * Creates a TextOutput object which will automatically output the string text to the screen, at the chosen position position, at the
	 * next output update. TextOutput assumes that all pointers given to it remain valid for calling refresh()
	 */
	TextOutput(const string * text, const Position<float> * pos, const Size<int> * size, GameColor * foreground, GameColor * background) ;
	
	/**
	 * Updates the main output with new values. TextOutput assumes that all pointers sent as arguments during its construction remain valid.
	 * Any changing of the values output by TextOutput must be done by changing the objects pointed to by the pointers held by TextOutput (TextOutput
	 * is unable to alter any of its members). What TextOutput *can* do is change the output onscreen based on new values read from text, or size,
	 * etc. If TextOutput discovers that any of its values have become null, it will immediately delete itself.
	 */
	void update() ;
	
	OutputData * getOutputData() { update() ; return &(this->data) ; }
	
	void erase() { SDL_DestroyTexture(texture) ; }
	
	~TextOutput() { erase() ; }
    
} ;









#endif /* defined(__SpriteFight__TextOutput__) */

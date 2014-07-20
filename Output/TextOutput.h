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
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <sstream>
#include <exception>
#include <vector>
#include <functional>

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
	

    static TTF_Font * gameFont ;
	
	static BasicMutex textMutex ;
	
	static vector<TextOutput *> allTextOutput ;
	
	static const vector<const OutputData<float, int> *> * viewOutputData ; /* debug variable, remove this */
	
	
	bool updateFlag = false ;
	
	string text ;
	
	Position<float> position ;
	
	Size<int> size ;
	
	OutputData<float, int> data ;
	
	GameColor foreground, background ;
	
	friend class GraphicalOutput ;
	
	
public:
	
	static void init() ;
    
    static Size<int> getSizeOfText(const string & str) ;
	
	static void updateAll() ;
	
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
	
	OutputData<float, int> * getOutputData() { update() ; return &(this->data) ; }
	
	const string * viewText() { return & text ; }
	
	void erase() { text = "" ;}
	
	/**
	 * @brief Draws a continuously updating text representation of 
	 *		  updatingText, overriding this->text. When using this function
	 *		  there is no need to call updateText().
	 * 
	 * @param updatingText The text to draw
	 */
	static void displayContinuousText(const string * updatingText, const Position<float> & pos, GameColor foreground, GameColor background) ;
	
	/**
	 * @brief Draws a continuously updating text representation of the string returned
	 *		  by stringUpdatingFunction, overriding this->text. When using this function
	 *		  there is no need to call updateText().
	 *
	 * @param stringUpdatingFunction A function that returns the text to draw
	 */
	static void displayContinuousText(function<const string (void)> stringUpdatingFunction, const Position<float> & pos, GameColor foreground, GameColor background) ;
	
	
    
} ;









#endif /* defined(__SpriteFight__TextOutput__) */

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

#include "../World/GameState.hpp"

using namespace std ;

template<typename POSUTYPE, typename SIZEUTYPE>
class TextOutput : public OutputData<POSUTYPE, SIZEUTYPE> {
	
protected:
	

    static TTF_Font * gameFont ;
	
	static BasicMutex textMutex ;
	
	static const vector<OutputData<POSUTYPE, SIZEUTYPE> *> * viewOutputData ; /* debug variable, remove this */
	
	
	string text ;
	
	//Position<POSUTYPE> position ;
	
	Size<SIZEUTYPE> size ;
	
	//OutputData<POSUTYPE, SIZEUTYPE> outputData ;
	
	GameColor foreground, background ;
	
	friend class GraphicalOutput ;
	
	void initGraphicsData() ;
    
    void update() ;
	
public:
	
	static void init() ;
    
    static Size<SIZEUTYPE> getSizeOfText(const string & str) ;
	
	static void exit() ;
	

    //static void outputText(const string & text, GameColor foreground, GameColor background, const Position<POSUTYPE> pos,
	// const Size<SIZEUTYPE> * size) ;
	
	/**
	 * Creates a TextOutput object which will automatically output the string text to the screen, at the chosen position position, at the
	 * next output update. TextOutput assumes that all pointers given to it remain valid for calling refresh()
	 */
	TextOutput(const string & text, const Position<POSUTYPE> & pos, GameColor foreground, GameColor background) ;
	
	~TextOutput() { erase() ; }
	
	void updateText(const string & newText) ;
	
	void updatePosition(const Position<POSUTYPE> & pos) ;
	
	void updateForegroundColor(GameColor color) ;
	
	void updateBackgroundColor(GameColor color) ;
	
	//void setTexture(Texture * texture) ;
	
	const OutputData<POSUTYPE, SIZEUTYPE> * getOutputData() { return this ; }
	
	const string * viewText() { return & text ; }
	
	void erase() { text = "" ;}
	
	/**
	 * @brief Draws a continuously updating text representation of 
	 *		  updatingText, overriding this->text. When using this function
	 *		  there is no need to call updateText().
	 * 
	 * @param updatingText The text to draw
	 */
	static void displayContinuousText(const string * updatingText, const Position<POSUTYPE> & pos, GameColor foreground, GameColor background) ;
	
	/**
	 * @brief Draws a continuously updating text representation of the string returned
	 *		  by stringUpdatingFunction, overriding this->text. When using this function
	 *		  there is no need to call updateText().
	 *
	 * @param stringUpdatingFunction A function that returns the text to draw
	 */
	static void displayContinuousText(function<const string (void)> stringUpdatingFunction, const Position<POSUTYPE> & pos, GameColor foreground, GameColor background) ;
	
	
    
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
TTF_Font * TextOutput<POSUTYPE, SIZEUTYPE>::gameFont = nullptr ;

template<typename POSUTYPE, typename SIZEUTYPE>
BasicMutex TextOutput<POSUTYPE, SIZEUTYPE>::textMutex ;

template<typename POSUTYPE, typename SIZEUTYPE>
const vector<OutputData<POSUTYPE, SIZEUTYPE> *> * TextOutput<POSUTYPE, SIZEUTYPE>::viewOutputData = OutputData<POSUTYPE, SIZEUTYPE>::getOutputData() ; /* debug variable, remove this */

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::init() {
	int sdlinit_error = TTF_Init() ;
	
	if (sdlinit_error == -1) {
		stringstream ss ;
		ss << "TTF_Init() failed. Outputting error:" << '\n' ;
		ss << TTF_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	
	gameFont = TTF_OpenFont("/Assets/Fonts/Roboto-Regular.ttf", 24) ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
Size<SIZEUTYPE> TextOutput<POSUTYPE, SIZEUTYPE>::getSizeOfText(const string & str) {
	
	int w ;
	int h ;
	
	textMutex.lock() ;
	
	TTF_SizeUTF8(gameFont, str.c_str(), &w, &h) ;
	
	textMutex.unlock() ;
	
	//free(cstr) ;
	
	Size<SIZEUTYPE> textSize(w, h) ;
	
	return textSize ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::exit() {
	TTF_CloseFont(gameFont) ;
	TTF_Quit();
}

template<typename POSUTYPE, typename SIZEUTYPE>
TextOutput<POSUTYPE, SIZEUTYPE>::TextOutput(const string & text, const Position<POSUTYPE> & pos, GameColor foreground, GameColor background) :
	OutputData<POSUTYPE, SIZEUTYPE>(new Position<POSUTYPE>(pos), 1.0, PositionType::screenPosition),
	text(text), size(), foreground(foreground), background(background)
{
	
	/* we can't create any SDL_Textures here because we don't know that we're on the main thread. Instead
		OutputData simply sets the updateFlag */
	
	/* initFlag = true */
}


//must be called from main thread
template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::initGraphicsData() {
	
	if (this->initFlag) { /* check if we actually need to update anything */
		
		Surface * surface = TTF_RenderUTF8_Blended(gameFont, text.c_str(), foreground.convertToSDL_Color()) ;
		
		/* Debug code */
		stringstream ss ;
		ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* End debug code */
		
		this->setTexture(SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface)) ;
		
		/* Debug code */
		stringstream st ;
		st << "Checking for SDL errors after call to SDL_CreateTextureFromSurface(): " << SDL_GetError() << '\n' ;
		DebugOutput << st.rdbuf() ;
		/* End debug code */
		
		SDL_FreeSurface(surface) ;
		
		size = getSizeOfText(text) ;
		
		/* reset the update flag */
		this->initFlag = false ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::update() {
    
    if (this->checkIfUpdated()) { /* check if we actually need to update anything */
		
        this->OutputData<POSUTYPE, SIZEUTYPE>::update() ;
        
		Surface * surface = TTF_RenderUTF8_Blended(gameFont, text.c_str(), foreground.convertToSDL_Color()) ;
		
		/* Debug code */
		stringstream ss ;
		ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* End debug code */
		
		this->setTexture(SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface)) ;
		
		/* Debug code */
		stringstream st ;
		st << "Checking for SDL errors after call to SDL_CreateTextureFromSurface(): " << SDL_GetError() << '\n' ;
		DebugOutput << st.rdbuf() ;
		/* End debug code */
		
		SDL_FreeSurface(surface) ;
		
		size = getSizeOfText(text) ;
		
		/* reset the update flag */
		this->updateFlag = false ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updateText(const string & newText) {
	if (this->text != newText) {
		this->text = newText ;
		size = getSizeOfText(text) ;
		/* set updateflag so update() knows to run */
		this->updateFlag = true ;
	}
	/* else leave updateflag false so we don't waste GPU/CPU resources redrawing */
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updatePosition(const Position<POSUTYPE> & pos) {
	if (*this->position != pos) {
		this->position->setAll(pos) ;
		/* set updateflag so update() knows to run */
		this->updateFlag = true ;
	}
	/* else leave updateflag false so we don't waste GPU/CPU resources redrawing */
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updateForegroundColor(GameColor color) {
	if (this->foreground != color) {
		foreground = color ;
		/* set updateflag so update() knows to run */
		this->updateFlag = true ;
	}
	/* else leave updateflag false so we don't waste GPU/CPU resources redrawing */
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updateBackgroundColor(GameColor color) {
	if (this->background != color) {
		background = color ;
		/* set updateflag so update() knows to run */
		this->updateFlag = true ;
	}
	/* else leave updateflag false so we don't waste GPU/CPU resources redrawing */
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::displayContinuousText(const string * updatingText, const Position<POSUTYPE> & pos, GameColor foreground, GameColor background) {
	
	TextOutput * textoutput = new TextOutput(*updatingText, pos, foreground, background) ;
	
	auto continuousTextDisplay = [=] () -> void {
		
		while (GLOBAL_CONTINUE_FLAG) {
			textoutput->updateText(*updatingText) ;
			this_thread::sleep_for(defaultSleepTime) ;
		}
		
		delete textoutput ;
		
	} ;
	
	thread thr(continuousTextDisplay) ;
	thr.detach() ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::displayContinuousText(function<const string (void)> stringUpdatingFunction, const Position<POSUTYPE> & pos, GameColor foreground, GameColor background) {
	
	TextOutput * textoutput = new TextOutput(stringUpdatingFunction(), pos, foreground, background) ;
	
	auto continuousTextDisplay = [=] () -> void {
		
		while (GLOBAL_CONTINUE_FLAG) {
			textoutput->updateText(stringUpdatingFunction()) ;
			this_thread::sleep_for(defaultSleepTime) ;
		}
		
		delete textoutput ;
		
	} ;
	
	thread thr(continuousTextDisplay) ;
	thr.detach() ;
}










#endif /* defined(__SpriteFight__TextOutput__) */

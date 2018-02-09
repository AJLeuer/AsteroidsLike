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

#include "GraphicsData.hpp"
#include "GameColor.h"

#include "../World/GameState.hpp"

using namespace std ;

/**
 * @note Must be created with heap allocated memory. Do not call delete, this is done automatically.
 * @see GraphicsData for more detailed explanation
 */
template<typename POSUTYPE, typename SIZEUTYPE>
class TextOutput : public GraphicsData<POSUTYPE, SIZEUTYPE> {
	
protected:
	

    static TTF_Font * gameFont ;
	
	static BasicMutex textMutex ;
	
	static const vector<GraphicsData<POSUTYPE, SIZEUTYPE> *> * viewOutputData ; /* debug variable, remove this */
	
	string text ;
	
	bool text_was_updated = false ;
	
	GameColor foreground, background ;
	
	bool color_was_updated = false ;
	
	bool position_was_updated ;
    

    /**
     * @brief Destructor for TextOutput
     */
    ~TextOutput() { erase() ; }
	
	
	void addAdditionalUpdateFlags() ;
	
	friend class GraphicalOutput ;
	
	void completeInitialization() ;
    
    virtual void update() ;
	
public:
	
	static void init() ;
    
    static Size<SIZEUTYPE> getSizeOfText(const string & str) ;
	
	static void exit() ;
	

	/**
	 * Creates a TextOutput object which will automatically output the string text to the screen, at the chosen position position, at the
	 * next output update. TextOutput assumes that all pointers given to it remain valid for calling refresh()
	 */
	TextOutput(const string & text, const Vect<POSUTYPE> & pos, const Angle orientation, GameColor foreground, GameColor background) ;

	
	void updateText(const string & newText) ;
	
	void updatePosition(const Vect<POSUTYPE> & pos) ;
	
	void updateOrientation(const Angle angle) ;
	
	void updateForegroundColor(GameColor color) ;
	
	void updateBackgroundColor(GameColor color) ;
	
	//void setTexture(Texture * texture) ;
	
	const GraphicsData<POSUTYPE, SIZEUTYPE> * getOutputData() { return this ; }
	
	const string * viewText() { return & text ; }
	
	void erase() { text = "" ;}
	
	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
TTF_Font * TextOutput<POSUTYPE, SIZEUTYPE>::gameFont = nullptr ;

template<typename POSUTYPE, typename SIZEUTYPE>
BasicMutex TextOutput<POSUTYPE, SIZEUTYPE>::textMutex ;

template<typename POSUTYPE, typename SIZEUTYPE>
const vector<GraphicsData<POSUTYPE, SIZEUTYPE> *> * TextOutput<POSUTYPE, SIZEUTYPE>::viewOutputData = GraphicsData<POSUTYPE, SIZEUTYPE>::getOutputData() ; /* debug variable, remove this */

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
TextOutput<POSUTYPE, SIZEUTYPE>::TextOutput(const string & text, const Vect<POSUTYPE> & pos, const Angle orientation, GameColor foreground, GameColor background) :
    GraphicsData<POSUTYPE, SIZEUTYPE>(new Vect<POSUTYPE>(pos), orientation, 1.0, PositionType::screenPosition),
	text(text), foreground(foreground), background(background)
{
	
	/* we can't create any SDL_Textures here because we don't know that we're on the main thread. Instead
		GraphicsData simply sets the needsInit */
	
	/* needsInit = true */
	this->disableCollisionDetection() ;
	
	addAdditionalUpdateFlags() ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::addAdditionalUpdateFlags() {
	
	this->updateFlags.push_back(& color_was_updated) ;
	this->updateFlags.push_back(& text_was_updated) ;
	this->updateFlags.push_back(& position_was_updated) ;
	/* add more here... */
}


//must be called from main thread
template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::completeInitialization() {
	
	{
	/* Debug code */
	#ifdef DEBUG_MODE
	if (this_thread::get_id() != mainThreadID) {
		DebugOutput << "TextOutput::initGraphicsData() can only be called on the main thread \n" ;
		throw exception() ;
	}
	#endif
	/* End Debug code */
	}
	
	if (this->needsInit) { /* check if we actually need to init anything */
		
		Surface * surface = TTF_RenderUTF8_Blended(gameFont, text.c_str(), foreground.convertToSDL_Color()) ;
		
		/* Debug code */
//        stringstream ss ;
//        ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
//        DebugOutput << ss.rdbuf() ;
		/* End debug code */
		
		this->setTexture(SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface)) ;
		
		/* Debug code */
//        stringstream st ;
//        st << "Checking for SDL errors after call to SDL_CreateTextureFromSurface(): " << SDL_GetError() << '\n' ;
//        DebugOutput << st.rdbuf() ;
		/* End debug code */
		
		SDL_FreeSurface(surface) ;
		
		this->size = getSizeOfText(text) ;
		
		/* reset the update flag */
		this->needsInit = false ;
		
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::update() {
	
	{
	/* Debug code */
	#ifdef DEBUG_MODE
	if (this_thread::get_id() != mainThreadID) {
		DebugOutput << "TextOutput::update() can only be called on the main thread \n" ;
		throw exception() ;
	}
	#endif
	/* End Debug code */
	}
    
    if (this->checkIfUpdated()) { /* check if we actually need to update anything */
		
        this->GraphicsData<POSUTYPE, SIZEUTYPE>::update() ;
        
		Surface * surface = TTF_RenderUTF8_Blended(gameFont, text.c_str(), foreground.convertToSDL_Color()) ;
		
		/* Debug code */
		stringstream ss ;
//        ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
//        DebugOutput << ss.rdbuf() ;
		/* End debug code */
		
		this->setTexture(SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface)) ;
		
		/* Debug code */
//        stringstream st ;
//        st << "Checking for SDL errors after call to SDL_CreateTextureFromSurface(): " << SDL_GetError() << '\n' ;
//        DebugOutput << st.rdbuf() ;
		/* End debug code */
		
		SDL_FreeSurface(surface) ;
		
		this->size = getSizeOfText(text) ;
		
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updateText(const string & newText) {
	if (this->text != newText) {
		
		this->text = newText ;
		this->size = getSizeOfText(text) ;
		
		/* set updateflag so update() knows to run */
		text_was_updated = true ;
	}
	/* else leave updateflag false so we don't waste GPU/CPU resources redrawing */
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updatePosition(const Vect<POSUTYPE> & pos) {
	if (*this->position != pos) {
		
		this->position->setAll(pos) ;
		
		/* set updateflag so update() knows to run */
		position_was_updated = true ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updateForegroundColor(GameColor color) {
	if (this->foreground != color) {
		
		foreground = color ;
		
		/* set updateflag so update() knows to run */
		color_was_updated = true ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void TextOutput<POSUTYPE, SIZEUTYPE>::updateBackgroundColor(GameColor color) {
	if (this->background != color) {
		
		background = color ;
		
		/* set updateflag so update() knows to run */
		color_was_updated = true ;
	}
}








template<typename POSUTYPE, typename SIZEUTYPE>
class ContinuousTextOutput : public TextOutput<POSUTYPE, SIZEUTYPE> {
	
protected:
	
	
	static vector <ContinuousTextOutput *> allContinuousTextOutput ;
	
	static bool isInit ;
	
	static void init() ;
	
	static void continuousTextDisplayUpdate() ;
	
	static void removeFromAllContinuousTextOutput(const ContinuousTextOutput * textOutput) ;
	
	

	function<const string (void)> stringUpdatingFunction ;
	
public:
	
	ContinuousTextOutput(const function<const string (void)> & stringUpdatingFunction, const Vect<POSUTYPE> & pos,
						 const Angle orientation, GameColor foreground, GameColor background) :
	
						TextOutput<POSUTYPE, SIZEUTYPE>(stringUpdatingFunction(), pos,
			   				orientation, foreground, background),
						stringUpdatingFunction(stringUpdatingFunction)
	
	{
		//use the first call to a constructor to do initialization for the whole class
		if (isInit == false) {
			init() ;
			//init() will set isInit to true also
		}
	}
	
	~ContinuousTextOutput() {
		removeFromAllContinuousTextOutput(this) ;
	}
	

	
	void display() { allContinuousTextOutput.push_back(this) ; }
	
	void stopDisplaying() const { removeFromAllContinuousTextOutput(this) ; }
	

	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector <ContinuousTextOutput<POSUTYPE, SIZEUTYPE> *>  ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::allContinuousTextOutput = vector <ContinuousTextOutput<POSUTYPE, SIZEUTYPE> *>() ;

template<typename POSUTYPE, typename SIZEUTYPE>
bool ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::isInit = false ;

template<typename POSUTYPE, typename SIZEUTYPE>
void ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::init() {
	
	static thread threadedTextUpdate(& ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::continuousTextDisplayUpdate) ;
	threadedTextUpdate.detach() ;
	
	isInit = true ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::continuousTextDisplayUpdate() {
	
	while (GLOBAL_CONTINUE_FLAG) {
		
		for (auto i = 0 ; i < allContinuousTextOutput.size(); i++) {
			auto currentTextOutput = allContinuousTextOutput[i] ;
			currentTextOutput->updateText(currentTextOutput->stringUpdatingFunction()) ;
		}
		
		this_thread::sleep_for(chrono::milliseconds(384)) ;
	}

}

template<typename POSUTYPE, typename SIZEUTYPE>
void ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::removeFromAllContinuousTextOutput(const ContinuousTextOutput<POSUTYPE, SIZEUTYPE> * textOutput) {
	
	for (auto i = ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::allContinuousTextOutput.begin() ; i != ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::allContinuousTextOutput.end() ; i++) {
		
		if (textOutput == * i) {
			ContinuousTextOutput<POSUTYPE, SIZEUTYPE>::allContinuousTextOutput.erase(i) ;
			break ;
		}
		
	}
}
















#endif /* defined(__SpriteFight__TextOutput__) */

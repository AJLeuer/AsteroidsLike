//
//  TextOutput.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "TextOutput.h"

using namespace std ;

vector<TextOutput *> TextOutput::allTextOutput ; //= vector<TextOutput *>() ;

TTF_Font * TextOutput::gameFont = nullptr ;
BasicMutex TextOutput::textMutex ;


void TextOutput::init() {
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

Size<int> TextOutput::getSizeOfText(const string & str) {
	/*
	 size_t l = strlen(str.c_str()) ; // running into compatibility issues with std::strings
	char * cstr = (char *) malloc(l + 1) ;
    strcpy(cstr, str.c_str()) ; */
    
	int w ;
	int h ;
	
	textMutex.lock() ;
	
    TTF_SizeUTF8(gameFont, str.c_str(), &w, &h) ;
	
	textMutex.unlock() ;
	
	//free(cstr) ;
    
    Size<int> textSize(w, h) ;
    
    return textSize ;
}

void TextOutput::exit() {
    TTF_CloseFont(gameFont) ;
	TTF_Quit();
}


TextOutput::TextOutput(const string & text, const Position<float> & pos, GameColor foreground, GameColor background) :
	text(text), position(pos), size(), data(), foreground(foreground), background(background), texture(nullptr)
{

	size = getSizeOfText(text) ;
	
	data.setAll(&texture, &position, &size) ;
	
	allTextOutput.push_back(this) ;
	
	/* we can't create any SDL_Textures here because we don't know that we're on the main thread. Instead
		we'll simply set the updateFlag */
	
	updateFlag = true ; /* ensure that update() will run the first time through main loop */
}

//must be called from main thread
void TextOutput::update() {
	
	if (updateFlag) { /* check if we actually need to update anything */
		
		if (texture != nullptr) {
			SDL_DestroyTexture(texture) ;
		}
		
		texture = nullptr ;
		
		//const char * cstr { text->c_str() } ;
		//strcpy(cstr, text->c_str()) ;
		
		size_t l = strlen(text.c_str()) ;
		char * cstr = (char *) malloc(l + 1) ;
		memmove(cstr, text.c_str(), (l + 1)) ;
		
		Surface * surface = TTF_RenderUTF8_Shaded(gameFont, cstr, foreground.convertToSDL_Color(), background.convertToSDL_Color()) ;
		
		/* Debug code */
		stringstream ss ;
		ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* End debug code */
		
		texture = SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface) ;
		
		/* Debug code */
		stringstream st ;
		ss << "Checking for SDL errors after call to SDL_CreateTextureFromSurface(): " << SDL_GetError() << '\n' ;
		DebugOutput << st.rdbuf() ;
		/* End debug code */
		
		SDL_FreeSurface(surface) ;
		free(cstr) ;
		
		/* reset the update flag */
		updateFlag = false ;
	}
}

void TextOutput::updateText(const string & newText) {
	this->text = newText ;
	/* set updateflag so update() knows to run */
	updateFlag = true ;
}

void TextOutput::updatePosition(const Position<float> & pos) {
	position.setAll(pos) ;
	/* set updateflag so update() knows to run */
	updateFlag = true ;
}

void TextOutput::updateForegroundColor(GameColor color) {
	foreground = color ;
	/* set updateflag so update() knows to run */
	updateFlag = true ;
}

void TextOutput::updateBackgroundColor(GameColor color) {
	background = color ;
	/* set updateflag so update() knows to run */
	updateFlag = true ;
}






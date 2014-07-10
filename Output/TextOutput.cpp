//
//  TextOutput.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "TextOutput.h"

using namespace std ;

TTF_Font * TextOutput::gameFont = nullptr ;

BasicMutex TextOutput::textMutex ;

vector<TextOutput *> TextOutput::allTextOutput = vector<TextOutput *>() ;

const vector<OutputData *> * TextOutput::viewOutputData = GameState::getAdditionalGraphicalOutputData() ; /* debug variable, remove this */


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

void TextOutput::updateAll() {
	for (auto i = 0 ; i < allTextOutput.size() ; i++) {
		if (allTextOutput.at(i) != nullptr) {
			allTextOutput.at(i)->update() ;
		}
	}
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
	
	
	GameState::addAdditionalGraphicalOutputData(&data) ;
	
	
	/* we can't create any SDL_Textures here because we don't know that we're on the main thread. Instead
		we'll simply set the updateFlag */
	
	updateFlag = true ; /* ensure that update() will run the first time through main loop */
	
	allTextOutput.push_back(this) ;
}

//must be called from main thread
void TextOutput::update() {
	
	if (updateFlag) { /* check if we actually need to update anything */
		
		if (texture != nullptr) {
			SDL_DestroyTexture(texture) ;
		}
		
		texture = nullptr ;
		
		Surface * surface = TTF_RenderUTF8_Shaded(gameFont, text.c_str(), foreground.convertToSDL_Color(), background.convertToSDL_Color()) ;
		
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






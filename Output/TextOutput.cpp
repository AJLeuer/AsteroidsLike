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

const vector<const OutputData<float, int> *> * TextOutput::viewOutputData = GameState::getGraphicalOutputData<float, int>() ; /* debug variable, remove this */


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
	text(text), position(pos), size(), data(&position, &size, PositionType::screenPosition), foreground(foreground), background(background)
{
	
	size = getSizeOfText(text) ;
	

	/* we can't create any SDL_Textures here because we don't know that we're on the main thread. Instead
		we'll simply set the updateFlag */
	
	updateFlag = true ; /* ensure that update() will run the first time through main loop */
	
	allTextOutput.push_back(this) ;
}

//must be called from main thread
void TextOutput::update() {
	
	if (updateFlag) { /* check if we actually need to update anything */
		
		Surface * surface = TTF_RenderUTF8_Blended(gameFont, text.c_str(), foreground.convertToSDL_Color()) ;
		
		/* Debug code */
		stringstream ss ;
		ss << "Checking for TTF or SDL errors after call to TTF_RenderUTF8_Shaded(): " << TTF_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* End debug code */
		
		data.setTexture(SDL_CreateTextureFromSurface(GameState::getMainRenderer(), surface)) ;
		
		/* Debug code */
		stringstream st ;
		st << "Checking for SDL errors after call to SDL_CreateTextureFromSurface(): " << SDL_GetError() << '\n' ;
		DebugOutput << st.rdbuf() ;
		/* End debug code */
		
		SDL_FreeSurface(surface) ;
		
		/* reset the update flag */
		updateFlag = false ;
	}
}

void TextOutput::updateText(const string & newText) {
	this->text = newText ;
	size = getSizeOfText(text) ;
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

void TextOutput::displayContinuousText(const string * updatingText, const Position<float> & pos, GameColor foreground, GameColor background) {
	
	TextOutput * output = new TextOutput(*updatingText, pos, foreground, background) ;
	
	auto continuousTextDisplay = [=] () -> void {
		
		while (GLOBAL_CONTINUE_FLAG) {
			if (*updatingText != output->text) {
				output->updateText(*updatingText) ;
			}
			
			this_thread::sleep_for(defaultSleepTime) ;
		}
		
	} ;
	
	thread thr(continuousTextDisplay) ;
	thr.detach() ;
}

void TextOutput::displayContinuousText(function<const string (void)> stringUpdatingFunction, const Position<float> & pos, GameColor foreground, GameColor background) {
	
	TextOutput * output = new TextOutput(stringUpdatingFunction(), pos, foreground, background) ;
	
	auto continuousTextDisplay = [=] () -> void {
		
		while (GLOBAL_CONTINUE_FLAG) {
			if (stringUpdatingFunction() != *output->viewText()) {
				output->updateText(stringUpdatingFunction()) ;
			}
			
			this_thread::sleep_for(defaultSleepTime) ;
		}
		
	} ;
	
	thread thr(continuousTextDisplay) ;
	thr.detach() ;
}





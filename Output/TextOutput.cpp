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


void TextOutput::init() {
	int sdlinit_error = TTF_Init() ;
	
	if (sdlinit_error == -1) {
		stringstream ss ;
		ss << "TTF_Init() failed. Outputting error:" << '\n' ;
		ss << TTF_GetError() << '\n' ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
    
    gameFont = TTF_OpenFont("/Assets/Fonts/Roboto-Regular.ttf", 18) ;
}

Size<int> TextOutput::getSizeOfText(const string & str) {
    
    int w, h ;
    
    TTF_SizeUTF8(gameFont, str.c_str(), &w, &h) ;
    
    Size<int> textSize(w, h) ;
    
    return textSize ;
}

void TextOutput::exit() {
    TTF_CloseFont(gameFont) ;
	TTF_Quit();
}


TextOutput::TextOutput(const string & text_, const Position<float> & pos, GameColor foreground, GameColor background) :
	text(text_), position(pos), size(getSizeOfText(text_)), data(), foreground(foreground), background(background), texture(nullptr)
{
	data.setAll(texture, &position, &size) ;
	
	Surface * surface = TTF_RenderUTF8_Shaded(gameFont, text.c_str(), foreground.convertToSDL_Color(), background.convertToSDL_Color()) ;
	
	allTextOutput.push_back(this) ;
}

//must be called from main thread
void TextOutput::update() {
	
	data.texture = nullptr ;
	
	if (texture != nullptr) {
		SDL_DestroyTexture(texture) ;
	}

	else {
		
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
		
		data.texture = texture ;
	}
}






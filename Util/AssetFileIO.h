//
//  AssetFileIO.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/27/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__AssetFileIO__
#define __GameWorld__AssetFileIO__


#include <iostream>
#include <vector>
#include <dirent.h>
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>

#include "../Util/Util.hpp"
#include "../Util/BoundsCheck.hpp"
#include "../Gameworld/GameData.h"

using namespace std ;

/**
 * This class will store the names and directory info of all file assets used in the program. 
 * Add the names of any new files added to AssetFileIO.cpp
 */
class AssetFileIO {
	
	static vector<std::string> * characterImageFilenames ;
	/*static vector<string> * sceneryImageFilenames ; */  //todo add this
	
public:
	static SDL_Texture * getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type) ;
	static string & getImageFilename(vector<string>::size_type index, AssetType type) ;
	static string & getRandomImageFilename(AssetType type) ;
};











#endif /* defined(__GameWorld__AssetFileIO__) */

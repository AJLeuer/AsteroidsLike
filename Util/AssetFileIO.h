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
#include <SDL2/SDL_surface.h>
#include <SDL2_image/SDL_image.h>

#include "../Util/Util.hpp"
#include "../Util/BoundsCheck.hpp"
#include "../Gameworld/GameData.h"

using namespace std ;

enum class ImageType {
	character = 0, 
	scenery = 1
	//add more here
};

/**
 * This class will store the names and directory info of all file assets used in the program. 
 * Add the names of any new files added to AssetFileIO.cpp
 */
class AssetFileIO {
	
	static vector<std::string> * characterImageFilenames ;
	/*static vector<string> * sceneryImageFilenames ; */  //todo add this
	
public:
	static SDL_Surface * getSurfaceFromFilename(ImageType type, const string & str) ;
	static string & getImageFilename(ImageType type, vector<string>::size_type index) ;
	static string & getRandomImageFilename(ImageType type) ;
};











#endif /* defined(__GameWorld__AssetFileIO__) */

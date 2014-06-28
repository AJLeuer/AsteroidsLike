//
//  AssetFileIO.h
//  World
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
#include "../World/GameState.h"

#include "../Control/Configuration.h"

using namespace std ;

enum class AssetType {
	asteroid,
	block,
	character,
	misc,
    ship,
	UI,
	weapon
	//what else do we need?
	//add more here
} ;

/**
 * This class will store the names and directory info of all file assets used in the program. 
 * Add the names of any new files added to AssetFileIO.cpp
 */
class AssetFileIO {
	
	static vector<std::string> * asteroidImageFilenames ;
	static vector<std::string> * blockImageFilenames ;
	static vector<std::string> * characterImageFilenames ;
	static vector<std::string> * miscImageFilenames ;
	static vector<string> * shipImageFilenames ;
	static vector<string> * UIImageFilenames ;
	static vector<string> * weaponImageFilenames ;
	
public:
	static SDL_Texture * getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type) ;
	static string & getImageFilename(vector<string>::size_type index, AssetType type) ;
	static string & getRandomImageFilename(AssetType type) ;
	
	/**
	 * In addition to finding the AssetType corresponding to the given string,
	 * getAssetTypeFrom() also serves to check that the string, which
	 * will presumably be used to initialize a GameObject or other in-game actor's
	 * texture, is in fact valid. It will throw an exception if it is not.
	 *
	 * @return The AssetType corresponding to imageFilename
	 */
	static AssetType getAssetTypeFromImageFile(const string & imageFilename) ;
};











#endif /* defined(__GameWorld__AssetFileIO__) */

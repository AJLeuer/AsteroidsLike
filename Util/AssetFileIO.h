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
#include "../World/CharacterData.h"
#include "../World/GameState.h"

#include "../Control/Configuration.h"

using namespace std ;

enum class AssetType {
	asteroid,
	misc,
	powerup = misc,
    playerShip,
	enemyShip,
	UI,
	weapon
	//what else do we need?
	//add more here
} ;

struct AssetFile {
	string fileName ;
	string filePath ;
	AssetType type ;
	Color color ;
	//other fields?
} ;

/**
 * This class will store the names and directory info of all file assets used in the program. 
 * Add the names of any new files added to AssetFileIO.cpp
 */
class AssetFileIO {
	
	static vector<AssetFile> * asteroidImageFiles ;
	static vector<AssetFile> * miscImageFilenames ;
	static vector<AssetFile> * playerShipImageFilenames ;
	static vector<AssetFile> * enemyShipImageFilenames ;
	static vector<AssetFile> * UIImageFilenames ;
	static vector<AssetFile> * weaponImageFilenames ;
	
public:
	static SDL_Texture * getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type) ;
	static string & getImageFilename(vector<AssetFile>::size_type index, AssetType type) ;
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

//
//  AssetFileIO.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/27/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "AssetFileIO.h"

using namespace std ;

vector<std::string> * AssetFileIO::characterImageFilenames = new vector<string> {
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_001.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_002.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_003.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_004.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_005.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_006.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_02_007.png"
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_001.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_002.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_003.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_004.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_005.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_006.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_02_007.png", } ;

vector<std::string> * AssetFileIO::blockImageFilenames = new vector<string> {
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_001.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_002.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_003.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_004.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_005.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_006.png",
																					"/Assets/Blocks/Blocks_01_64x64_Alt_03_007.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_001.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_002.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_003.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_004.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_005.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_006.png",
																					"/Assets/Blocks/Blocks_01_256x256_Alt_03_007.png"  } ;



vector<std::string> * AssetFileIO::shipImageFilenames = new vector<string> {
																					"/Assets/Ships/Ship 1.tiff",
																					"/Assets/Ships/Ship 2.tiff"  } ;


SDL_Texture * AssetFileIO::getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type)  {
	bool noMatch = true ;
	switch (type) {
		case AssetType::character:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < characterImageFilenames->size() ; i++) {
				if (str == characterImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::block:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < blockImageFilenames->size() ; i++) {
				if (str == blockImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::ship:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < shipImageFilenames->size() ; i++) {
				if (str == shipImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
	}
	//throw an except if it's not
	if (noMatch) {
		DebugOutput << "No file matching given filename \n" ;
		throw exception() ;
	}
	//otherwise, return the requested surface
	SDL_Texture * img = IMG_LoadTexture(renderer, str.c_str()) ;
	
	{
		/* debug code */
		#ifdef DEBUG_MODE
		stringstream ss ;
		ss << "Checking for IMG or SDL errors after IMG_LoadTexture(): " << IMG_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		#endif
		/* end debug code */
	}
	
	return img ;
}


/* Add other vectors of filename strings here */
string & AssetFileIO::getImageFilename(vector<string>::size_type index, AssetType type) {
	switch (type) {
		case AssetType::character:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, characterImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return characterImageFilenames->at(index) ;
		}
		case AssetType::block:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, blockImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return blockImageFilenames->at(index) ;
		}
		case AssetType::ship:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, shipImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return shipImageFilenames->at(index) ;
		}
	}
}


std::string & AssetFileIO::getRandomImageFilename(AssetType type) {
    FastRand<vector<string>::size_type> rnd(1000, 1000) ; //not used
	switch (type) {
		case AssetType::character:
		{
			rnd = FastRand<vector<string>::size_type>(0, characterImageFilenames->size() -1) ;
			return characterImageFilenames->at(rnd()) ;
		}
		case AssetType::block:
		{
			rnd = FastRand<vector<string>::size_type>(0, blockImageFilenames->size() -1) ;
			return blockImageFilenames->at(rnd()) ;
		}
		case AssetType::ship:
		{
			rnd = FastRand<vector<string>::size_type>(0, shipImageFilenames->size() -1) ;
			return shipImageFilenames->at(rnd()) ;
		}
	}
    return getImageFilename(rnd(), type) ;
}

AssetType AssetFileIO::getAssetTypeFrom(const string & imageFilename) {
	/* Try characters */
	for (auto i = 0 ; i < characterImageFilenames->size() ; i++) {
		if (imageFilename == characterImageFilenames->at(i)) {
			return AssetType::character ;
		}
	}
	/* Nothing found, so try blocks */
	for (auto i = 0 ; i < blockImageFilenames->size() ; i++) {
		if (imageFilename == blockImageFilenames->at(i)) {
			return AssetType::block ;
		}
	}
	/* try ships */
	for (auto i = 0 ; i < shipImageFilenames->size() ; i++) {
		if (imageFilename == shipImageFilenames->at(i)) {
			return AssetType::ship ;
		}
	}
	/* try any other added asset types here */
	
	/* couldn't find anything, throwing an exception */
	DebugOutput << "No corresponding file found for the given file name \n" ;
	throw new exception() ;
}



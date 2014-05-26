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
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_001.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_002.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_003.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_004.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_005.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_006.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_01_007.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_001.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_002.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_003.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_004.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_005.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_006.png",
																			"/Assets/Blocks/Blocks_01_64x64_Alt_02_007.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_001.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_002.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_003.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_004.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_005.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_006.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_01_007.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_001.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_002.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_003.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_004.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_005.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_006.png",
																			"/Assets/Blocks/Blocks_01_256x256_Alt_02_007.png"
																			} ;


SDL_Texture * AssetFileIO::getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type)  {
	switch (type) {
		case AssetType::character:
		{
			bool noMatch = true ;
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < characterImageFilenames->size() ; i++) {
				if (str == characterImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			//throw an except if it's not
			if (noMatch) {
				cout << "No file matching given filename" << endl ;
				throw exception() ;
			}
			//otherwise, return the requested surface
			SDL_Texture * img = IMG_LoadTexture(renderer, str.c_str()) ;

			/* debug code */
			stringstream ss ;
			ss << "Checking for IMG or SDL errors after IMG_LoadTexture(): " << IMG_GetError() << '\n' ;
			DebugOutput << ss.rdbuf() ;
			/* end debug code */

			return img ;
		}
		/*
		case AssetType::somethingElse?:
		{
			//todo
		}
		*/
	}
}


/* Add other vectors of filename strings here */
string & AssetFileIO::getImageFilename(vector<string>::size_type index, AssetType type) {
	switch (type) {
		case AssetType::character:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, characterImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return characterImageFilenames->at(index) ;
			break ;
		}
		/*
		case AssetType::somethingElse?:
		{
			//todo
		}
		*/
	}
}


std::string & AssetFileIO::getRandomImageFilename(AssetType type) {
	switch (type) {
		case AssetType::character:
		{
			FastRand<vector<string>::size_type> rnd(0, characterImageFilenames->size() -1) ;
			return getImageFilename(rnd(), AssetType::character) ;
		}
		/*
		case AssetType::scenery:
		{
			//todo
		}
		*/
	}
}



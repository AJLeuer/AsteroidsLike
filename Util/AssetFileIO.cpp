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


SDL_Surface * AssetFileIO::getSurfaceFromFilename(ImageType type, const string & str)  {
	switch (type) {
		case ImageType::character:
		{
			bool noMatch = true ;
			for (auto i = 0 ; i < characterImageFilenames->size() ; i++) {
				if (str == characterImageFilenames->at(i)) {
					noMatch = false ;
				}
			}
			if (noMatch) {
				cout << "No file matching given filename" << endl ;
				throw exception() ;
			}
			return IMG_Load(str.c_str()) ;
		}
		case ImageType::scenery:
		{
			//todo
			/* temp code */
			return new SDL_Surface ;
		}
	}
}


/* Add other vectors of filename strings here */
string & AssetFileIO::getImageFilename(ImageType type, vector<string>::size_type index) {
	index = (vector<string>::size_type)index ;
	switch (type) {
		case ImageType::character:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, characterImageFilenames->size() -1 /*y vals don't matter*/, 0, 0) ;
			bc.checkAgainst(&index) ;
			return characterImageFilenames->at(index) ;
			break ;
		}
		case ImageType::scenery:
		{
			//todo
			/* get string from scenery filename vector */
			string * s = new string("") ; //placeholder code
			return *s ;
		}
			
	}
}


std::string & AssetFileIO::getRandomImageFilename(ImageType type) {
	switch (type) {
		case ImageType::character:
		{
			fastRand<vector<string>::size_type> rnd(0, characterImageFilenames->size() -1) ;
			return getImageFilename(ImageType::character, rnd()) ;
			/* break ; */
		}
		case ImageType::scenery:
		{
			//todo
			/* temp code */
			string * s = new string("") ; //placeholder code
			return *s ;
		}
	}
}



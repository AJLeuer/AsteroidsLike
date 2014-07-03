//
//  AssetFileIO.cpp
//  World
//
//  Created by Adam James Leuer on 4/27/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "AssetFileIO.h"

using namespace std ;

vector<std::string> * AssetFileIO::asteroidImageFilenames = new vector<string> {
																					"/Assets/Asteroids/Asteroid0_Brown.png",
																					"/Assets/Asteroids/Asteroid0_Gray.png"   } ;


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


vector<std::string> * AssetFileIO::miscImageFilenames = new vector<string> {
																					"/Assets/Misc/Ship0_Damage0.png",
																					"/Assets/Misc/Ship0_Damage1.png",
																					"/Assets/Misc/Ship0_Damage2.png",
																					"/Assets/Misc/Ship1_Damage0.png",
																					"/Assets/Misc/Ship1_Damage1.png",
																					"/Assets/Misc/Ship1_Damage2.png",
																					"/Assets/Misc/Ship2_Damage0.png",
																					"/Assets/Misc/Ship2_Damage1.png",
																					"/Assets/Misc/Ship2_Damage2.png",
																					"/Assets/Misc/EngineThrust0.png",
																					"/Assets/Misc/Bolt_Gold.png",
																					"/Assets/Misc/Pill_Blue.png",
																					"/Assets/Misc/Pill_Green.png",
																					"/Assets/Misc/Shield_Silver.png"   } ;

vector<std::string> * AssetFileIO::playerShipImageFilenames = new vector<string> {
																					"/Assets/Ships/Saucer_Red.png",
																					"/Assets/Ships/Ship0_Blue.png",
																					"/Assets/Ships/Ship0_Red.png",
																					"/Assets/Ships/Ship1_Green.png",
																					"/Assets/Ships/Ship2_Blue.png"     } ;


vector<std::string> * AssetFileIO::enemyShipImageFilenames = new vector<string> {
																					"/Assets/Ships/Enemy_Ship0_Blue.png",
																					"/Assets/Ships/Enemy_Ship0_Orange.png",
																					"/Assets/Ships/Enemy_Ship2_Blue.png",
																					"/Assets/Ships/Enemy_Ship2_Green.png", } ;

vector<string> * AssetFileIO::UIImageFilenames = new vector<string> {
	
																					"/Assets/UI/Button_Blue.png",
																					"/Assets/UI/Button_Green.png",
																					"/Assets/UI/Button_Purple.png",
																					"/Assets/UI/Button_Red.png"      } ;

vector<string> * AssetFileIO::weaponImageFilenames = new vector<string> {
																					"/Assets/Weapons/Explosion0_Blue.png",
																					"/Assets/Weapons/Explosion0_Green.png",
																					"/Assets/Weapons/Explosion0_Red.png",
																					"/Assets/Weapons/LaserBlast0_Green.png",
																					"/Assets/Weapons/LaserBlast0_Red.png",
																					"/Assets/Weapons/LaserBlast1_Blue.png",
																					"/Assets/Weapons/LaserBlast1_Green.png",
																					"/Assets/Weapons/LaserBlast2_Blue.png",
																					"/Assets/Weapons/LaserBlast2_Red.png",
																					"/Assets/Weapons/MuzzleFlare0_Blue.png",
																					"/Assets/Weapons/Shield_Hi.png",
																					"/Assets/Weapons/Shield_Low.png",
																					"/Assets/Weapons/Shield_Med.png"   } ;



SDL_Texture * AssetFileIO::getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type)  {
	bool noMatch = true ;
	switch (type) {
		case AssetType::asteroid:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < asteroidImageFilenames->size() ; i++) {
				if (str == asteroidImageFilenames->at(i)) {
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
		case AssetType::misc:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < miscImageFilenames->size() ; i++) {
				if (str == miscImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::playerShip:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < playerShipImageFilenames->size() ; i++) {
				if (str == playerShipImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::enemyShip:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < enemyShipImageFilenames->size() ; i++) {
				if (str == enemyShipImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::UI:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < UIImageFilenames->size() ; i++) {
				if (str == UIImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::weapon:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < weaponImageFilenames->size() ; i++) {
				if (str == weaponImageFilenames->at(i)) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
	}
	//throw an except if it's not
	if (noMatch) {
		cerr << "No file matching given filename \n" ;
		throw exception() ;
	}
	//otherwise, return the requested surface
	SDL_Texture * img = IMG_LoadTexture(renderer, str.c_str()) ;
	
	{
		/* debug code */
		stringstream ss ;
		ss << "Checking for IMG or SDL errors after IMG_LoadTexture(): " << IMG_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* end debug code */
	}
	
	return img ;
}


/* Add other vectors of filename strings here */
string & AssetFileIO::getImageFilename(vector<string>::size_type index, AssetType type) {
	switch (type) {
		case AssetType::asteroid:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, asteroidImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return asteroidImageFilenames->at(index) ;
			break ;
		}
		case AssetType::block:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, blockImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return blockImageFilenames->at(index) ;
			break ;
		}
		case AssetType::misc:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, miscImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return miscImageFilenames->at(index) ;
			break ;
		}
		case AssetType::playerShip:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, playerShipImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return playerShipImageFilenames->at(index) ;
			break ;
		}
		case AssetType::enemyShip:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, enemyShipImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return enemyShipImageFilenames->at(index) ;
			break ;
		}
		case AssetType::UI:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, UIImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return UIImageFilenames->at(index) ;
			break ;
		}
		case AssetType::weapon:
		{
			BoundsCheck<vector<string>::size_type> bc = BoundsCheck<vector<string>::size_type>(0, weaponImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return weaponImageFilenames->at(index) ;
			break ;
		}
	}
}


std::string & AssetFileIO::getRandomImageFilename(AssetType type) {
    FastRand<vector<string>::size_type> rnd(0, 1000) ; //not used
	switch (type) {
		case AssetType::asteroid:
		{
			rnd = FastRand<vector<string>::size_type>(0, asteroidImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::block:
		{
			rnd = FastRand<vector<string>::size_type>(0, blockImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::misc:
		{
			rnd = FastRand<vector<string>::size_type>(0, miscImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::playerShip:
		{
			rnd = FastRand<vector<string>::size_type>(0, playerShipImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::enemyShip:
		{
			rnd = FastRand<vector<string>::size_type>(0, enemyShipImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::UI:
		{
			rnd = FastRand<vector<string>::size_type>(0, UIImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::weapon:
		{
			rnd = FastRand<vector<string>::size_type>(0, weaponImageFilenames->size() -1) ;
			break ;
		}
	}
    return getImageFilename(rnd(), type) ;
}

AssetType AssetFileIO::getAssetTypeFromImageFile(const string & imageFilename) {
	for (auto i = 0 ; i < asteroidImageFilenames->size() ; i++) {
		if (imageFilename == asteroidImageFilenames->at(i)) {
			return AssetType::asteroid ;
		}
	}
	for (auto i = 0 ; i < blockImageFilenames->size() ; i++) {
		if (imageFilename == blockImageFilenames->at(i)) {
			return AssetType::block ;
		}
	}
	for (auto i = 0 ; i < miscImageFilenames->size() ; i++) {
		if (imageFilename == miscImageFilenames->at(i)) {
			return AssetType::misc ;
		}
	}
	for (auto i = 0 ; i < playerShipImageFilenames->size() ; i++) {
		if (imageFilename == playerShipImageFilenames->at(i)) {
			return AssetType::playerShip ;
		}
	}
	for (auto i = 0 ; i < enemyShipImageFilenames->size() ; i++) {
		if (imageFilename == enemyShipImageFilenames->at(i)) {
			return AssetType::enemyShip ;
		}
	}
	for (auto i = 0 ; i < UIImageFilenames->size() ; i++) {
		if (imageFilename == UIImageFilenames->at(i)) {
			return AssetType::UI ;
		}
	}
	for (auto i = 0 ; i < weaponImageFilenames->size() ; i++) {
		if (imageFilename == weaponImageFilenames->at(i)) {
			return AssetType::weapon ;
		}
	}
	
	/* couldn't find anything, throwing an exception */
	cerr << "No corresponding file found for the given file name \n" ;
	throw exception() ;
}



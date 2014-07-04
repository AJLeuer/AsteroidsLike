//
//  AssetFileIO.cpp
//  World
//
//  Created by Adam James Leuer on 4/27/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "AssetFileIO.h"

using namespace std ;

vector<AssetFile> * AssetFileIO::asteroidImageFiles = new vector<AssetFile> {
	{"Asteroid0_Brown.png", "/Assets/Asteroids/Asteroid0_Brown.png", AssetType::asteroid, Color::brown},
	{"Asteroid0_Gray.png",  "/Assets/Asteroids/Asteroid0_Gray.png",	 AssetType::asteroid, Color::brown}
} ;





vector<AssetFile> * AssetFileIO::miscImageFilenames = new vector<AssetFile> {
	{"Ship0_Damage0.png", "/Assets/Misc/Ship0_Damage0.png", AssetType::misc, Color::transparent},
	{"Ship0_Damage1.png", "/Assets/Misc/Ship0_Damage1.png", AssetType::misc, Color::transparent},
	{"Ship0_Damage2.png", "/Assets/Misc/Ship0_Damage2.png", AssetType::misc, Color::transparent},
	{"Ship1_Damage1.png", "/Assets/Misc/Ship1_Damage1.png", AssetType::misc, Color::transparent},
	{"Ship1_Damage2.png", "/Assets/Misc/Ship1_Damage2.png", AssetType::misc, Color::transparent},
	{"Ship2_Damage0.png", "/Assets/Misc/Ship2_Damage0.png", AssetType::misc, Color::transparent},
	{"Ship2_Damage1.png", "/Assets/Misc/Ship2_Damage1.png", AssetType::misc, Color::transparent},
	{"Ship2_Damage2.png", "/Assets/Misc/Ship2_Damage2.png", AssetType::misc, Color::transparent},
	{"EngineThrust0.png", "/Assets/Misc/EngineThrust0.png", AssetType::misc, Color::transparent},
	{"Bolt_Gold.png",     "/Assets/Misc/Bolt_Gold.png",     AssetType::powerup, Color::gold},
	{"Pill_Blue.png",     "/Assets/Misc/Pill_Blue.png",		AssetType::powerup, Color::blue},
	{"Pill_Green.png",    "/Assets/Misc/Pill_Green.png",	AssetType::powerup, Color::blue},
	{"Shield_Silver.png", "/Assets/Misc/Shield_Silver.png", AssetType::powerup, Color::silver}
} ;

vector<AssetFile> * AssetFileIO::playerShipImageFilenames = new vector<AssetFile> {
	{"Saucer_Red.png",  "/Assets/Ships/Saucer_Red.png",  AssetType::playerShip, Color::red},
	{"Ship0_Blue.png",  "/Assets/Ships/Ship0_Blue.png",  AssetType::playerShip, Color::blue},
	{"Ship0_Red.png",   "/Assets/Ships/Ship0_Red.png",	 AssetType::playerShip, Color::red},
	{"Ship1_Green.png", "/Assets/Ships/Ship1_Green.png", AssetType::playerShip, Color::green},
	{"Ship2_Blue.png",  "/Assets/Ships/Ship2_Blue.png",	 AssetType::playerShip, Color::blue}

} ;


vector<AssetFile> * AssetFileIO::enemyShipImageFilenames = new vector<AssetFile> {
	{"Enemy_Ship0_Blue.png",   "/Assets/Ships/Enemy_Ship0_Blue.png",	AssetType::enemyShip, Color::blue},
	{"Enemy_Ship0_Orange.png", "/Assets/Ships/Enemy_Ship0_Orange.png",  AssetType::enemyShip, Color::orange},
	{"Enemy_Ship2_Blue.png",   "/Assets/Ships/Enemy_Ship2_Blue.png",	AssetType::enemyShip, Color::blue},
	{"Enemy_Ship2_Green.png",  "/Assets/Ships/Enemy_Ship2_Green.png",	AssetType::enemyShip, Color::green}
} ;

vector<AssetFile> * AssetFileIO::UIImageFilenames = new vector<AssetFile> {
	{"Button_Blue.png",		"/Assets/UI/Button_Blue.png",	AssetType::UI, Color::blue},
	{"Button_Green.png",	"/Assets/UI/Button_Green.png",	AssetType::UI, Color::green},
	{"Button_Purple.png",	"/Assets/UI/Button_Purple.png",	AssetType::UI, Color::purple},
	{"Button_Red.png",		"/Assets/UI/Button_Red.png",	AssetType::UI, Color::red}
} ;

vector<AssetFile> * AssetFileIO::weaponImageFilenames = new vector<AssetFile> {
	{"Explosion0_Blue.png",		"/Assets/Weapons/Explosion0_Blue.png",		AssetType::weapon, Color::blue},
	{"Explosion0_Green.png",	"/Assets/Weapons/Explosion0_Green.png",		AssetType::weapon, Color::green},
	{"Explosion0_Red.png",		"/Assets/Weapons/Explosion0_Red.png",		AssetType::weapon, Color::red},
	{"LaserBlast0_Green.png",   "/Assets/Weapons/LaserBlast0_Green.png",	AssetType::weapon, Color::green},
	{"LaserBlast0_Red.png",		"/Assets/Weapons/LaserBlast0_Red.png",		AssetType::weapon, Color::red},
	{"LaserBlast1_Blue.png",	"/Assets/Weapons/LaserBlast1_Blue.png",		AssetType::weapon, Color::blue},
	{"LaserBlast1_Green.png",	"/Assets/Weapons/LaserBlast1_Green.png",	AssetType::weapon, Color::green},
	{"LaserBlast2_Blue.png",	"/Assets/Weapons/LaserBlast2_Blue.png",		AssetType::weapon, Color::blue},
	{"LaserBlast2_Red.png",		"/Assets/Weapons/LaserBlast2_Red.png",		AssetType::weapon, Color::red},
	{"MuzzleFlare0_Blue.png",	"/Assets/Weapons/MuzzleFlare0_Blue.png",	AssetType::weapon, Color::blue},
	{"Shield_Hi.png",			"/Assets/Weapons/Shield_Hi.png",			AssetType::weapon, Color::transparent},
	{"Shield_Low.png",			"/Assets/Weapons/Shield_Low.png",			AssetType::weapon, Color::transparent},
	{"Shield_Med.png",			"/Assets/Weapons/Shield_Med.png",			AssetType::weapon, Color::transparent}
} ;



SDL_Texture * AssetFileIO::getTextureFromFilename(SDL_Renderer * renderer, const string & str, AssetType type)  {
	bool noMatch = true ;
	switch (type) {
		case AssetType::asteroid:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < asteroidImageFiles->size() ; i++) {
				if (str == asteroidImageFiles->at(i).fileName) {
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
				if (str == miscImageFilenames->at(i).fileName) {
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
				if (str == playerShipImageFilenames->at(i).fileName) {
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
				if (str == enemyShipImageFilenames->at(i).fileName) {
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
				if (str == UIImageFilenames->at(i).fileName) {
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
				if (str == weaponImageFilenames->at(i).fileName) {
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
string & AssetFileIO::getImageFilename(vector<AssetFile>::size_type index, AssetType type) {
	switch (type) {
		case AssetType::asteroid:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, asteroidImageFiles->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return asteroidImageFiles->at(index).fileName ;
			break ;
		}
		case AssetType::misc:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, miscImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return miscImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::playerShip:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, playerShipImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return playerShipImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::enemyShip:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, enemyShipImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return enemyShipImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::UI:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, UIImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return UIImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::weapon:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, weaponImageFilenames->size()-1, /*rest of vals don't matter*/ 0, 0) ;
			bc.checkAgainst(&index) ;
			return weaponImageFilenames->at(index).fileName ;
			break ;
		}
	}
}


std::string & AssetFileIO::getRandomImageFilename(AssetType type) {
    FastRand<vector<AssetFile>::size_type> rnd(0, 1000) ; //not used
	switch (type) {
		case AssetType::asteroid:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, asteroidImageFiles->size() -1) ;
			break ;
		}
		case AssetType::misc:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, miscImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::playerShip:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, playerShipImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::enemyShip:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, enemyShipImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::UI:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, UIImageFilenames->size() -1) ;
			break ;
		}
		case AssetType::weapon:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, weaponImageFilenames->size() -1) ;
			break ;
		}
	}
    return getImageFilename(rnd(), type) ;
}

AssetType AssetFileIO::getAssetTypeFromImageFile(const string & imageFilename) {
	for (auto i = 0 ; i < asteroidImageFiles->size() ; i++) {
		if (imageFilename == asteroidImageFiles->at(i).fileName) {
			return AssetType::asteroid ;
		}
	}
	for (auto i = 0 ; i < miscImageFilenames->size() ; i++) {
		if (imageFilename == miscImageFilenames->at(i).fileName) {
			return AssetType::misc ;
		}
	}
	for (auto i = 0 ; i < playerShipImageFilenames->size() ; i++) {
		if (imageFilename == playerShipImageFilenames->at(i).fileName) {
			return AssetType::playerShip ;
		}
	}
	for (auto i = 0 ; i < enemyShipImageFilenames->size() ; i++) {
		if (imageFilename == enemyShipImageFilenames->at(i).fileName) {
			return AssetType::enemyShip ;
		}
	}
	for (auto i = 0 ; i < UIImageFilenames->size() ; i++) {
		if (imageFilename == UIImageFilenames->at(i).fileName) {
			return AssetType::UI ;
		}
	}
	for (auto i = 0 ; i < weaponImageFilenames->size() ; i++) {
		if (imageFilename == weaponImageFilenames->at(i).fileName) {
			return AssetType::weapon ;
		}
	}
	
	/* couldn't find anything, throwing an exception */
	cerr << "No corresponding file found for the given file name \n" ;
	throw exception() ;
}



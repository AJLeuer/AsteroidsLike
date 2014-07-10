//
//  AssetFileIO.cpp
//  World
//
//  Created by Adam James Leuer on 4/27/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "AssetFileIO.h"

using namespace std ;

vector<AssetFile> * AssetFile::asteroidImageFiles = new vector<AssetFile> {
	{"Asteroid0_Brown.png", "/Assets/Asteroids/Asteroid0_Brown.png", AssetType::asteroid, Colors::brown},
	{"Asteroid0_Gray.png",  "/Assets/Asteroids/Asteroid0_Gray.png",	 AssetType::asteroid, Colors::brown}
} ;

vector<AssetFile> * AssetFile::miscImageFilenames = new vector<AssetFile> {
	{"Ship0_Damage0.png", "/Assets/Misc/Ship0_Damage0.png", AssetType::misc, Colors::transparent},
	{"Ship0_Damage1.png", "/Assets/Misc/Ship0_Damage1.png", AssetType::misc, Colors::transparent},
	{"Ship0_Damage2.png", "/Assets/Misc/Ship0_Damage2.png", AssetType::misc, Colors::transparent},
	{"Ship1_Damage1.png", "/Assets/Misc/Ship1_Damage1.png", AssetType::misc, Colors::transparent},
	{"Ship1_Damage2.png", "/Assets/Misc/Ship1_Damage2.png", AssetType::misc, Colors::transparent},
	{"Ship2_Damage0.png", "/Assets/Misc/Ship2_Damage0.png", AssetType::misc, Colors::transparent},
	{"Ship2_Damage1.png", "/Assets/Misc/Ship2_Damage1.png", AssetType::misc, Colors::transparent},
	{"Ship2_Damage2.png", "/Assets/Misc/Ship2_Damage2.png", AssetType::misc, Colors::transparent},
	{"EngineThrust0.png", "/Assets/Misc/EngineThrust0.png", AssetType::misc, Colors::transparent},
	{"Bolt_Gold.png",     "/Assets/Misc/Bolt_Gold.png",     AssetType::powerup, Colors::gold},
	{"Pill_Blue.png",     "/Assets/Misc/Pill_Blue.png",		AssetType::powerup, Colors::blue},
	{"Pill_Green.png",    "/Assets/Misc/Pill_Green.png",	AssetType::powerup, Colors::blue},
	{"Shield_Silver.png", "/Assets/Misc/Shield_Silver.png", AssetType::powerup, Colors::silver}
} ;

vector<AssetFile> * AssetFile::playerShipImageFilenames = new vector<AssetFile> {
	{"Saucer_Red.png",  "/Assets/Ships/Saucer_Red.png",  AssetType::playerShip, Colors::red},
	{"Ship0_Blue.png",  "/Assets/Ships/Ship0_Blue.png",  AssetType::playerShip, Colors::blue},
	{"Ship0_Red.png",   "/Assets/Ships/Ship0_Red.png",	 AssetType::playerShip, Colors::red},
	{"Ship1_Green.png", "/Assets/Ships/Ship1_Green.png", AssetType::playerShip, Colors::green},
	{"Ship2_Blue.png",  "/Assets/Ships/Ship2_Blue.png",	 AssetType::playerShip, Colors::blue}

} ;


vector<AssetFile> * AssetFile::enemyShipImageFilenames = new vector<AssetFile> {
	{"Enemy_Ship0_Blue.png",   "/Assets/Ships/Enemy_Ship0_Blue.png",	AssetType::enemyShip, Colors::blue},
	{"Enemy_Ship0_Orange.png", "/Assets/Ships/Enemy_Ship0_Orange.png",  AssetType::enemyShip, Colors::orange},
	{"Enemy_Ship2_Blue.png",   "/Assets/Ships/Enemy_Ship2_Blue.png",	AssetType::enemyShip, Colors::blue},
	{"Enemy_Ship2_Green.png",  "/Assets/Ships/Enemy_Ship2_Green.png",	AssetType::enemyShip, Colors::green}
} ;

vector<AssetFile> * AssetFile::UIImageFilenames = new vector<AssetFile> {
	{"Button_Blue.png",		"/Assets/UI/Button_Blue.png",	AssetType::UI, Colors::blue},
	{"Button_Green.png",	"/Assets/UI/Button_Green.png",	AssetType::UI, Colors::green},
	{"Button_Purple.png",	"/Assets/UI/Button_Purple.png",	AssetType::UI, Colors::purple},
	{"Button_Red.png",		"/Assets/UI/Button_Red.png",	AssetType::UI, Colors::red}
} ;

vector<AssetFile> * AssetFile::weaponImageFilenames = new vector<AssetFile> {
	{"Explosion0_Blue.png",		"/Assets/Weapons/Explosion0_Blue.png",		AssetType::weapon, Colors::blue},
	{"Explosion0_Green.png",	"/Assets/Weapons/Explosion0_Green.png",		AssetType::weapon, Colors::green},
	{"Explosion0_Red.png",		"/Assets/Weapons/Explosion0_Red.png",		AssetType::weapon, Colors::red},
	{"LaserBlast0_Green.png",   "/Assets/Weapons/LaserBlast0_Green.png",	AssetType::weapon, Colors::green},
	{"LaserBlast0_Red.png",		"/Assets/Weapons/LaserBlast0_Red.png",		AssetType::weapon, Colors::red},
	{"LaserBlast1_Blue.png",	"/Assets/Weapons/LaserBlast1_Blue.png",		AssetType::weapon, Colors::blue},
	{"LaserBlast1_Green.png",	"/Assets/Weapons/LaserBlast1_Green.png",	AssetType::weapon, Colors::green},
	{"LaserBlast2_Blue.png",	"/Assets/Weapons/LaserBlast2_Blue.png",		AssetType::weapon, Colors::blue},
	{"LaserBlast2_Red.png",		"/Assets/Weapons/LaserBlast2_Red.png",		AssetType::weapon, Colors::red},
	{"MuzzleFlare0_Blue.png",	"/Assets/Weapons/MuzzleFlare0_Blue.png",	AssetType::weapon, Colors::blue},
	{"Shield_Hi.png",			"/Assets/Weapons/Shield_Hi.png",			AssetType::weapon, Colors::transparent},
	{"Shield_Low.png",			"/Assets/Weapons/Shield_Low.png",			AssetType::weapon, Colors::transparent},
	{"Shield_Med.png",			"/Assets/Weapons/Shield_Med.png",			AssetType::weapon, Colors::transparent}
} ;

vector< vector<AssetFile> * > * AssetFile::allAssetFiles = new vector< vector<AssetFile> * > {
	asteroidImageFiles ,
	miscImageFilenames ,
	playerShipImageFilenames,
	enemyShipImageFilenames,
	UIImageFilenames,
	weaponImageFilenames
	/* Add any more here */
} ;

AssetFile::AssetFile(const string & existingFilename) {
	
	bool found = false ;
	for (auto i = 0 ; i < allAssetFiles->size() ; i++) {
		for (auto j = 0 ; j < allAssetFiles->at(i)->size() ; j++) {
			if (allAssetFiles->at(i)->at(j).fileName == existingFilename) {
				*this = allAssetFiles->at(i)->at(j) ;
				found = true ;
			}
		}
	}
	if (!found) {
		cerr << "No such file found" << endl ;
		throw exception() ;
	}
}

AssetFile::AssetFile(FastRand<int> randm) {
    unsigned i = (unsigned)randm(0, (allAssetFiles->size() - 1)) ;
    unsigned j = (unsigned)randm(0, (allAssetFiles->at(i)->size() - 1)) ;
    
    AssetFile * file = &(allAssetFiles->at(i)->at(j)) ;
    
    *this = *file ;
}

AssetFile & AssetFile::operator = (const AssetFile & rhs) {
	if (this != &rhs) {
		this->fileName = rhs.fileName ;
		this->filePath = rhs.filePath ;
		this->type = rhs.type ;
		this->color = rhs.color ;
	}
	return *this ;
}


AssetFile & AssetFile::operator = (const string & str) {
	if (this->fileName != str) {
		*this = AssetFile(str) ;
	}
	return *this ;
}

Texture * AssetFileIO::getTextureFromFilename(Renderer * renderer, const AssetFile & file, AssetType type)  {
	bool noMatch = true ;
	switch (type) {
		case AssetType::asteroid:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < AssetFile::asteroidImageFiles->size() ; i++) {
				if (file.fileName == AssetFile::asteroidImageFiles->at(i).fileName) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::misc:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < AssetFile::miscImageFilenames->size() ; i++) {
				if (file.fileName == AssetFile::miscImageFilenames->at(i).fileName) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::playerShip:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < AssetFile::playerShipImageFilenames->size() ; i++) {
				if (file.fileName == AssetFile::playerShipImageFilenames->at(i).fileName) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::enemyShip:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < AssetFile::enemyShipImageFilenames->size() ; i++) {
				if (file.fileName == AssetFile::enemyShipImageFilenames->at(i).fileName) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::UI:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < AssetFile::UIImageFilenames->size() ; i++) {
				if (file.fileName == AssetFile::UIImageFilenames->at(i).fileName) {
					noMatch = false ;
					break ;
				}
			}
			break ;
		}
		case AssetType::weapon:
		{
			//check the string given to make sure it's a match
			for (auto i = 0 ; i < AssetFile::weaponImageFilenames->size() ; i++) {
				if (file.fileName == AssetFile::weaponImageFilenames->at(i).fileName) {
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
	Texture * img = IMG_LoadTexture(renderer, file.filePath.c_str()) ;
	
	{
		/* debug code */
		stringstream ss ;
		ss << "Checking for IMG or SDL errors after IMG_LoadTexture(): " << IMG_GetError() << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* end debug code */
	}
	
	return img ;
}


/* Add other vectors of filename strings here */ /*
string & AssetFileIO::getImageFilename(vector<AssetFile>::size_type index, AssetType type) {
	switch (type) {
		case AssetType::asteroid:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, AssetFile::asteroidImageFiles->size()-1, 0, 0) ;
			bc.checkAgainst(&index) ;
			return AssetFile::asteroidImageFiles->at(index).fileName ;
			break ;
		}
		case AssetType::misc:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, AssetFile::miscImageFilenames->size()-1, 0, 0) ;
			bc.checkAgainst(&index) ;
			return AssetFile::miscImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::playerShip:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, AssetFile::playerShipImageFilenames->size()-1, 0, 0) ;
			bc.checkAgainst(&index) ;
			return AssetFile::playerShipImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::enemyShip:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, AssetFile::enemyShipImageFilenames->size()-1, 0, 0) ;
			bc.checkAgainst(&index) ;
			return AssetFile::enemyShipImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::UI:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, AssetFile::UIImageFilenames->size()-1, 0, 0) ;
			bc.checkAgainst(&index) ;
			return AssetFile::UIImageFilenames->at(index).fileName ;
			break ;
		}
		case AssetType::weapon:
		{
			BoundsCheck<vector<AssetFile>::size_type> bc = BoundsCheck<vector<AssetFile>::size_type>(0, AssetFile::weaponImageFilenames->size()-1, 0, 0) ;
			bc.checkAgainst(&index) ;
			return AssetFile::weaponImageFilenames->at(index).fileName ;
			break ;
		}
	}
}
*/

AssetFile AssetFileIO::getRandomImageFile(AssetType type) {
	
    FastRand<vector<AssetFile>::size_type> rnd(0, 1000) ; //not used
	
	switch (type) {
		case AssetType::asteroid:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, AssetFile::asteroidImageFiles->size() -1) ;
			return AssetFile::asteroidImageFiles->at(rnd()) ;
		}
		case AssetType::misc:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, AssetFile::miscImageFilenames->size() -1) ;
			return AssetFile::miscImageFilenames->at(rnd()) ;
		}
		case AssetType::playerShip:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, AssetFile::playerShipImageFilenames->size() -1) ;
			return AssetFile::playerShipImageFilenames->at(rnd()) ;
		}
		case AssetType::enemyShip:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, AssetFile::enemyShipImageFilenames->size() -1) ;
			return AssetFile::enemyShipImageFilenames->at(rnd()) ;
		}
		case AssetType::UI:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, AssetFile::UIImageFilenames->size() -1) ;
			return AssetFile::UIImageFilenames->at(rnd()) ;
		}
		case AssetType::weapon:
		{
			rnd = FastRand<vector<AssetFile>::size_type>(0, AssetFile::weaponImageFilenames->size() -1) ;
			return AssetFile::weaponImageFilenames->at(rnd()) ;
		}
	}
}




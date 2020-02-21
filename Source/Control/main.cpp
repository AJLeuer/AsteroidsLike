#include <iostream>

#include "../Util/Util.hpp"
#include "../Util/AssetFileIO.h"
#include "../World/GameObject.h"

#include "MainControl.h"


int main(int argc, char ** argv) {

	GameState::currentDirectory = argv[0] ;
	cout << "the current directory is: " << GameState::currentDirectory << endl ;

	MainControl::init() ;
	MainControl::main() ;
     
	//MainControl calls its own exit()
    
	return 0 ;
}








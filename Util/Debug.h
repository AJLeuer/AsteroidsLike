//
//  Debug.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/6/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Debug__
#define __GameWorld__Debug__

#include <iostream>
#include <fstream>

#include "Location.h"

using namespace std ;

class Debug {
	
public:
	static ofstream * debugFile ;
	
	/*
	 * Draws a grid representing every object in the container. Draws an icon at every
	 * index an object is found. It calls the object's getIcon(), returning a std::string,
	 * and prints that at each index. It is recommended that icon consist of only a single char (though
	 * stored inside a string) in order to maintain proportions.
	 *
	 * Note: If the class stored in the container does not implement getIcon() returning a std::string, 
	 * the code will not compile.
	 *
	 * @param whiteSpace The character to be printed in spaces where there are no objects
	 * @param writeTo The outputstream (std::ostream) that will receive output
	 */
	template <typename Container>
	static void draw2DRepresentation(ostream & writeTo, Container * container, char whitespace) ;
	
	/* Use for whatever */
	static unsigned debugCounter ;
	
} ;



template<class Container>
void Debug::draw2DRepresentation(ostream & writeTo, Container * container, char whitespace) {
	
	for (auto i = 0 ; i < container->at(0)->size() ; i++) {
		for (auto j = 0 ; j < container->size() ; j++) {
			if (container->at(j)->at(i) != nullptr) {
				auto c = container->at(j)->at(i)->getIcon() ;
				writeTo << c ; /*->getIcon()*/  //remove *(     )
			}
			else {
				writeTo << whitespace ;
			}
		}
		writeTo << endl ;
	}
}
	

#endif /* defined(__GameWorld__Debug__) */

//
//  Util.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/20/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_Util_h
#define GameWorld_Util_h

template<typename T>
T randSignFlip(T n) ;

template<typename T>
T chooseAtRand(T n1, T n2) ;

template<typename T>
T randSignFlip(T n) {
	bool pos = rand() % 2 ;
	if (pos) {
		return n ;
	}
	else {
		return (n * -1) ;
	}
}

template <typename T>
T chooseAtRand(T n1, T n2) {
	bool first = rand() % 2 ;
	if (first) {
		return n1 ;
	}
	else {
		return n2 ;
	}
}


#endif

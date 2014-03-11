//
//  GameData.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_GameData_h
#define GameWorld_GameData_h

struct Damage {
	
public:
	
	unsigned long baseValue ;
	unsigned long modifier ;
	
	/**
	 * @return The total amount of damage
	 */
	unsigned long calculate() {
		return baseValue + modifier ;
	}
	
} ;

struct Health {
	
public:
	
	unsigned long baseValue ;
	unsigned long modifier ;
	
	/**
	 * @return The total amount of health
	 */
	unsigned long calculate() {
		return baseValue + modifier ;
	}
	
} ;

#endif

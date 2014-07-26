//
//  Enemy.h
//  World
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__Enemy__
#define __SpriteFight__Enemy__

#include <iostream>

#include "NPC.h"

class Enemy : public NPC {
  

public:
    
    
    Enemy() : NPC() {}
	
	Enemy(const AssetFile & imageFile, float size, const Position<float> & loc, const Angle<float> & rotation, string name, DoA alive,
		  CharacterState state, unsigned health, unsigned damage, bool monitorVelocity, Reaction reaction) :
	
		NPC(imageFile, size, loc, rotation, name, alive, state, health, damage, monitorVelocity, reaction) {}
    
    void defaultBehaviors() override ;
    
    
    
} ;

#endif /* defined(__SpriteFight__Enemy__) */

//
//  Enemy.h
//  World
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Enemy__
#define __GameWorld__Enemy__

#include <iostream>

#include "NPC.h"

class Enemy : public NPC {
  

public:
    
    
    Enemy() : NPC() {}
	
	Enemy(Colors color, AssetType type, const AssetFile & imageFile, float size, const Pos2<float> & loc, string name, DoA alive, CharacterState state, unsigned health, unsigned damage, Reaction reaction) :
		NPC(color, type, imageFile, size, loc, name, alive, state, health, damage, reaction) {}
    
    void defaultBehaviors() override ;
    
    
    
} ;

#endif /* defined(__GameWorld__Enemy__) */

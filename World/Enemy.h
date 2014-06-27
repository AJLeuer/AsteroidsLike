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

#include "GameObject.h"

class Enemy : public GameObject {
  

public:
    
    
    Enemy() : GameObject() {}
	
	Enemy(AssetType type, const string & imageFileName, float sizeModifier, const Pos2<float> & loc_) :
		GameObject(type, imageFileName, sizeModifier, loc_) {}
    
    void defaultBehaviors() override ;
    
    
    
} ;

#endif /* defined(__GameWorld__Enemy__) */

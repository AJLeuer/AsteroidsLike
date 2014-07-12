//
//  Projectile.h
//  SpriteFight
//
//  Created by Adam James Leuer on 7/12/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameObject.h"

#ifndef SpriteFight_Projectile
#define SpriteFight_Projectile

class Projectile : public GameObject {
	
protected:
	
	
public:
	
	Projectile(Colors color, const AssetFile & imageFile, float sizeModifier, const Pos2<float> & loc_) :
		GameObject(color, imageFile, sizeModifier, loc_)
	{
		visible = false ;
	}
	
	void fire() ;
};

















#endif
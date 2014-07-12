//
//  Weapon.h
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__Weapon__
#define __SpriteFight__Weapon__

#include <iostream>

#include "GameObject.h"
#include "Projectile.h"

#include "../Util/AssetFileIO.h"

class Weapon : public GameObject {
	
protected:
	
	Projectile projectile ;
	
public:
	
	Weapon(Colors color, const AssetFile & imageFile, float sizeModifier, const Pos2<float> & loc_,
		   const AssetFile & projectileImageFile) :
		GameObject(color, imageFile, sizeModifier, loc_),
		projectile(color, projectileImageFile, sizeModifier / 2, loc_) {}
	
	void fire() ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */

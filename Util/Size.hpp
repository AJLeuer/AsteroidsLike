//
//  Size.h
//  World
//
//  Created by Adam James Leuer on 5/13/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Size__
#define __GameWorld__Size__

#include <iostream>
#include <sstream>

#include "Position.hpp"

template<typename N>
struct Size : public Position<N> {

private:

	N * w = &(this->x) ; /* Never assign w or h, only ever assign to the values they point to (*w or *h) */
	N * h = &(this->y) ;
	N * z ; //don't use it

	/**
	 * The size modifier. Each GameObject will have a default size based on the sprite texture used to represent them, which will
	 * be multiplied by size. By default size will be 1, leaving the output unchanged. To adjust the size of this object, simply change the value of
	 * size (for instance, to make this half the normal size, set size = 0.5).
	 */
	float sizeModifier ;
	
	void setModifier(float modifier) { this->sizeModifier = modifier ; }
	void setWidth(N w) { *(this->w) = w ; }
	void setHeight(N h) { *(this->h) = h ; }

public:


	Size() :
		Position<N>(),
		sizeModifier(1.0) {}

	Size(N w_, N h_, float modifier) :
		Position<N>(w_, h_, 0),
		sizeModifier(modifier) {}
    
    Size(N w_, N h_) :
        Position<N>(w_, h_, 0),
        sizeModifier(1) {}

	Size(const Size<N> & other) :
		Position<N>(other),
		sizeModifier(other.sizeModifier) {}

	Size(Size<N> && other) :
		Position<N>(std::move(other)),
		sizeModifier(other.sizeModifier) {}

	~Size() {}

	Size & operator=(const Size<N> & rhs) {
		this->Position<N>::operator=(rhs) ;
		sizeModifier = rhs.sizeModifier ;
		return *this ;
	}
	
	Size & operator=(Size<N> && rhs) {
		this->Position<N>::operator=(std::move(rhs)) ;
		sizeModifier = rhs.sizeModifier ;
		return *this ;
	}
	
	void setSize(N w, N h, float modifier) {
		setWidth(w) ;
		setHeight(h) ;
		setModifier(modifier) ;
	}
	
	float getModifier() const { return sizeModifier ; }
	N getWidth() const { return (*(this->w) * sizeModifier) ; }
	N getHeight() const { return (*(this->h) * sizeModifier) ; }

} ;


#endif /* defined(__GameWorld__Size__) */

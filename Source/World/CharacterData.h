//
//  CharacterData.h
//  World
//
//  Created by Adam James Leuer on 4/29/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__CharacterData__
#define __SpriteFight__CharacterData__


#include <iostream>
#include <ostream>

#include "../Util/BoundsCheck.hpp"
#include "../Util/Random.hpp"

using namespace std ;

/**
 * A parent type that will define the basic functions and members for Damage and Health (and other similar data types
 * if we need them)
 */
struct CharacterData {
	
protected:
	
	unsigned long baseValue ;
	unsigned int modifier ;
	
public:
	
	CharacterData() :
		baseValue(0),
		modifier(1) {}
	
	CharacterData(unsigned long b) :
		baseValue(b),
		modifier(1) {}
	
	
	CharacterData(unsigned long b, unsigned m) :
		baseValue(b),
		modifier(m) {}
	
	
	CharacterData(const CharacterData & other) :
		baseValue(other.baseValue),
		modifier(other.modifier) {}
	
	CharacterData(CharacterData && other) :
		baseValue(other.baseValue),
		modifier(other.modifier) {}
	
	
	CharacterData & operator=(const CharacterData & rhs) {
		if (this != &rhs) {
			this->baseValue = rhs.baseValue ;
			this->modifier = rhs.modifier ;
		}
		return *this ;
	}
	
	CharacterData & operator=(CharacterData && rhs) {
		if (this != &rhs) {
			this->baseValue = rhs.baseValue ;
			this->modifier = rhs.modifier ;
		}
		return *this ;
	}
	
	bool operator==(CharacterData & rhs) {
		return (this->value() == rhs.value()) ;
	}
	
	bool operator!=(CharacterData & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	bool operator>(CharacterData & rhs) {
		return (this->value() > rhs.value()) ;
	}
	
	bool operator<(CharacterData & rhs) {
		return (this->value() < rhs.value()) ;
	}
	
	bool operator>=(CharacterData & rhs) {
		return (this->value() >= rhs.value()) ;
	}
	
	bool operator<=(CharacterData & rhs) {
		return (this->value() <= rhs.value()) ;
	}
	
	CharacterData & operator+(const CharacterData & rhs) {
		this->baseValue += rhs.baseValue ;
		this->modifier += rhs.modifier ;
		return *this ;
	}
	
	CharacterData & operator-(const CharacterData & rhs) {
		this->baseValue -= rhs.baseValue ;
		this->modifier -= rhs.modifier ;
		return *this ;
	}
	
	/**
	 * Override the << output stream operator
	 */
	inline
	friend ostream & operator<<(std::ostream & os, CharacterData & gmd) {
		os << gmd.value() ;
		return os ;
	}
	
	/**
	 * Override the << output stream operator
	 */
	inline
	friend ostream & operator<<(std::ostream & os, const CharacterData & gmd) {
		os << gmd.value() ;
		return os ;
	}
	
	
	/**
	 * @return The value of this CharacterData object (e.g. Health or Damage)
	 */
	unsigned long value() const {
		return baseValue * modifier ;
	}
	
	/**
	 * See value()
	 *
	 * @return The total amount of this CharacterData value (e.g. Health or Damage
	 */
	unsigned long operator()() {
		return value() ;
	}
	
} ;



/**
 * A data structure that will hold damage values
 */
struct Damage : public CharacterData {
	
public:
	
	Damage() :
		CharacterData() {}
	
	Damage(long b) :
		CharacterData(b) {}
	
	Damage(unsigned long b, unsigned m) :
		CharacterData(b, m) {}
	
	Damage(const Damage & other) :
		CharacterData(other) {}
	
	
} ;



/**
 * A data structure that will hold health values
 */
struct Health : public CharacterData {
	
public:
	
	Health() :
		CharacterData() {}
	
	Health(long b) :
		CharacterData(b) {}
	
	Health(unsigned long b, unsigned m) :
		CharacterData(b, m) {}
	
	Health(const Health & other) :
		CharacterData(other) {}
	
	
} ;



/* Below are a few enums we can use in various ways through the game.
 Note for all enums here: the value -1 essentially can be considered as essentially
 as an equivalent to NULL */
enum class Colors {
	undefined = -1,
	green,
	blue,
	purple,
	red,
	orange,
	brown,
	gray,
	silver,
	gold,
	transparent,
	various,
	//add more here
	last //always last
};

inline Colors getRandomColor() {
	Randm<unsigned> randm(0, static_cast<unsigned>(Colors::last)) ;
	//auto c = static_cast<Colors>( ;)
	return Colors::blue ;
}

#endif /* defined(__SpriteFight__CharacterData__) */


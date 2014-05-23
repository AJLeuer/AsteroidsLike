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
#include <ostream>
#include <fstream>


using namespace std ;

class Debug : public ostream {
	
private:
	ostream * out ;
	bool file_output ;
	mutex dbgMutex ;
	mutex dbgMutex1 ;
	mutex dbgMutex2 ;
	
	
	
public:
	Debug(ostream * _out) : out(_out) {}
	

	template <typename T>
	Debug & operator <<(const T & data) ;
	
	
	Debug & operator<<(std::ostream & (*ptr)(std::ostream&)) {
		dbgMutex.lock() ;
		(*out) << ptr;
		dbgMutex.unlock() ;
		return *this;
	}
	
	Debug & operator<<(Debug & (*ptr)(Debug &)) {
		dbgMutex1.lock() ;
		return ptr(*this);
		dbgMutex1.unlock() ;
	}
	
	ostream & get_ostream() { return *(this->out) ; }
	
	static Debug * debugOutput ;
	
	static void init(bool stdoutput) ;
	
	/* Use for whatever */
	static unsigned debugCounter ;
	
} ;

template <typename T>
Debug & Debug::operator<<(const T & data) {
	dbgMutex2.lock() ;
	*out << data ;
	dbgMutex2.unlock() ;
	return *this;
}



#define DebugOutput *(Debug::debugOutput)

#endif /* defined(__GameWorld__Debug__) */

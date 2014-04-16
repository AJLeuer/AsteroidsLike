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
	
	
public:
	Debug(ostream * _out) : out(_out) {}
	

	template <typename T>
	Debug & operator <<(T data) ;
	
	
	Debug & operator<<(std::ostream & (*ptr)(std::ostream&)) {
		dbgMutex.lock() ;
		(*out) << ptr;
		return *this;
		dbgMutex.unlock() ;
	}
	
	Debug & operator<<(Debug & (*ptr)(Debug &)) {
		dbgMutex.lock() ;
		return ptr(*this);
		dbgMutex.unlock() ;
	}
	
	ostream & get_ostream() { return *(this->out) ; }
	
	static Debug * debugOutput ;
	
	static void init(bool stdoutput) ;
	
	/* Use for whatever */
	static unsigned debugCounter ;
	
} ;

template <typename T>
Debug & Debug::operator<<(T data) {
	dbgMutex.lock() ;
	*out << data ;
	return *this;
	dbgMutex.unlock() ;
}








#endif /* defined(__GameWorld__Debug__) */

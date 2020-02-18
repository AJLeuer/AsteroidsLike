#pragma once

#include <iostream>
#include <fstream>
#include <chrono>

#include "../Util/Util.hpp"

#include "../Output/DisplayData.h"

#include "DefaultConfig.h"


using namespace std ;



/**
 * This class will mainly be used for overriding default settings,
 * usually based on some form of user input. 
 */
class Configuration {

private:
    
	static ifstream configFile ;
	
    static void doUserOverrides() ;
	
	static void adjustForHiDPI() ;
	

public:
    
    static bool isInit ;
    
    static void init() ;
	
	/**
	 * A floating point value used in the calculation of the on-screen size of
	 * objects. Takes into account that the user can change absolute resolution
	 */
	static double globalScalingValue() ;

} ;
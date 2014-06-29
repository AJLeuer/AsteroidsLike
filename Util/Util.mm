

#ifdef __APPLE__


#include "Util.hpp"
#include "Decl_Apple.h"


bool hiDPI_OSX = ((getDisplayScalingFactor_OSX() == 2.0) ? true : false) ;

float displayScalingFactor_OSX = getDisplayScalingFactor_OSX() ;

float getDisplayScalingFactor_OSX() {
	
    float displayScale = 1.0 ;
	
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        screens = [NSScreen screens];
        for (int i = 0; i < [screens count]; i++) {
            float s = [[screens objectAtIndex:i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
    return displayScale ;
}



#endif
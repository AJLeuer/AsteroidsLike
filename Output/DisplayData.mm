
#ifdef __APPLE__


#include "../Util/Util.hpp"

#include "DisplayData.h"
#include "Decl_Apple.h"

bool DisplayData::isInit ;

bool DisplayData::hiDPI_referenceVal ;

float DisplayData::displayScalingFactor_referenceVal ;

void DisplayData::init() {
	
	/* init displayScalingFactor_referenceVal */
	calculateDisplayScalingFactor() ;
	
	/* init hiDPI_referenceVal */
	hiDPI_referenceVal = ((displayScalingFactor_referenceVal == 2.0) ? true : false) ;
	
	isInit = true ;
}

void DisplayData::calculateDisplayScalingFactor() {
	float displayScale = 1.0 ;
	
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        screens = [NSScreen screens];
        for (int i = 0; i < [screens count]; i++) {
            float s = [[screens objectAtIndex:i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
    displayScalingFactor_referenceVal = displayScale ;
}

bool DisplayData::hiDPI() {
	
	if (isInit == false) {
		init() ;
		return DisplayData::hiDPI_referenceVal ;
	}
	else {
		return DisplayData::hiDPI_referenceVal ;
	}
	
}

float DisplayData::getDisplayScalingFactor() {
	
	if (isInit == false) {
		init() ;
		return DisplayData::displayScalingFactor_referenceVal ;
	}
	else {
		return displayScalingFactor_referenceVal ;
	}
	
}



#endif
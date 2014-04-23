//
//  OpenClUtil.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/22/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_OpenClUtil_h
#define GameWorld_OpenClUtil_h

#define __CL_ENABLE_EXCEPTIONS

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <cl.h>
#endif

#include <OpenCL/cl.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


int OpenCLArrayAdd(void) ;

#endif

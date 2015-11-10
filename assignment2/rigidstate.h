//
//  rigidstate.hpp
//  assignment2
//
//  Created by yuyaolong on 15/11/8.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#ifndef __RIGIDSTATE_H__
#define __RIGIDSTATE_H__

#include "Quaternion.h"
#include "Vector.h"
#include "rigidstateA.h"
class Rigidstate {
public:
    Vector3d xposition;
    Quaternion quater;
    Vector3d pfmom;
    Vector3d lamom;
    
    Rigidstate();
    
    Rigidstate operator+(const Rigidstate);
    
    Rigidstate operator+(const RigidstateA);
    
};


#endif /* rigidstate_hpp */

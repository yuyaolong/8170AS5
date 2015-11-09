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
class Rigidstate {
public:
    Vector3d xposition;
    Quaternion quater;
    Vector3d pfmom;
    Vector3d lamom;
    
    Rigidstate();
    
};


#endif /* rigidstate_hpp */

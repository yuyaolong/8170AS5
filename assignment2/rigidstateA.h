//
//  rigidstateA.hpp
//  assignment2
//
//  Created by yuyaolong on 15/11/8.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#ifndef __RIGIDSTATEA_H__
#define __RIGIDSTATEA_H__

#include "Quaternion.h"
#include "Vector.h"
class RigidstateA {
public:
    Vector3d velocity;
    Quaternion quaterA;
    Vector3d force;
    Vector3d torque;
    
    RigidstateA();
    
};

#endif /* rigidstateA_hpp */

//
//  rigidstateA.cpp
//  assignment2
//
//  Created by yuyaolong on 15/11/8.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#include "rigidstateA.h"

RigidstateA:: RigidstateA():velocity(Vector3d(0,0,0)),
                            quaterA(Quaternion()),
                            force(Vector3d(0,0,0)),
                            torque(Vector3d(0,0,0))
{
    
    
}


RigidstateA RigidstateA::operator*(double h)
{
    RigidstateA stateDelt;
    
    stateDelt.velocity = this->velocity * h;
    stateDelt.quaterA =  this->quaterA * h;
    stateDelt.force = this->force*h;
    stateDelt.torque = this->torque*h;

    return stateDelt;
}


RigidstateA RigidstateA::operator+(const RigidstateA a)
{
    RigidstateA newA;
    
    newA.velocity = this->velocity + a.velocity;
    newA.quaterA = this->quaterA + a.quaterA;
    newA.force = this->force + a.force;
    newA.torque = this->torque + a.torque;
    return newA;
    
    
}
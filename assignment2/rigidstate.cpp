//
//  rigidstate.cpp
//  assignment2
//
//  Created by yuyaolong on 15/11/8.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#include "rigidstate.h"


Rigidstate::Rigidstate(): xposition(Vector3d(0,0,0)),
                            quater(Quaternion()),
                            pfmom(Vector3d(0,0,0)),
                            lamom(Vector3d(0,0,0))
{
    
}



Rigidstate Rigidstate::operator+(const Rigidstate state)
{
    Rigidstate stateNew;
    stateNew.xposition = this->xposition + state.xposition;
    stateNew.quater = this->quater + state.quater;
    stateNew.pfmom = this->pfmom + state.pfmom;
    stateNew.lamom = this->lamom + state.lamom;
    
    return stateNew;
}

Rigidstate Rigidstate::operator+(const RigidstateA stateDelt)
{
    Rigidstate stateNew;
    stateNew.xposition = this->xposition + stateDelt.velocity*1;
//    Quaternion qNew = this->quater + stateDelt.quaterA * 1;
//    stateNew.quater = qNew / (qNew.norm());
    
    stateNew.quater = this->quater + stateDelt.quaterA * 1;

    
    stateNew.pfmom = this->pfmom + stateDelt.force * 1;
    stateNew.lamom = this->lamom + stateDelt.torque * 1;
    return stateNew;
}
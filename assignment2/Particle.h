//
//  Particle.h
//  assignment2
//
//  Created by yuyaolong on 15/9/19.
//  Copyright (c) 2015年 yuyaolong. All rights reserved.
//

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <stdio.h>
#include <string>
#include "Vector.h"

class Particle
{
public:
    Particle();
    Particle(const Vector3d &p, const Vector3d &v, const Vector3d &a, const Vector4d &c, const double m, const double l, const double s, const bool t, const string n);
    //~Particle();
    //Particle(const Particle&);
    //Particle& operator= (const Particle& );
    
    void setPosition(const Vector3d&);
    void setVelocity(const Vector3d&);
    void setAcceleration(const Vector3d&);
    void setColor(const Vector4d&);
    void setMass(const float);
    void setLifeSpan(const double);
    void setPointSize(const double);
    void setStopSign(const bool);
    void setName(const string);
    
    
    const Vector3d& getPosition();
    const Vector3d& getVelocity();
    const Vector3d& getAcceleration();
    const Vector4d& getColor();
    const float     getMass();
    const double   getLifeSpan();
    const double   getPointSize();
    const bool     getStopSign();
    const string   getName();
    
    
private:
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;
    Vector4d color;
    float mass;
    int lifeSpan;
    double pointSize;
    bool   stopSign;
    string name;
    
};

#endif /* defined(__assignment2__Particle__) */

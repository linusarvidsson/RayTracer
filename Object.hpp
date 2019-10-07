//
//  Object.hpp
//  RayTracer
//
//  Created by Linus Arvidsson on 2019-10-07.
//  Copyright © 2019 Linus Arvidsson. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include "Ray.hpp"

enum Material{
    DIFFUSE,
    SPECULAR,
    TRANSPARENT
};

class Object{
public:
    virtual ~Object() = 0;
    virtual bool rayIntersection(Ray &ray) = 0;
    virtual Material material() = 0;
};

#endif /* Object_hpp */

//
//  ColorDbl.hpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-09-13.
//

#ifndef ColorDbl_hpp
#define ColorDbl_hpp

#include <stdio.h>

class ColorDbl {
public:
    double r;
    double g;
    double b;
    
    ColorDbl();
    ColorDbl(double rgb);
    ColorDbl(double r_, double g_, double b_);
    
    ColorDbl operator*(double rhs);
    ColorDbl operator+(ColorDbl rhs);
};

#endif /* ColorDbl_hpp */


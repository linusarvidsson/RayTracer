//
//  ColorDbl.cpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-09-13.
//

#include "ColorDbl.hpp"

ColorDbl::ColorDbl()
{
    r = g = b = 0;
}

ColorDbl::ColorDbl(double rgb)
{
    r = g = b = rgb;
}
    
ColorDbl::ColorDbl(double r_, double g_, double b_)
{
    r = r_;
    g = g_;
    b = b_;
}

ColorDbl ColorDbl::operator*(double rhs)
{
    return ColorDbl(r * rhs, g * rhs, b * rhs);
}

ColorDbl ColorDbl::operator*(ColorDbl rhs)
{
    return ColorDbl(r * rhs.r, g * rhs.g, b * rhs.b);
}

ColorDbl ColorDbl::operator+(ColorDbl rhs)
{
    return ColorDbl(r + rhs.r, g + rhs.g, b + rhs.b);
}

#include "Vertex.hpp"

Vertex::Vertex()
{
    x = y = z = w = 0;
}
    
Vertex::Vertex(double x_, double y_, double z_, double w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

Vertex::Vertex(Vector v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1;
}
    
Vector Vertex::operator-(Vertex rhs)
{
    return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vertex Vertex::operator+(Vector rhs)
{
    return Vertex(x + rhs.x, y + rhs.y, z + rhs.z, w);
}

double Vertex::magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

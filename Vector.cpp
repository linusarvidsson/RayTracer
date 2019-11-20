#include "Vector.hpp"

Vector::Vector()
    : x(0), y(0), z(0) {}

Vector::Vector(double x_, double y_, double z_)
    : x(x_), y(y_), z(z_) {}

Vector Vector::operator+(Vector rhs)
{
    return Vector (x + rhs.x, y + rhs.y, z + rhs.z);
}
    
Vector Vector::operator-(Vector rhs)
{
    return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
};

Vector Vector::operator*(double rhs)
{
    return Vector(x * rhs, y * rhs, z * rhs);
}
    
Vector Vector::cross(Vector rhs)
{
    return Vector(
      y * rhs.z - z * rhs.y,
      z * rhs.x - x * rhs.z,
      x * rhs.y - y * rhs.x
    );
}
    
double Vector::dot(Vector rhs)
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

double Vector::magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

double Vector::magnitudeSquared()
{
    return x*x + y*y + z*z;
}

Vector Vector::normalize(){
    double magnitude = this->magnitude();
    return Vector(x / magnitude, y / magnitude, z / magnitude);
}

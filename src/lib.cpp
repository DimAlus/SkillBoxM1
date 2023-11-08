#include <string>
#include <math.h>

#include "lib.h"

std::ostream &operator<<(std::ostream &out, Vector &v)
{
    out << "(" << v.x << ';' << v.y << ';' << v.z << ")";
    return out;
}
std::istream &operator>>(std::istream &in, Vector &v)
{
    in >> v.x;
    in >> v.y;
    in >> v.z;

    // out << "(" << v.x << ';' << v.y << ';' << v.z << ")";
    return in;
}
Vector operator+(const Vector &v1, const Vector &v2)
{
    return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vector operator-(const Vector &v1, const Vector &v2)
{
    return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
Vector operator*(const Vector &v1, const float f)
{
    return Vector(v1.x * f, v1.y * f, v1.z * f);
}
bool operator>(Vector &v1, Vector &v2)
{
    return float(v1) > float(v2);
}
bool operator<(Vector &v1, Vector &v2)
{
    return float(v1) < float(v2);
}
bool operator==(Vector &v1, Vector &v2)
{
    return float(v1) == float(v2);
}

Vector::operator float()
{
    return sqrt(x * x + y * y + z * z);
}

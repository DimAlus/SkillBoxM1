#include <iostream>
#include <string>
#include <math.h>

class Vector {
private:
    float x, y, z;

public:
    Vector() : x(0), y(0), z(0) { }
    Vector(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
    //Vector(float X, float Y, float Z) { x =(X); y=(Y); z=(Z); }

    friend Vector operator+(const Vector& v1, const Vector& v2);
    friend Vector operator-(const Vector& v1, const Vector& v2);
    friend Vector operator*(const Vector& v1, const float f);
    friend bool operator>(Vector& v1, Vector& v2);
    friend bool operator<(Vector& v1, Vector& v2);
    friend bool operator==(Vector& v1, Vector& v2);
    friend std::ostream& operator<<(std::ostream& out, Vector& v);
    friend std::istream& operator>>(std::istream& in, Vector& v);
    operator float() {
        return sqrt(x * x + y * y + z * z);
    }
};

std::ostream& operator<<(std::ostream& out, Vector& v) {
    out << "(" << v.x << ';' << v.y << ';' << v.z << ")";
    return out;
}
std::istream& operator>>(std::istream& in, Vector& v) {
    in >> v.x;
    in >> v.y;
    in >> v.z;

    //out << "(" << v.x << ';' << v.y << ';' << v.z << ")";
    return in;
}
Vector operator+(const Vector& v1, const Vector& v2) {
    return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vector operator-(const Vector& v1, const Vector& v2) {
    return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
Vector operator*(const Vector& v1, const float f) {
    return Vector(v1.x * f, v1.y * f, v1.z * f);
}
bool operator>(Vector& v1, Vector& v2) {
    return float(v1) > float(v2);
}
bool operator<(Vector& v1, Vector& v2) {
    return float(v1) < float(v2);
}
bool operator==(Vector& v1, Vector& v2) {
    return float(v1) == float(v2);
}
int main()
{
    Vector v1(1, 2, 3), v2(4, 5, 6), v3;
    std::cout << "Please, enter vector: \n";
    std::cin >> v3;
    std::cout << "\nYou enter: " << v3 << "\n";
    std::cout << "Length: " << float(v3) << "\n";
    std::cout << v3 << " < " << v1 << ": " << (v3 < v1) << "\n";
    std::cout << v2 << " > " << v3 << ": " << (v2 > v3) << "\n";
    std::cout << v2 << " * " << float(v3) << ": " << (v2 * float(v3)) << "\n";
}

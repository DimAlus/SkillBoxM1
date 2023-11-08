#pragma once

#include <iostream>

class Vector
{
private:
    float x, y, z;

public:
    Vector() : x(0), y(0), z(0) {}
    Vector(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
    // Vector(float X, float Y, float Z) { x =(X); y=(Y); z=(Z); }

    friend Vector operator+(const Vector &v1, const Vector &v2);
    friend Vector operator-(const Vector &v1, const Vector &v2);
    friend Vector operator*(const Vector &v1, const float f);
    friend bool operator>(Vector &v1, Vector &v2);
    friend bool operator<(Vector &v1, Vector &v2);
    friend bool operator==(Vector &v1, Vector &v2);
    friend std::ostream &operator<<(std::ostream &out, Vector &v);
    friend std::istream &operator>>(std::istream &in, Vector &v);
    operator float();
};

std::ostream &operator<<(std::ostream &out, Vector &v);
std::istream &operator>>(std::istream &in, Vector &v);
Vector operator+(const Vector &v1, const Vector &v2);
Vector operator-(const Vector &v1, const Vector &v2);
Vector operator*(const Vector &v1, const float f);
bool operator>(Vector &v1, Vector &v2);
bool operator<(Vector &v1, Vector &v2);
bool operator==(Vector &v1, Vector &v2);

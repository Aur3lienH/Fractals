#pragma once

template <typename T>
class Vector2
{
public:
    Vector2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }
    Vector2()
    {
        this->x = 0;
        this->y = 0;
    }
    T x;
    T y;
};

template <typename T>
class Vector3
{
public:
    Vector3(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    T x;
    T y;
    T z;
};
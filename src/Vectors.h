#pragma once

template <typename T>
class Vector2
{
public:
    Vector2(T x, T y)
    {
        array = new T[2];
        array[0] = x;
        array[1] = y;
    }
    Vector2()
    {
        array = new T[2];
        array[0] = 0;
        array[1] = 0;
    }
    T* array;
};

template <typename T>
class Vector3
{
public:
    Vector3(T x, T y, T z)
    {
        array = new T[3];
        array[0] = x;
        array[1] = y;
        array[2] = z;
    }
    Vector3()
    {
        array = new T[3];
        array[0] = 0;
        array[1] = 0;
        array[2] = 0;
    }
    T* array;
};

template <typename T>
class Vector4
{
public:
    T operator [](int index)
    {
        return array[index];
    }

    Vector4(T x, T y, T z, T w)
    {
        array = new T[4];
        array[0] = x;
        array[1] = y;
        array[2] = z;
        array[3] = w;
    }
    Vector4()
    {
        array = new T[4];
        array[0] = 0;
        array[1] = 0;
        array[2] = 0;
        array[3] = 0;
    }
    T* array;
};
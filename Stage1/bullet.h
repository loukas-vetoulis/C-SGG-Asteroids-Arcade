#pragma once
#include "box.h"

class Bullet : public Box, public GameObject
{
public:
    Bullet(float x, float y, float w, float h, float dx, float dy)
        : Box(x,y,w,h,dx,dy) {}
};

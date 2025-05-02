#pragma once
#include <math.h>
#include <algorithm>
#include "gamestate.h"

struct Box
{
    float m_pos_x  = 3.0f;
    float m_pos_y  = 3.0f;
    float m_width  = 1.0f;
    float m_height = 1.0f;
    float m_accel;
    float dx;
    float dy;
    float m_rotation;

    /** Detects intersection (overlap) between this Box and another Box instance.
    *   \param other is a reference to another Box instance to check for collision with
    *   \return true if the two boxes overlap
    */

    void wrapCordinates(float& out_m_pos_x, float& out_m_pos_y)
    {

        if (out_m_pos_x < 0.0f) out_m_pos_x = out_m_pos_x + Gamestate::getInstance()->getCanvasWidth();
        if (out_m_pos_x > Gamestate::getInstance()->getCanvasWidth()) out_m_pos_x = out_m_pos_x - Gamestate::getInstance()->getCanvasWidth();

        if (out_m_pos_y < 0.0f) out_m_pos_y = out_m_pos_y + Gamestate::getInstance()->getCanvasHeight();
        if (out_m_pos_y > Gamestate::getInstance()->getCanvasHeight()) out_m_pos_y = out_m_pos_y - Gamestate::getInstance()->getCanvasHeight();
    }

    /** Default ctor
    */
    Box() {}

    /** Basic Box ctor.
    *   \param x is the x coordinate of the box center
    *   \param y is the y coordinate of the box center
    *   \param w is the width of the box
    *   \param h is the height of the box
    */
    Box(float x, float y, float w, float h) 
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h) {}

    /** 
    For Bullet
    */
    Box(float x, float y, float w, float h, float dx, float dy)
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h) ,dx(dx), dy(dy) {}

};


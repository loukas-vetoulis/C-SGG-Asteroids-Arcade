#pragma once
#include "box.h"
#include "gamestate.h"
#include "gameobject.h"
#include <list>

class Asteroid : public Box, public GameObject
{
public:
    int leveled=1; 
    // Constructor
    Asteroid(float x, float y, float w, float h, float dx, float dy, int leveled)
        : Box(x, y, w, h, dx, dy), leveled(leveled) {}
    
	void split_Asteroid(Asteroid* it_asteroid, std::list<Asteroid>& m_asteroids);
	
	void update(float dt);
};

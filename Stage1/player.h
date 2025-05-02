#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"
#include "bullet.h"


class Player : public Box,public GameObject {
	graphics::Brush m_brush_player;
	graphics::Brush bullet_brush;

	void init_bullet_brush();
	void drawBullet(int i, graphics::Brush debug_brush);

	const float M_PI = 3.141592;
	const float radian_to_degrees = 180.0f / M_PI;
	const float m_max_velocity = 10.0f;

	float calculateAngle(float x1, float y1, float x2, float y2);
	void movePlayer(float dt);
	void rotate();
	int health;
public:
	int getHealth() { return health; }
	void setHealth(int i) { health = i; }
	void shootBullet(float dt);
	const float bullespeed= 6.0f;
	const float bullesize = 0.05;

	std::vector<Bullet> m_bullets;
	float m_vx = 0.0f;
	float m_vy = 0.0f;

	Player(std::string name) : GameObject(name) {}
	
	void update(float dt) override;
	void init() override;
	void draw() override;
	float get_m_rotation()
	{
		return m_rotation;
	}
	float get_m_accel()
	{
		return m_accel;
	}

protected:
	void debugDraw();
};
#pragma once
#include"gameobject.h"
#include "sgg/graphics.h"
#include "asteroid.h"
#include <list>
#include "player.h"
#include <vector>
#include <string>
#include <random>


class Level :public GameObject {
	graphics::Brush m_brush_background;
	int score = 0;

	struct CollisionEffect {
		float x, y;       // Position of the collision
		float size;       // Size of explosiom
		float timer;      // Time remaining for the effect
		float total_duration;
	};
	//make it apear as dynamic
	std::vector<CollisionEffect> m_collisionEffects;
	void draw_m_collisionEffects(std::vector<CollisionEffect> *m_collisionEffects);

	//for making new asteroids
	const float MIN_DISTANCE = 2.00f; // Minimum safe distance from player
	float distance;

	//starting size
	const std::vector<int> values = { 1, 2, 4 };
	// Define probabilities for values 1, 2, and 4
	const std::vector<double> probabilities = { 1.0, 1.0, 1.0 };  // Equal probability for each value
	const float starting_size = 1.2f;


	std::vector<GameObject*> m_static_object;
	std::list<Box*> m_dynamic_object;

	std::list<Asteroid> m_asteroids;
	std::string m_asteroids_names;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;
	graphics::Brush score_brush;
	graphics::Brush health_brush;


	std::string score_text;
	std::string health_text;

	void drawBlock(int i);
	void push_asteroids(int i);
	void checkCollisions();
	void increase_difficulity(float dt);
	void drawscore();
	void drawStarting();
	void drawhealth();
	std::random_device rd; // Seed generator

	//increasing difficulity
	const float how_difficult = 6000.0f;
	float increase_speed = 1.0f;
public:
	// Static variable to track accumulated time between difficulty increases
	bool isPointInsideCircle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius-radius/2;
	}
	void update(float dt) override;
	void init() override;	
	void draw() override;

	Level(const std::string& name = "Level0");
	~Level();
};
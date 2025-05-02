#pragma once
#include "level.h"
#include "gamestate.h"
#include "player.h"
#include "sgg/graphics.h"
#include "util.h"
#include "bullet.h"
#include "asteroid.h"
#include <random>
#include <iostream>

void Level::drawscore() 
{
	// Draw score in top right corner
	SETCOLOR(score_brush.fill_color, 1.0f, 1.0f, 0.0f);  // Yellow color (RGB: 1,1,0)
	score_brush.fill_opacity = 1.0f;

	// Convert score to string
	score_text = "Score: " + std::to_string(score);

	// Position in top right with some padding
	float score_x = m_state->getCanvasWidth() - m_state->getCanvasWidth()/6.0f;  // Adjust this value for desired padding
	float score_y = m_state->getCanvasWidth()/12;  // Adjust this value for desired padding
	graphics::drawText(score_x, score_y, m_state->getCanvasWidth() / 55, score_text, score_brush);
}

void Level::drawhealth()
{
	// Draw score in top right corner
	SETCOLOR(health_brush.fill_color, 1.0f, 0.0f, 0.0f);  // red color (RGB: 1,1,0)
	health_brush.fill_opacity = 1.0f;

	// Convert score to string
	health_text = "Lives Left: " + std::to_string(m_state->getPlayer()->getHealth());

	// Position in top right with some padding
	float score_x = m_state->getCanvasWidth() / 9.0f;  // Adjust this value for desired padding
	float score_y = m_state->getCanvasWidth() / 12;  // Adjust this value for desired padding
	graphics::drawText(score_x, score_y, m_state->getCanvasWidth() / 55, health_text, health_brush);
}



void Level::push_asteroids(int asteroids_count)
{
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<float> dis_position(0.0f, 6.0f);
	std::uniform_real_distribution<float> dis_velocity(-1.0f, 1.0f);
	
	std::discrete_distribution<int> dist(probabilities.begin(), probabilities.end());

	
	for (int i = 0; i < asteroids_count; i++) {
		float x, y;
		float distance;

		if (m_state->getPlayer())
		{
			// Keep generating positions until we find one far enough from the player
			do {
				x = dis_position(gen);
				y = dis_position(gen);

				// Calculate distance from player using Pythagorean theorem
				float dx = x - m_state->getPlayer()->m_pos_x;
				float dy = y - m_state->getPlayer()->m_pos_y;
				distance = std::sqrt(dx * dx + dy * dy);
			} while (distance <= MIN_DISTANCE);
		}
		else
		{
			x = dis_position(gen);
			y = dis_position(gen);
		}

		float vel_x = dis_velocity(gen);
		float vel_y = dis_velocity(gen);
		float size = starting_size;
		int leveled = values[dist(gen)]; 

		m_asteroids.push_back(Asteroid(x, y, size/leveled, size/leveled, vel_x*increase_speed, vel_y*increase_speed, leveled));
	}
}

void Level::draw_m_collisionEffects(std::vector<CollisionEffect>* m_collisionEffects)
{
	for (auto it = m_collisionEffects->begin(); it != m_collisionEffects->end();) {
		// Calculate which texture to use based on elapsed time
		int total_frames = 5; // Total number of explosion textures
		int current_frame = static_cast<int>(((it->total_duration - it->timer) / it->total_duration) * total_frames);

		// Clamp the frame index to the range [0, total_frames - 1]
		current_frame = std::min(current_frame, total_frames - 1);

		// Set the texture based on the frame
		graphics::Brush explosion_brush;
		explosion_brush.texture = m_state->getFullAssetPath("explosion" + std::to_string(current_frame) + ".png");
		explosion_brush.fill_opacity = 1.0f;
		explosion_brush.outline_opacity = 0.0f;

		// Draw the explosion effect
		graphics::drawRect(it->x, it->y, it->size, it->size, explosion_brush);

		// Decrease timer and remove the effect if expired
		it->timer -= graphics::getDeltaTime() / 1000.0f;
		if (it->timer <= 0.0f) {
			it = m_collisionEffects->erase(it);
		}
		else {
			++it;
		}
	}
};


void Level::drawBlock(int i)
{
	// Create an iterator and advance it to the i-th element
	auto it = m_asteroids.begin();
	std::advance(it, i);  // Move the iterator to the i-th position

	// Dereference the iterator to get the Asteroid reference
	Asteroid& asteroid = *it;

	float x = asteroid.m_pos_x;
	float y = asteroid.m_pos_y;
	m_block_brush.texture = m_state->getFullAssetPath(m_asteroids_names);

	graphics::drawRect(x, y, asteroid.m_width, asteroid.m_height, m_block_brush);

	if (m_state->m_debbuging)
		graphics::drawRect(x, y, asteroid.m_width, asteroid.m_height, m_block_brush_debug);
	
}


void Level::checkCollisions()
{
	for (auto it_asteroid = m_asteroids.begin(); it_asteroid != m_asteroids.end();) {
		bool box_erased = false;

		// Check collision with bullets
		for (auto it_bullet = m_state->getPlayer()->m_bullets.begin(); it_bullet != m_state->getPlayer()->m_bullets.end();) {
			if (isPointInsideCircle(it_asteroid->m_pos_x, it_asteroid->m_pos_y, it_asteroid->m_width, it_bullet->m_pos_x, it_bullet->m_pos_y)) {

				score += 25 * (it_asteroid->leveled);

				graphics::playSound(m_state->getFullAssetPath("asteroid_hitting.mp3"), 0.01f);
				m_collisionEffects.push_back({ it_asteroid->m_pos_x, it_asteroid->m_pos_y,it_asteroid->m_width ,0.5f,0.5f }); // 1 second duration

				// Erase bullet
				it_bullet = m_state->getPlayer()->m_bullets.erase(it_bullet);

				it_asteroid->split_Asteroid(&(*it_asteroid), m_asteroids);
				it_asteroid = m_asteroids.erase(it_asteroid);

				box_erased = true; // Mark the box as erased
				break; // Exit the bullet loop
			}
			else {
				++it_bullet; // Move to the next bullet
			}
		}

		// If the box was erased, continue to the next box
		if (box_erased) {
			continue;
		}

		// Check collision with the player
		if (isPointInsideCircle(it_asteroid->m_pos_x, it_asteroid->m_pos_y, it_asteroid->m_width, m_state->getPlayer()->m_pos_x, m_state->getPlayer()->m_pos_y)) {;
			// Erase the box
			m_collisionEffects.push_back({ it_asteroid->m_pos_x, it_asteroid->m_pos_y,m_state->getPlayer()->m_width * 5,0.5f,0.5f }); // 1 second duration,big boom
			it_asteroid = m_asteroids.erase(it_asteroid);
			graphics::playSound(m_state->getFullAssetPath("boom.mp3"), 0.5f);
			//move player out of map
			m_state->getPlayer()->m_pos_x = m_state->getCanvasWidth() + 10;
			m_state->getPlayer()->m_pos_y = m_state->getCanvasHeight() + 10;
			m_state->getPlayer()->setActive(false);  
			m_state->getPlayer()->setHealth(m_state->getPlayer()->getHealth()-1);
			if (m_state->getPlayer()->getHealth() <= 0)
			{
				m_state->setFinal_score(score);
				m_state->setGame_scene(2);
			}
			continue; // Restart the loop to process the next box
		}

		// Move to the next box
		++it_asteroid;
	}
}


void Level::update(float dt)
{
	
	increase_difficulity(dt);

	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);
	else
	{
		if (graphics::getKeyState(graphics::SCANCODE_SPACE) && m_state->getPlayer()->getHealth()>0)
		{
			m_state->getPlayer()->m_pos_x = m_state->getCanvasWidth() / 2;
			m_state->getPlayer()->m_pos_y = m_state->getCanvasHeight() / 2;
			m_state->getPlayer()->setActive(true);
		}
	}
	
	checkCollisions();
	GameObject::update(dt);

	for (auto& asteroid : m_asteroids) {
		asteroid.update(dt);
	}
}

void Level::draw()
{

	// Center the background at (0.0, 0.0) in world coordinates
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), m_state->getCanvasHeight(), m_brush_background);

	//drawing score
	drawscore();
	drawhealth();

	// Draw player if active
	if (m_state->getPlayer()->isActive())
	{
		m_state->getPlayer()->draw();  // Assuming the player draws relative to world coordinates
	}

	// Draw blocks
	for (int i = 0; i < m_asteroids.size(); i++) {
		drawBlock(i);
	}
	
	// Draw collision effects
	draw_m_collisionEffects(&m_collisionEffects);
}



void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background.png");

	for (auto p_gob : m_static_object)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_object)
		if (p_gob) p_gob;


	m_asteroids_names = "asteroid1.png";

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.2f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
}




Level::Level(const std::string& name)
{
}

Level::~Level()
{
	for (auto p_gob : m_static_object)
		if (p_gob) delete p_gob;

	for (auto p_gob : m_dynamic_object)
		if (p_gob) delete p_gob;

	m_asteroids.clear();  // This is optional since the vector will clean itself up

}

void Level::increase_difficulity(float dt)
{
	// Static variable to track accumulated time between difficulty increases
	static float time_accumulator = 0.0f;
	time_accumulator += dt;

	if (time_accumulator >= how_difficult && m_state->getPlayer()->isActive()) {
		score += 1000;
		//random amount of asteroids each time
		std::mt19937 gen(rd()); // Mersenne Twister engine
		std::uniform_int_distribution<int> dis_position(2, 4);

		increase_speed *= 1.05f;
		push_asteroids(dis_position(gen));
		time_accumulator = 0.0f;  // Reset accumulator
	}

}


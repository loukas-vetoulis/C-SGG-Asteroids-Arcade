#include "asteroid.h"

void Asteroid::update(float dt)
{
	m_pos_x += dx * dt / 1000;
	m_pos_y += dy * dt / 1000;

	wrapCordinates(m_pos_x, m_pos_y);
}

void Asteroid::split_Asteroid(Asteroid* it_asteroid, std::list<Asteroid>& m_asteroids)
{
	// Erase box
	if (it_asteroid->leveled <= 2)
	{
		// Calculate the new velocities for split asteroids
		float original_speed = sqrt(it_asteroid->dx * it_asteroid->dx + it_asteroid->dy * it_asteroid->dy);
		float angle_1 = atan2(it_asteroid->dy, it_asteroid->dx) + 3.14 / 4;  // Original angle + 45 degrees
		float angle_2 = atan2(it_asteroid->dy, it_asteroid->dx) - 3.14 / 4;  // Original angle - 45 degrees

		// First asteroid (45 degrees clockwise)
		m_asteroids.push_back(Asteroid(
			it_asteroid->m_pos_x, it_asteroid->m_pos_y,
			it_asteroid->m_width / 2, it_asteroid->m_height / 2,
			original_speed * cos(angle_1), original_speed * sin(angle_1),
			it_asteroid->leveled * 2));

		// Second asteroid (45 degrees counter-clockwise)
		m_asteroids.push_back(Asteroid(
			it_asteroid->m_pos_x, it_asteroid->m_pos_y,
			it_asteroid->m_width / 2, it_asteroid->m_height / 2,
			original_speed * cos(angle_2), original_speed * sin(angle_2),
			it_asteroid->leveled * 2));
	}
}
#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
#include "util.h"
#include <iostream>

Gamestate::Gamestate()
{
}

void Gamestate::init() {
	graphics::preloadBitmaps(getAssetDir());
	graphics::setFont(m_asset_path + "nulshockbd.otf");
	map_draw[0] = &Gamestate::drawStarting;
	map_draw[1] = &Gamestate::drawLevel;
	map_draw[2] = &Gamestate::drawEnding;
	map_draw[3] = &Gamestate::drawHelp;

	if(game_scene == 0)
		graphics::playMusic(getFullAssetPath("arcade.mp3"), 0.8f, true);

	if (game_scene == 1) {
		graphics::stopMusic(1500);
		m_current_level = new Level();
		m_current_level->init();

		graphics::playSound(getFullAssetPath("warp.mp3"), 1.0f);
		m_player = new Player("Player");
		m_player->init();
	}
}


void Gamestate::draw()
{
	if (!m_current_level && game_scene==1)
		return;

	(this->*map_draw[game_scene])();
}

void Gamestate::update(float dt)
{

	if (dt > 500)
		return;

	float sleep_time = std::max(0.0f, 10.0f - dt);

	std::this_thread::sleep_for(std::chrono::duration<float,std::milli>(sleep_time));

	p = fabs(cos(graphics::getGlobalTime() / 300.0f));

	if (game_scene == 0)
	{
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			if (!is_init)
			{
				game_scene = 1;  // Start the game
				init();
				is_init = true;
			}
		}
		else if (graphics::getKeyState(graphics::SCANCODE_H))
			game_scene = 3; //go to help menu
	}
	else if(game_scene == 3)
	{
		if (graphics::getKeyState(graphics::SCANCODE_B))
			game_scene = 0; // go back to starting screen
	}

	if (!m_current_level)
		return;
	m_current_level->update(dt);

	m_debbuging = graphics::getKeyState(graphics::SCANCODE_0);
}

Gamestate* Gamestate::getInstance()
{
	if (m_unique_instance == nullptr)
		m_unique_instance = new Gamestate();

	return m_unique_instance;
}

Gamestate::~Gamestate()
{
	if (m_player)
		delete m_player;
	if (m_current_level)
		delete m_current_level;
}

std::string Gamestate::getAssetDir()
{
	return m_asset_path;
}

std::string Gamestate::getFullAssetPath(const std::string& asset)
{
	return m_asset_path+asset;
}

Gamestate* Gamestate::m_unique_instance=nullptr;

void Gamestate::drawStarting()
{
	starting_background.outline_opacity = 0.0f;
	starting_background.texture = getFullAssetPath("background.png");

	graphics::drawRect(m_canvas_width / 2.0f, m_canvas_height / 2.0f, m_canvas_width, m_canvas_height, starting_background);

	// Set the text color to pink
	SETCOLOR(starting_brush.fill_color, 1.0f+get_p(), 0.0f + get_p(), 1.0f + get_p());  // Pink color (RGB: 1, 0, 1)
	starting_brush.fill_opacity = 1.0f;

	graphics::drawText(m_canvas_width / 2.0f - m_canvas_width / 4.0f, m_canvas_height / 2.0f , m_canvas_width / 30, starting_text, starting_brush);

	SETCOLOR(help_brush_start.fill_color, 1.0f - p/2, 1.0f - p/2, 1.0f - p/2);

	graphics::drawText(m_canvas_width - m_canvas_width / 3.8, m_canvas_height - m_canvas_height / 6, m_canvas_width / 65, help_text_start, help_brush_start);

}

void Gamestate::drawEnding()
{
	graphics::playSound(getFullAssetPath("intro.mp3"), 1.0f);

	ending_background.outline_opacity = 0.0f;
	ending_background.texture = getFullAssetPath("ending_scene.png");
	graphics::drawRect(m_canvas_width / 2.0f, m_canvas_height / 2.0f, m_canvas_width, m_canvas_height, ending_background);

	// Set the text color to red
	SETCOLOR(ending_brush.fill_color, 1.0f + get_p()/2, 0.0f + get_p()/2, 0.0f + get_p()/2);  // Red color (RGB: 1, 0, 0)
	ending_brush.fill_opacity = 1.0f;
	graphics::drawText(m_canvas_width / 2.0f - m_canvas_width / 6.0f, m_canvas_height / 2.0f, m_canvas_width / 50, ending_text + std::to_string(final_score), ending_brush);
}

void Gamestate::drawLevel()
{
	m_current_level->draw();
}

void Gamestate::drawHelp()
{
	help_backkground.outline_opacity = 0.0f;
	help_backkground.texture = getFullAssetPath("help.png");


	graphics::drawRect(m_canvas_width / 2.0f, m_canvas_height / 2.0f, m_canvas_width, m_canvas_height, help_backkground);

	SETCOLOR(help_brush.fill_color, 1.0f-p, 1.0f-p, 1.0f-p);
	help_backkground.fill_opacity = 1.0f;

	graphics::drawText(m_canvas_width - m_canvas_width/3.8, m_canvas_height - m_canvas_height/6, m_canvas_width / 65, help_text_leave, help_brush);

}
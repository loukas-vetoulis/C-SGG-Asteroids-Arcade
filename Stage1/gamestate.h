#pragma once
#include <string>
#include <sgg/graphics.h>
#include <map>

class Gamestate {
private:
	std::string m_asset_path = "assets\\";
	graphics::Brush starting_background;
	graphics::Brush starting_brush;

	graphics::Brush help_backkground;
	graphics::Brush help_brush;
	graphics::Brush help_brush_start;

	graphics::Brush ending_brush;
	graphics::Brush ending_background;

	std::map<int, void(Gamestate::*)()> map_draw;
	void drawLevel();

	float m_canvas_width = 6.0f;
	float m_canvas_height = 6.0f;
	int game_scene = 0;
	bool is_init = false;
	float p=0;
	int final_score=0;

	void drawStarting();
	void drawEnding();
	void drawHelp();

	static Gamestate* m_unique_instance;

	Gamestate();

	class Player* m_player = 0;
	class Level* m_current_level = 0;
	const std::string starting_text = "Press (Space) To Start:";
	const std::string ending_text = "You ended with (score): ";
	const std::string help_text_start= "Press (H) to go to Help";
	const std::string help_text_leave = "Press (B) to leave Help";


public:	
	float get_p() { return p; }
	int getGame_scene() { return game_scene; }
	void setGame_scene(int i) { game_scene = i; }
	void setFinal_score(int i) { final_score = i; }
	bool m_debbuging = false;
	bool m_game_started;

	void init();
	void draw();
	void update(float dt);

	static Gamestate* getInstance();
	~Gamestate();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	class Player* getPlayer(){ return m_player; }
	class Level* getLevel() { return m_current_level; }
};
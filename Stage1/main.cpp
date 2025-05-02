#include "sgg/graphics.h"
#include <stdio.h>
#include "util.h"
#include <string>
#include "gamestate.h"





void init() {
	Gamestate::getInstance()->init();
}

void draw() {
	Gamestate::getInstance()->draw();
}

void update(float dt) {
	Gamestate::getInstance()->update(dt);
}



int main(int argc, char** argv) {

	graphics::createWindow(800, 800, "Boing Stage2");

	init();

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::setCanvasSize(Gamestate::getInstance() ->getCanvasWidth(),
		Gamestate::getInstance()->getCanvasHeight());

	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	graphics::startMessageLoop();

	return 0;	
}
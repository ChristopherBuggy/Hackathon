#include "Level.h"
Level::Level(b2World& World, Render* renderer)
{
	platforms[0] = new Platform(50, 600, 100, 10, World, renderer);
	platforms[1] = new Platform(150, 600, 100, 10, World, renderer);

	platforms[2] = new Platform(349, 600, 100, 10, World, renderer);

	platforms[3] = new Platform(600, 500, 100, 10, World, renderer);

	platforms[4] = new Platform(750, 600, 100, 10, World, renderer);
	platforms[5] = new Platform(850, 600, 100, 10, World, renderer);
	platforms[6] = new Platform(950, 600, 100, 10, World, renderer);
	platforms[7] = new Platform(1050, 500, 100, 10, World, renderer);

	platforms[8] = new Platform(50, 400, 100, 10, World, renderer);
	platforms[9] = new Platform(150, 400, 100, 10, World, renderer);
	platforms[10] = new Platform(250, 400, 100, 10, World, renderer);
	platforms[11] = new Platform(349, 400, 100, 10, World, renderer);
	platforms[12] = new Platform(449, 400, 100, 10, World, renderer);
	platforms[13] = new Platform(549, 400, 100, 10, World, renderer);
	platforms[14] = new Platform(649, 400, 100, 10, World, renderer);
	platforms[15] = new Platform(749, 400, 100, 10, World, renderer);
	platforms[16] = new Platform(849, 400, 100, 10, World, renderer);

	platforms[17] = new Platform(1100, 300, 100, 10, World, renderer);
	platforms[18] = new Platform(1200, 300, 100, 10, World, renderer);
}

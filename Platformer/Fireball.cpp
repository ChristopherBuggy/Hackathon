#include "Fireball.h"
static const float SCALE = 30.f;

Fireball::Fireball(float x, float y, b2World& World, Render* renderer)
{
	world = &World;

	initX = x;
	initY = y;

	std::string basepath(SDL_GetBasePath());

	std::string imagePath = basepath + "fireball.bmp";
	sprite = SDL_LoadBMP(imagePath.c_str());

	if (spriteRect == NULL)
	{
		spriteRect = renderer->AddSurfaceToRenderer(sprite, initX, initY, 0.5f);
	}
	
		
	CreateBody();
}

void Fireball::RemoveSurface()
{
}

void Fireball::CreateBody()
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(initX / SCALE, initY / SCALE);
	BodyDef.type = b2_dynamicBody;
	BodyDef.userData = "fireball";
	b2Body* Body = world->CreateBody(&BodyDef);
	Body->SetFixedRotation(true);
	Body->SetSleepingAllowed(false);
	Body->SetGravityScale(0.0f);

	b2PolygonShape Shape;
	Shape.SetAsBox(spriteRect->w /2 / SCALE, spriteRect->h / 2 / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 10.0f;
	FixtureDef.friction = 0.f;
	FixtureDef.shape = &Shape;
	FixtureDef.isSensor = true;
	Body->CreateFixture(&FixtureDef);

	fireballBody = Body;
}

void Fireball::Update(int dir)
{
	if (spriteRect != NULL)
	{
		if (dir == 2)
		{
			fireballBody->SetTransform(b2Vec2(fireballBody->GetPosition().x - 0.25f, fireballBody->GetPosition().y), 0);
		}
		if (dir == 1)
		{
			fireballBody->SetTransform(b2Vec2(fireballBody->GetPosition().x + 0.25f, fireballBody->GetPosition().y), 0);
		}

		spriteRect->x = (fireballBody->GetPosition().x) * SCALE + 20;
		spriteRect->y = (fireballBody->GetPosition().y) * SCALE + 20;
	}
}

bool Fireball::CheckCollision(SDL_Rect* playerRect)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = spriteRect->x;
	rightA = spriteRect->x + spriteRect->w;
	topA = spriteRect->y;
	bottomA = spriteRect->y + spriteRect->h;

	//Calculate the sides of rect B
	leftB = playerRect->x;
	rightB = playerRect->x + playerRect->w;
	topB = playerRect->y;
	bottomB = playerRect->y + playerRect->h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Fireball::CheckLife()
{
	if (fireballBody->GetPosition().x < -275 / SCALE)
		fireballBody->SetTransform(b2Vec2(830 / SCALE, 379 / SCALE), 0);
		//fireballBody->DestroyFixture(fireballBody->GetFixtureList());
		//fireballBody->Dump();

		//spriteRect->x = -1000;
		//spriteRect->y = -1000;

		//RemoveSurface();
		//spriteRect = NULL;
		//sprite = NULL;
		//return true;
	
	else if (fireballBody->GetPosition().x > 830 / SCALE)
		fireballBody->SetTransform(b2Vec2(-275 / SCALE, 379 / SCALE), 0);

	return false;
}

#include "Edengine.h"

Entity::Entity()
{
	speed_x = 0;
	speed_y = 0;
	FacingRight = 0;
	pos_x = 8;
	pos_y = 8;
	mysize = 0.8;
}

Entity::~Entity(){}

Entity::Entity(double x, double y)
{
	speed_x = 0;
	speed_y = 0;
	FacingRight = 0;
	mysize = 1;
	Move(x,y);
}

void Player::Control()
{
	Direction	=(key[SDL_SCANCODE_Z])+
		0x02*key[SDL_SCANCODE_LEFT]+
		0x04*key[SDL_SCANCODE_RIGHT]+
		0x08*key[SDL_SCANCODE_DOWN]+
		0x10*key[SDL_SCANCODE_UP];
}

int Entity::Update()
{
	Move(pos_x + speed_x,pos_y + speed_y);
	return 0;
}

int IUnit::Update()
{
	Entity::Update();
		Collided = false;

	if (Invincible > 0)
		Invincible -= 1;
	return 0;
}

int Creature::Update()
{
	IUnit::Update();
/*	if(Direction & 0x10)
		Dash();*/
	
	speed_x *= (double)1 / 2;
	if (speed_x < 0.02 && speed_x > -0.02)
		speed_x = 0;
	if (speed_y < 0.02 && speed_y > -0.02)
		speed_y = 0;
	Standing = false;
	return 0;
}

int GroundEnemy::Update()
{
	Control();

		Push(0,0.04);
	if (Direction & 0x01)
		if (Standing)
		{
			Standing = false;
			Ascend();
		}

	if (Direction & 0x02)
		WalkLeft();
	else if(Direction & 0x04)
		WalkRight();

	if (Direction & 0x08)
		Descend();
	return Creature:: Update();
}

int FlyingEnemy::Update()
{
	Control();
	Creature::Update();
	speed_y/=2;
	return 0;
}

int Player::Update()
{
	Control();
		Push(0,0.04);
	
	if (Direction & 0x01)
		if (Standing)
		{
			Standing = false;
			Ascend();
		}
	if (Direction & 0x02)
		WalkLeft();
	else if (Direction & 0x04)
		WalkRight();

	if (Direction & 0x08)
		Descend();
	return Creature::Update();
}

void GroundEnemy::Control()
{
	if (Collided)
	{
		if (Direction & 0x02)
			Direction += 2;
		else
			Direction -= 2;
	}
}

void GroundEnemy::Ascend()
{
	Push(0,-.45);
}

void Player::Ascend()
{
	Push(0,-.45);
}

void Creature::Ascend()
{
			Push(0, -movespeed);
}

void Creature::WalkLeft()
{
	FacingRight = false;
	Push(-movespeed, 0);
}

void Creature::WalkRight()
{
	FacingRight = true;
	Push(movespeed, 0);
}

void Creature::Descend(){
		Push(0, movespeed);
}

void Creature::Dash(){
	Push(-speed_x + (-1+2*FacingRight)*5*movespeed,
		-speed_y);
}

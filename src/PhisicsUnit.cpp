#include "Edengine.h"

short Creature::Wall_Collision(bool Horizontal,int distance, short block)
{
  
if (Horizontal)
  {
	if(speed_x > 0)
		pos_x = int(pos_x + speed_x - 0.026) + 1 - mysize / 2;
	else
    		pos_x = int(pos_x + speed_x + 0.026) + mysize / 2;
		Collided = true;
  }
  else
  {
    if (speed_y > 0)
    {
      pos_y = int(pos_y+distance+speed_y-int(speed_y)+mysize/2)-mysize/2;
      Standing = true;
    }
    else
      pos_y = int(pos_y + distance + speed_y) + mysize / 2;
  }
  Stop(Horizontal);
  if (block >= LAVA)
  {
    return 1;
  }
  return 0;
}

char Player::Entity_Collision(Entity* Thing)
{
  if (Thing->get_Type() == COLLECTABLE)
  {
    delete Thing;
    Thing = 0;
    return 1;
  }
  if (Thing->get_Type() == DOOR)
  {
	Thing->Interact(this);
	map = ((Door*)Thing)->get_tMap();
	pos_x = ((Door*)Thing)->get_tX();
	pos_y = ((Door*)Thing)->get_tY();
	return 3;
  }
  if (Thing->get_Type() == ENEMY)
  {
    if (speed_y > Thing->get_Speed_Y())
    {
      delete Thing;
      Thing = NULL;
      Push(0, -speed_y - 0.25);
      SetInvincible(5);
      return 1;
    }
    else return 2*(Invincible<1);
  }
  return 0;
}

void Creature::Stop(bool H)
{
	if(H) speed_x = 0;
	else	speed_y = 0;
}

#include "Edengine.h"

void TextureUnit::Draw(Camera* Main_Camera, unsigned time, int dtime)
{
  SDL_Rect E_Rect;
  SDL_Rect Txtr_Rect;
  unsigned short Frame;
  unsigned short tsize = Main_Camera->get_TextureSize();
  ///Setting the Position and size of the Entity
  E_Rect = {int(Main_Camera->get_Width()/2+
			(pos_x-Main_Camera->get_X()-mysize/2) * Main_Camera->get_zoom()),
  int(Main_Camera->get_Height()/2+
			(pos_y-Main_Camera->get_Y()-mysize/2)*Main_Camera->get_zoom()),
	int(Main_Camera->get_zoom()*mysize),int(Main_Camera->get_zoom()*mysize)};
  if (Animation.Anim_Front != -1)
  {
    if (speed_x == 0 and (speed_y < 0.1 and speed_y > -0.1))
      Frame = Animation.Moves[ANIM_IDLE].Move_Front +
				(time * dtime % Animation.Anim_Speed) /
				(Animation.Anim_Speed / Animation.Moves[ANIM_IDLE].FrameNumber);
    else if (speed_y < -0.1 or speed_y > 0.1)
      Frame = Animation.Moves[ANIM_JUMP].Move_Front +
				(Animation.Moves[ANIM_JUMP].FrameNumber == 2) * (speed_y > 0);
    else
      Frame = Animation.Moves[ANIM_WALK].Move_Front +
				(time * dtime % Animation.Anim_Speed) /
				(Animation.Anim_Speed / Animation.Moves[ANIM_WALK].FrameNumber);
    
		Txtr_Rect = {(Animation.Anim_Front + Frame) % 8 * tsize,
    (Animation.Anim_Front + Frame) / 8 * tsize,tsize,tsize};
    SDL_RenderCopyEx(Main_Camera->get_renderer(),
			Main_Camera->get_Tileset(), &Txtr_Rect, &E_Rect, 0, 0,
			(!FacingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
  }
  else
  {
    SDL_SetRenderDrawColor(
			Main_Camera->get_renderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(Main_Camera->get_renderer(), &E_Rect);
  }
  if (Invincible > 0)
  {
    E_Rect.y = int(Main_Camera->get_Height() / 2 + 
			(pos_y + 0.25 - Main_Camera->get_Y() - mysize / 2) *
			Main_Camera->get_zoom());
    Txtr_Rect.x = 30 % 8 * tsize;
    Txtr_Rect.y = 30 / 8 * tsize;
    SDL_RenderCopyEx(Main_Camera->get_renderer(),
			Main_Camera->get_Tileset(), &Txtr_Rect, &E_Rect, 0, 0,
			 SDL_FLIP_NONE);
  }
}

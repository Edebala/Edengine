#include "Edengine.h"

void TextureUnit::Draw(Camera* Cam, unsigned time){
  unsigned short Frame,  tsize = Cam->get_TextureSize();
  SDL_Rect Txtr_Rect, E_Rect = {
		Cam->getWidth()/2 + (pos_x-Cam->getX()-mysize/2) * Cam->getZoom(),
  	Cam->getHeight()/2 +(pos_y-Cam->getY()-mysize/2) * Cam->getZoom(),
		Cam->getZoom()*mysize,Cam->getZoom()*mysize};
  if (Anim.Front != -1){
    if (speed_x == 0 and abs(speed_y)<0.1)
      Frame = Anim.Moves[IDLE].Front + (time % Anim.Speed)/
			(Anim.Speed / Anim.Moves[IDLE].FrameNumber);
    else if (abs(speed_y)>0.1)
      Frame = Anim.Moves[JUMP].Front +
				(Anim.Moves[JUMP].FrameNumber == 2) * (speed_y > 0);
    else
      Frame = Anim.Moves[WALK].Front + (time % Anim.Speed) /
				(Anim.Speed / Anim.Moves[WALK].FrameNumber);
    
		Txtr_Rect = {(Anim.Front + Frame) % 8 * tsize,
    (Anim.Front + Frame) / 8 * tsize,tsize,tsize};
    SDL_RenderCopyEx(Cam->getRenderer(),
			Cam->get_Tileset(), &Txtr_Rect, &E_Rect, 0, 0,
			(!FacingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
  }
  else{
    SDL_SetRenderDrawColor(
			Cam->getRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(Cam->getRenderer(), &E_Rect);
  }
  if (Invincible > 0){
    E_Rect.y = Cam->getHeight() / 2 + 
			(pos_y + 0.25 - Cam->getY() - mysize / 2) * Cam->getZoom();
    Txtr_Rect.x = 30 % 8 * tsize;
    Txtr_Rect.y = 30 / 8 * tsize;
    SDL_RenderCopyEx(Cam->getRenderer(),
			Cam->get_Tileset(), &Txtr_Rect, &E_Rect, 0, 0,SDL_FLIP_NONE);
  }
}

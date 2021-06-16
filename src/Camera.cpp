#include "Edengine.h"

void Camera::CreateCanvas
(string Title, int get_x, int get_y)
{
	if (Window)
		DeleteCanvas();
	Window = SDL_CreateWindow(
		Title.c_str(),SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,get_x,get_y,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(
		Window, -1, SDL_RENDERER_ACCELERATED);
}

void Camera::DeleteCanvas(){
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
}

void Camera::Follow_Target(){
  float n_x,n_y;
  n_x = (zoom * Target->getX() > WIDT/2)?
		Target->getX(): (float)WIDT/2/zoom;
  n_y = (zoom * Target->getY() > HEIT / 2)?
		Target->getY():(float)HEIT/2/zoom;
  Move(n_x,n_y);
}

Camera::Camera(string Tset,int x = 1200, int y = 1200){
	CreateCanvas("Game", x, y);
	ifstream be("Tilemaps/" + Tset + ".txt");
	be >> texture_size;
	string a ="Tilemaps/" + Tset + ".png";
	SDL_Surface *srfc = IMG_Load(a.c_str());
	SDL_SetColorKey(
		srfc,SDL_TRUE, SDL_MapRGB(
			srfc->format,255,255,255));

	Tileset = SDL_CreateTextureFromSurface(Renderer,srfc);
	SDL_FreeSurface(srfc);

	zoom = 48;
	WIDT = x;
	HEIT = y;
	Interface = new HUD(Renderer,this);
	Interface->SetPt(0);
}

int Camera::Update(){
	Follow_Target();
	SDL_GetWindowSize(Window, &WIDT, &HEIT);
}

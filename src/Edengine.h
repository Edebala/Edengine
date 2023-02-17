#pragma once
#include <algorithm>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <ctime>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#undef main

using namespace std;

struct chunk{ 
	int x, y;
	short** d;
};

struct Ede_move{
	unsigned short FrameNumber;
	unsigned short Front;
};

struct Ede_animation{
	Ede_move* Moves;
	short Front;
	unsigned int Speed;
};

enum AnimType
	{IDLE, WALK, JUMP };
enum Blocks
	{AIR, BRICK, DIRT, GRASS, LAVA,
	SPIKES, COIN, LENGHT };
enum EntityType
	{COLLECTABLE, ENEMY, PLAYER, DOOR, CAMERA, LIGHT};

class World;
class Camera;
class Entity;
class HUD;
class Door;
class PhisicsUnit;
class FlyingEnemy;
class GroundEnemy;
class Light;

#include "World.h"
#include "Entity.h"

class Camera : public Entity{
protected:
	int WIDT, HEIT;
	short zoom;
	SDL_Window* Window;
	Entity* Target;
	SDL_Texture* Tileset;
	int texture_size;
public:
	SDL_Renderer* Renderer;
	EntityType get_Type(){return CAMERA;}
	HUD* Interface;
	void Set_Target(Entity* nt) { Target = nt;}
	void Follow_Target();
	int get_TextureSize() { return texture_size; }
	Camera(string, int, int);
	SDL_Texture* get_Tileset() { return Tileset; }
	double getWidth() { return WIDT; }
	double getHeight() { return HEIT; }
	double getZoom() { return zoom; }
	void set_zoom(int value) { zoom = value; }
	SDL_Renderer* getRenderer() { return Renderer; }
	void CreateCanvas(string, int, int);
	void DeleteCanvas();
	int Update();
	HUD* GetHUD(){return Interface;}
};

class HUD{
	unsigned HudSize;
	TTF_Font* Font;
	SDL_Renderer* Renderer;
	unsigned short Point;
	SDL_Texture* PointTexture;
	Camera *Cam;
public:
	inline unsigned short GetPoint() { return Point; }
	inline void AddPt(int a) { Point += a; PointUpdate(); }
	inline void SetPt(int a) { Point = a; PointUpdate(); }
	void DrawInGame();
	void PointUpdate();
	HUD(SDL_Renderer*,Camera*);
	void ShowPoints();
	Camera* GetCamera(){return Cam;}
};

inline bool exists_test(const char* name) {
	std::ifstream f(name);
	return f.good();
}

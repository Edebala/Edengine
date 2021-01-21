#pragma once
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

struct chunk
{ 
	int x, y;
	short** d;
};

struct Ede_move
{
	unsigned short FrameNumber;
	unsigned short Move_Front;
};

struct Ede_animation
{
	Ede_move* Moves;
	short Anim_Front;
	unsigned int Anim_Speed;
};

enum AnimationType
	{ANIM_IDLE, ANIM_WALK, ANIM_JUMP };
enum Blocks
	{AIR, BRICK, DIRT, GRASS, LAVA,
	SPIKES, COIN, LENGHT };
enum EntityType
	{COLLECTABLE, ENEMY, PLAYER, DOOR, CAMERA};

class World;
class Camera;
class Entity;
class HUD;
class Door;
class PhisicsUnit;
class FlyingEnemy;
class GroundEnemy;

class World
{
	int PlayerIterator;
	long chunksize;
	std::vector <chunk*> Chunks;
	std::vector <Entity*> Entities;
	SDL_Event event;
	unsigned time;
	unsigned FrameRate;
	SDL_Texture* Map;
	short m_type;
public:
	Camera* Cam;
private:
	int FindChunk(int x, int y);
	void AddChunk(int x, int y, short** m);
	short GetBlock(int x, int y);
	void SetBlock(int x, int y);
	void Get_Input();
	int Update_Entities(string& NextMap);
	int EntityWallHCollision(int i);
	int EntityWallVCollision(int i);
	int EntityEntityCollision(int i, int j);
public:
	short Game(int &s_x, int &s_y,string& NextMap);
	void LoadMap(string Source_Folder);
	void LoadDoors(string Source_Folder);
	World(const string Source_Folder);
	~World();
	void DrawMapByBlock(bool Background = false);
	void DrawMapTexture();
	void DrawEntities();
	void DrawChunk(chunk*, bool Background=false);
	short** GetChunk(int x, int y);
	void Draw();
	bool SetTileset(string);
	void SpawnEntities();
};

class Entity
{
private:
protected:
	bool FacingRight;
	double pos_x, pos_y, speed_x, speed_y;
	double mysize;
public:
	Entity();
	Entity(double, double);
	virtual ~Entity();
	void Move(double x, double y){pos_x = x; pos_y = y;}
	void Push(double x, double y) {speed_x+=x;speed_y+= y;}
	virtual int Update(const unsigned char* key){}
	double inline get_X() { return pos_x; }
	double inline get_Y() { return pos_y; }
	double inline get_Speed_X() { return speed_x; }
	double inline get_Speed_Y() { return speed_y; }
	double inline get_size() { return mysize; }
	virtual EntityType get_Type() = 0;
	virtual	int Update();
	virtual void Draw(Camera*,unsigned,int){}
	virtual void Interact(Entity*){}
	virtual char Entity_Collision(Entity*){}
	virtual short Wall_Collision(
		bool Hrz,int dist,short block){return 0;}
};

class IUnit : public Entity
{
public:
	IUnit(double x,double y):Entity(x,y){}
	int Update();
protected:
	short Invincible = 0;
	bool Collided;
	short IsInvincible() { return Invincible > 0; }
	void SetInvincible(short n) { Invincible = n; }
};

class Camera : public Entity
{
	public:
	int WIDT, HEIT;
protected:
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
	double get_Width() { return WIDT; }
	double get_Height() { return HEIT; }
	double get_zoom() { return zoom; }
	void set_zoom(int value) { zoom = value; }
	SDL_Renderer* get_renderer() { return Renderer; }
	void CreateCanvas(string, int, int);
	void DeleteCanvas();
	int Update();
	HUD* GetHUD(){return Interface;}
};
class TextureUnit : public IUnit
{
protected:
	Ede_animation Animation;
public:
	void Draw(Camera*, unsigned, int);
	TextureUnit(double x, double y):IUnit(x,y){};
};

class PhisicsUnit
{
protected:
	PhisicsUnit(){Standing = false;}
	char Entity_Collision(Entity*);
	short Wall_Collision(bool,int, short);
	virtual void Stop(bool H){}
	bool Standing;
public:
	bool isStanding() { return Standing; }
};

class ControlUnit
{
protected:
	unsigned char Direction;
};

class Collectable : public TextureUnit
{
public:
	EntityType get_Type(){return COLLECTABLE;}
	Collectable(double x,double y):TextureUnit(x,y){}
};

class BotController : public ControlUnit
{
	protected:
	virtual void Control() = 0;	
};

class Creature : public TextureUnit, public PhisicsUnit
{
protected:
	double movespeed;
	Creature(double x, double y):TextureUnit(x,y){}
	virtual void Ascend();
	virtual void WalkLeft();
	virtual void WalkRight();
	virtual void Descend();
	virtual void Dash();
	virtual void Attack(){};
	void Stop(bool);
	virtual int Update();
public:
	short Wall_Collision(bool,int,short);
};

class GroundEnemy : public Creature , public BotController
{
public:
	EntityType get_Type(){return ENEMY;}
	GroundEnemy(double x,double y):Creature(x,y){}
	int Update();
	void Control();
	void Ascend();
};

class FlyingEnemy : public Creature, public BotController
{
public:
	EntityType get_Type(){return ENEMY;}
	FlyingEnemy(double x,double y):Creature(x,y){}
	virtual int Update();
	void Control(){}
};

class Player : public Creature, public ControlUnit
{
	double sx,sy;
	string map;
	const unsigned char* key;
	char Entity_Collision(Entity*);
public:
	int Update();
	double getSx(){return sx;}
	double getSy(){return sy;}
	string getMap(){return map;}
	void Control();
	void Ascend();
		Player(double x, double y):Creature(x,y){
		key = SDL_GetKeyboardState(0);
		Animation.Anim_Front = 10;
		Animation.Moves = new Ede_move[3];
		Animation.Moves[ANIM_IDLE].FrameNumber = 4;
		Animation.Moves[ANIM_IDLE].Move_Front = 0;
		Animation.Moves[ANIM_WALK].FrameNumber = 4;
		Animation.Moves[ANIM_WALK].Move_Front = 4;
		Animation.Moves[ANIM_JUMP].FrameNumber = 2;
		Animation.Moves[ANIM_JUMP].Move_Front = 8;
		Animation.Anim_Speed = 300;
		movespeed = 0.25;
		mysize = 0.9;}
	EntityType get_Type(){return PLAYER;}
friend Door;
};

class Door : public IUnit
{
	unsigned m_tX, m_tY;
	string m_targetMap;
public:
	void Interact (Player* T){
		T->Move(m_tX,m_tY);
		cout<<m_tX<<endl;
		T->map = m_targetMap;}
	double get_tX() { return m_tX; }
	double get_tY() { return m_tY; }
	string get_tMap(){return m_targetMap;}
 	EntityType get_Type(){return DOOR;}	 
	Door(double x, double y, string tMap, int tX,int tY)
	:IUnit(x,y){
		m_tX = tX;
		m_tY = tY;
		m_targetMap = tMap;
	}
friend World;
};

class Coin : public Collectable
{
public:
	Coin(double x,double y): Collectable(x,y){
		Animation.Anim_Front = 6;
		Animation.Moves = new Ede_move[3];
		Animation.Moves[ANIM_IDLE].FrameNumber = 4;
		Animation.Moves[ANIM_IDLE].Move_Front = 0;
		Animation.Moves[ANIM_WALK].FrameNumber = 0;
		Animation.Moves[ANIM_WALK].Move_Front = 0;
		Animation.Moves[ANIM_JUMP].FrameNumber = 0;
		Animation.Moves[ANIM_JUMP].Move_Front = 0;
		Animation.Anim_Speed = 250;
		mysize = 0.8;
			
	}
};

class Slime : public GroundEnemy
{
public:
	Slime(double x,double y): GroundEnemy(x,y){
		Animation.Anim_Front = 31;
		Animation.Moves = new Ede_move[3];
		Animation.Moves[ANIM_IDLE].FrameNumber = 1;
		Animation.Moves[ANIM_IDLE].Move_Front = 0;
		Animation.Moves[ANIM_WALK].FrameNumber = 1;
		Animation.Moves[ANIM_WALK].Move_Front = 0;
		Animation.Moves[ANIM_JUMP].FrameNumber = 2;
		Animation.Moves[ANIM_JUMP].Move_Front = 1;
		Animation.Anim_Speed = 300;
		Direction = 5;
		movespeed = 0.09;
		mysize = 0.9;
			
	}
};

class RedGuy : public GroundEnemy
{
public:
	RedGuy(double x,double y): GroundEnemy(x,y){
		Animation.Anim_Front = 20;
		Animation.Moves = new Ede_move[3];
		Animation.Moves[ANIM_IDLE].FrameNumber = 4;
		Animation.Moves[ANIM_IDLE].Move_Front = 0;
		Animation.Moves[ANIM_WALK].FrameNumber = 4;
		Animation.Moves[ANIM_WALK].Move_Front = 4;
		Animation.Moves[ANIM_JUMP].FrameNumber = 2;
		Animation.Moves[ANIM_JUMP].Move_Front = 8;
		Animation.Anim_Speed = 300;
		Direction = 4;
		movespeed = 0.09;
		mysize = 0.9;
	}
};

class Copter: public FlyingEnemy
{
public:
	Copter(double x,double y): FlyingEnemy(x,y){
		Animation.Anim_Front = 44;
		Animation.Moves = new Ede_move[3];
		Animation.Moves[ANIM_IDLE].FrameNumber = 4;
		Animation.Moves[ANIM_IDLE].Move_Front = 0;
		Animation.Moves[ANIM_WALK].FrameNumber = 1;
		Animation.Moves[ANIM_WALK].Move_Front = 0;
		Animation.Moves[ANIM_JUMP].FrameNumber = 1;
		Animation.Moves[ANIM_JUMP].Move_Front = 8;
		Animation.Anim_Speed = 100;
		Direction = 0;
		movespeed = 0;
		mysize = 0.9;	
	}
};

class HUD
{
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

inline bool exists_test(string name) {
	std::ifstream f(name);
	return f.good();

}

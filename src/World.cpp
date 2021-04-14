#include "Edengine.h"
 
void World::SpawnEntities()
{
	short block;
	for (chunk *c: Chunks)
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++)
	switch(c->d[i][j]-COIN)
	{
	case 0:
		Entities.push_back(new Coin(
			.5+c->x * chunksize + i,
			.5+c->y * chunksize+j));
		break;
	case 1:
		Entities.push_back(new RedGuy(
			.5+c->x * chunksize + i,
			.5+c->y * chunksize+j));
		break;
	case 2:
		Entities.push_back(new Slime(
			.5+c->x * chunksize + i,
			.5+c->y * chunksize+j));
		break;
	case 4:
		Entities.push_back(new Copter(
			.5+c->x * chunksize + i,
			.5+c->y * chunksize+j));
		break;
	}
	printf("Entities Spawned Successfully!\n");
}

World::World(string Source_Folder)
{
	SDL_Surface* surf;
	Cam = new Camera("Cave4", 600, 600);
	LoadMap(Source_Folder);
}

World::~World()
{
	for (unsigned short i = 0; i < Chunks.size(); i++)
	{
		for (unsigned short j = 0; j < chunksize; j++)
			delete[]Chunks[Chunks.size() - 1]->d[j];
		delete[]Chunks[Chunks.size() - 1]->d;
		Chunks.pop_back();
	}
}

void World::Draw()
{
  SDL_SetRenderTarget(
		Cam->get_renderer(),Map);
  SDL_SetRenderDrawColor(
		Cam->get_renderer(), 20, 20, 20, 0);
	SDL_RenderClear(Cam->get_renderer());
	Cam->Update();
	DrawMapByBlock(true);
	for (Entity* e:Entities)
		e->Draw(Cam, time, FrameRate);

	Cam->Interface->DrawInGame();
	SDL_RenderPresent(Cam->get_renderer());
  SDL_Rect r{0,0,Cam->get_Width(),
	Cam->get_Height()};
}

void World::DrawMapTexture()
{
	SDL_Rect Maprect = {
		Cam->get_Width() / 2 -
			Cam->get_X() * Cam->get_zoom(),
	Cam->get_Height() / 2 - 
			Cam->get_Y() * Cam->get_zoom(),
	128 * Cam->get_zoom(),
	128 * Cam->get_zoom()};
	SDL_RenderCopy(Cam->get_renderer(),
		Map, NULL, &Maprect);
}


void World::DrawMapByBlock(bool Background)
{
	for (chunk *c:Chunks)
	if(((c->x + 1)*chunksize >
		Cam->get_X() - Cam->get_Width()/Cam->get_zoom())&&
			(c->x * chunksize <
		Cam->get_X() + Cam->get_Width()/Cam->get_zoom())&&
			((c->y + 1)*chunksize >
		Cam->get_Y() -Cam->get_Height()/Cam->get_zoom())&&
			(c->y * chunksize <
		Cam->get_Y()+ Cam->get_Height()/Cam->get_zoom()))
			DrawChunk(c,Background);
}

void World::DrawChunk(chunk* Chunk,bool Background)
{
	SDL_Rect BR;
	SDL_Rect Txtr_Rect;
	for (long i = 0; i < chunksize; i++)
	if((i + Chunk->x * chunksize >
		Cam->get_X() - Cam->get_Width()/Cam->get_zoom())&&
			(i + Chunk->x * chunksize <
		Cam->get_X() + Cam->get_Width()/Cam->get_zoom()))
		for (unsigned j = 0; j < chunksize; j++)
	 		if((j + Chunk->y *chunksize >
				Cam->get_Y() - Cam->get_Height()/Cam->get_zoom()) &&
					(j + Chunk->y * chunksize <
					 	Cam->get_Y() + Cam->get_Height() / Cam->get_zoom()) &&
						((Chunk->d[i][j]>0 && Chunk->d[i][j]<COIN) ||
						(Chunk->d[i][j]<=0 || Chunk->d[i][j]>=COIN) && Background))
				{

					BR.x = Cam->get_Width() / 2 +
						(chunksize * Chunk->x + i - Cam->get_X()) * Cam->get_zoom();
					BR.y = Cam->get_Height() / 2 +
						(chunksize * Chunk->y + j - Cam->get_Y()) * Cam->get_zoom();
					BR.w = Cam->get_zoom();
					BR.h = BR.w;
					Txtr_Rect.x = Cam->get_TextureSize() * Chunk->d[i][j];

					Txtr_Rect.w = Cam->get_TextureSize();
					Txtr_Rect.h = Txtr_Rect.w;
					Txtr_Rect.x = (Chunk->d[i][j] < COIN ? Chunk->d[i][j] : 0)
						% 8 * Cam->get_TextureSize();
					Txtr_Rect.y = (Chunk->d[i][j] < COIN ? Chunk->d[i][j] : 0)
						/ 8 * Cam->get_TextureSize();

					SDL_RenderCopy(
						Cam->Renderer, Cam->get_Tileset(),
						&Txtr_Rect, &BR);
				}
}

void World::Get_Input()
{
	SDL_PollEvent(&event);
}

int World::Update_Entities(string& NextMap)
{
	int a;
	for (Entity *i: Entities)
	{
	int result = i->Update();
	for (Entity *j: Entities){
		a = EntityEntityCollision(i,j);
		if(i->get_Type() == PLAYER and a > 1)
			return a;
	}

	if(EntityWallVCollision(i) || EntityWallHCollision(i))
		if(i->get_Type()) 
			return 1;
		else
			Entities.erase(std::find(Entities.begin(),Entities.end(),i));
	return 0;
}

int World::EntityEntityCollision(Entity *i, Entity *j)
{
	int a;
	if (i == j) return 0;
	if (abs(i->get_X() - j->get_X()) <
		i->get_size() / 2 + j->get_size() * 2 / 5)
		for (int k = 0; k <= abs(i->get_Speed_Y()); k++)
		{
	if(abs(k + (i->get_Y()) - j->get_Y()) <
				i->get_size() / 2 + j->get_size() * 2 / 5)
			{
				a = i->Entity_Collision(j);
				if (a == 1)
				{
					Cam->Interface->AddPt(1);
					Entities.erase(std::find(Entities.begin(),Entities.end(),j));
				}
				else if (a == 2)
						Cam->Interface->SetPt(0);
				if(a) 
					return a;
			}
		}
	return 0;
}

int World::EntityWallHCollision(Entity* i)
{
	int Cbl = GetBlock((i->get_X() + i->get_Speed_X() +
		(-1 + 2 *(i->get_Speed_X() > 0))*i->get_size()  * 0.35),
		i->get_Y() - i->get_size() / 2 + .025);
	
	if(Cbl < 1 || Cbl >= COIN)
 		Cbl = GetBlock((i->get_X() + i->get_Speed_X() +
		(-1+2*(i->get_Speed_X() > 0))*i->get_size() *0.35),
		i->get_Y() + i->get_size() / 2 - .025);

	if (Cbl > 0 && Cbl < COIN)
		if(i->Wall_Collision(1, 0, Cbl))
			return 1;
	return 0;
}

int World::EntityWallVCollision(Entity* i)
{
	double s = i->get_Speed_Y()-
		(int)i->get_Speed_Y();

	for(int j=0;j < abs(i->get_Speed_Y() -
				i->get_size()); j++)
	{
		auto Cbl=GetBlock((i->get_X() -
				i->get_size() / 2 + .025),
			i->get_Y() + j + s +
				(-1 + 2 * (i->get_Speed_Y() > 0))*
				i->get_size() / 2);

	if(Cbl < 1 || Cbl >= COIN)
	{
		Cbl = GetBlock((i->get_X() +
				i->get_size() / 2 - .025),
			i->get_Y() + j + s +
 			(-1 + 2 * (i->get_Speed_Y() > 0))*
				i->get_size() / 2);
	}

	if (Cbl > 0 && Cbl < COIN)
			if (i->Wall_Collision(0, j, Cbl))
					return 1;
			else break;
	}
	return 0;
}

short World::Game(int &s_x, int &s_y,string& NextMap)
{
	int a;
	event.type = SDL_FIRSTEVENT;
	FrameRate = 30;
	time = 0;
	LoadDoors(NextMap);
	Player* Pl = new Player(s_x, s_y);
	Entities.push_back(Pl);
	PlayerIterator = Entities.size()-1;
	Cam->Set_Target(Entities.back());
	SpawnEntities();
	unsigned BaseTime = clock()*FrameRate/CLOCKS_PER_SEC;
	unsigned CurrentTime = BaseTime;
	unsigned delay = 0, totalDelay = 0;
	while (event.type != SDL_QUIT)
	{
		if (((totalDelay + clock())*FrameRate)/CLOCKS_PER_SEC < CurrentTime)
		{
 			delay = ((CurrentTime)* CLOCKS_PER_SEC /FrameRate - (totalDelay+clock()));
			totalDelay += delay;
			usleep(delay);
		}

		time++;
		CurrentTime = (time + BaseTime);
		Draw();
		Get_Input();
		a = Update_Entities(NextMap);
		if (a && a<3)
		{
			Entities.clear();
			return 2;
		}
		if (a == 3)
		{
			s_x = Entities[PlayerIterator]->get_X();
			s_y = Entities[PlayerIterator]->get_Y();
			NextMap = ((Player*)Entities[PlayerIterator])->getMap();
			
			Entities.clear();
			return 1;
		}
	}
	if (event.type == SDL_QUIT) 
	{
		Entities.clear();
		return 0;
	}
return -1;
}

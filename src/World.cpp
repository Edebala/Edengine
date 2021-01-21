#include "Edengine.h"
 
void World::SpawnEntities()
{
	short block;
	for (unsigned short i = 0; i < Chunks.size(); i++)
		for (short j = 0; j < chunksize; j++)
			for (short k = 0; k < chunksize; k++)
			{
				if (Chunks[i]->d[j][k] >= COIN)
				{
	block = Chunks[i]->d[j][k];
	if (block - COIN == 0)
		Entities.push_back(new Coin(
			.5+Chunks[i]->x * chunksize + j,
			.5+Chunks[i]->y * chunksize+k));

	if (block - COIN == 1)
		Entities.push_back(new RedGuy(
			.5+Chunks[i]->x * chunksize + j,
			.5+Chunks[i]->y * chunksize+k));
	
	if (block - COIN == 2)
		Entities.push_back(new Slime(
			.5+Chunks[i]->x * chunksize + j,
			.5+Chunks[i]->y * chunksize+k));
	
	if (block - COIN == 4)
		Entities.push_back(new Copter(
			.5+Chunks[i]->x * chunksize + j,
			.5+Chunks[i]->y * chunksize+k));
				}
			}
	cout<<"Entities Spawned Successfully!\n";
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
	for (unsigned i = 0; i < Entities.size(); i++)
		Entities[i]->Draw(Cam, time, FrameRate);

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
	for (int i = 0; i < Chunks.size(); i++)
	if(((Chunks[i]->x + 1)*chunksize >
		Cam->get_X() - Cam->get_Width()/Cam->get_zoom())&&
			(Chunks[i]->x * chunksize <
		Cam->get_X() + Cam->get_Width()/Cam->get_zoom())&&
			((Chunks[i]->y + 1)*chunksize >
		Cam->get_Y() -Cam->get_Height()/Cam->get_zoom())&&
			(Chunks[i]->y * chunksize <
		Cam->get_Y()+ Cam->get_Height()/Cam->get_zoom()))
			DrawChunk(Chunks[i],Background);
}

void World::DrawChunk(chunk* Chunk,bool Background)
{
	SDL_Rect Block_Rect;
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

					Block_Rect.x = int(Cam->get_Width() / 2 +
						(chunksize * Chunk->x + i - Cam->get_X()) * Cam->get_zoom());
					Block_Rect.y = int(Cam->get_Height() / 2 +
						(chunksize * Chunk->y + j - Cam->get_Y()) * Cam->get_zoom());
					Block_Rect.w = int(Cam->get_zoom());
					Block_Rect.h = int(Cam->get_zoom());
					Txtr_Rect.x = Cam->get_TextureSize() * Chunk->d[i][j];

					Txtr_Rect.w = Cam->get_TextureSize();
					Txtr_Rect.h = Cam->get_TextureSize();
					Txtr_Rect.x = (Chunk->d[i][j] < COIN ? Chunk->d[i][j] : 0)
						% 8 * Cam->get_TextureSize();
					Txtr_Rect.y = (Chunk->d[i][j] < COIN ? Chunk->d[i][j] : 0)
						/ 8 * Cam->get_TextureSize();

					SDL_RenderCopy(
						Cam->Renderer, Cam->get_Tileset(),
						&Txtr_Rect, &Block_Rect);
				}
}

void World::Get_Input()
{
	SDL_PollEvent(&event);
}

int World::Update_Entities(string& NextMap)
{
	int a;
	for (unsigned i = 0; i < Entities.size(); i++)
	{
	 
	int result = Entities[i]->Update();

	for (unsigned short j = 0; j < Entities.size(); j++)
	{
		a = EntityEntityCollision(i, j);
		if(Entities[i]->get_Type() == PLAYER and a > 1)
			return a;
	}

	if(EntityWallVCollision(i))
				if(Entities[i]->get_Type()) 
					return 1;
				else
					Entities.erase(Entities.begin()+i);

		if (EntityWallHCollision(i))
			if (Entities[i]->get_Type() == PLAYER)
				return 1;
			else
				Entities.erase(Entities.begin() + i);

	}
	return 0;
}

int World::EntityEntityCollision(int i, int j)
{

	int a;
	if (i == j) return 0;
	if (abs(Entities[i]->get_X() - Entities[j]->get_X()) <
		Entities[i]->get_size() / 2 + Entities[j]->get_size() * 2 / 5)
		for (int k = 0; k <= abs(Entities[i]->get_Speed_Y()); k++)
		{
	if(abs(k + (Entities[i]->get_Y()) - Entities[j]->get_Y()) <
				Entities[i]->get_size() / 2 + Entities[j]->get_size() * 2 / 5)
			{
				a = Entities[i]->Entity_Collision(Entities[j]);
				if (a == 1)
				{
					Cam->Interface->AddPt(1);
					Entities.erase(Entities.begin() + j);
				}
				else if (a == 2)
						Cam->Interface->SetPt(0);
				if(a) 
	return a;
			}
		}
	return 0;
}

int World::EntityWallHCollision(int i)
{
	int Cbl = GetBlock((Entities[i]->get_X() +
		Entities[i]->get_Speed_X() +
		(-1 + 2 *(Entities[i]->get_Speed_X() > 0))*
		Entities[i]->get_size()  * 0.35),
		Entities[i]->get_Y() - Entities[i]->get_size() / 2 + .025);
	
	if(Cbl < 1 || Cbl >= COIN)
 		Cbl = GetBlock((Entities[i]->get_X() +
			Entities[i]->get_Speed_X() +
		(-1+2*(Entities[i]->get_Speed_X() > 0))*
			Entities[i]->get_size() *0.35),
		Entities[i]->get_Y() + Entities[i]->get_size() / 2 - .025);

	if (Cbl > 0 && Cbl < COIN)
		if(Entities[i]->Wall_Collision(1, 0, Cbl))
			return 1;
	return 0;
}

int World::EntityWallVCollision(int i)
{
	double s = Entities[i]->get_Speed_Y()-
		(int)Entities[i]->get_Speed_Y();

	for(int j=0;j < abs(Entities[i]->get_Speed_Y() -
				Entities[i]->get_size()); j++)
	{
		auto Cbl=GetBlock((Entities[i]->get_X() -
				Entities[i]->get_size() / 2 + .025),
			Entities[i]->get_Y() + j + s +
				(-1 + 2 * (Entities[i]->get_Speed_Y() > 0))*
				Entities[i]->get_size() / 2);

	if(Cbl < 1 || Cbl >= COIN)
	{
		Cbl = GetBlock((Entities[i]->get_X() +
				Entities[i]->get_size() / 2 - .025),
			Entities[i]->get_Y() + j + s +
 			(-1 + 2 * (Entities[i]->get_Speed_Y() > 0))*
				Entities[i]->get_size() / 2);
	}

	if (Cbl > 0 && Cbl < COIN)
			if (Entities[i]->Wall_Collision(0, j, Cbl))
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

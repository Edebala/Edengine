#include "Edengine.h"

int max(int a,int b){
	return a>b?a:b;
}

int min(int a,int b){
	return a<b?a:b;
}

void World::SpawnEntities(){
	for (chunk *c: Chunks)
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++){
	double y = .5+c->y*chunksize+j ,x = .5+c->x*chunksize+i;
	switch(c->d[i][j]-COIN){
	case 0:Entities.push_back(new Coin(x,y));break;
	case 1:Entities.push_back(new RedGuy(x,y));break;
	case 2:Entities.push_back(new Slime(x,y));break;
	case 4:Entities.push_back(new Copter(x,y));break;
		}
	}
	printf("Entities Spawned Successfully!\n");
}

World::World(string Source_Folder){
	SDL_Surface* surf;
	Cam = new Camera("Cave4", 900, 900);
	LoadMap(Source_Folder.c_str());
	FrameRate = 30;
}

World::~World(){
	for(chunk *i:Chunks){
		for(short j = 0; j < chunksize; j++)
			delete[]Chunks[0]->d[j];
		delete[]Chunks[0]->d;
		Chunks.erase(Chunks.begin());
	}
}

void World::Draw(){
  SDL_SetRenderTarget(Cam->getRenderer(),Map);
  SDL_SetRenderDrawColor(Cam->getRenderer(), 0, 0, 0, 0);
	SDL_RenderClear(Cam->getRenderer());
	Cam->Update();
	DrawMapByBlock(true);
	for (Entity* e:Entities)
		e->Draw(Cam, time * FrameRate);

	Cam->Interface->DrawInGame();
	SDL_RenderPresent(Cam->getRenderer());
  SDL_Rect r{0,0,Cam->getWidth(),
	Cam->getHeight()};
}

void World::DrawMapTexture(){
	SDL_Rect Maprect = {
		Cam->getWidth() / 2 -Cam->getX() * Cam->getZoom(),
		Cam->getHeight() / 2 - Cam->getY() * Cam->getZoom(),
		128 * Cam->getZoom(),128 * Cam->getZoom()};
	SDL_RenderCopy(Cam->getRenderer(),
		Map, NULL, &Maprect);
}


void World::DrawMapByBlock(bool Background){
	for (chunk *c:Chunks)
		DrawChunk(c,Background);
}

void World::DrawChunk(chunk* c,bool Background){
	SDL_Rect BlockRect,TxtrRect;
	int hRange = Cam->getWidth()/Cam->getZoom(),
		vRange = Cam->getHeight()/Cam->getZoom(),
	  firstX = max(Cam->getX() - hRange - c->x * chunksize,0),
		lastX =  min(Cam->getX() + hRange - c->x * chunksize,chunksize),
		firstY = max(Cam->getY() - vRange - c->y * chunksize,0),
		lastY =  min(Cam->getY() + vRange - c->y * chunksize,chunksize);
	for (int i = firstX; i < lastX; i++)
	for (int j = firstY; j < lastY; j++){
		int xPos = i + c->x * chunksize,
			yPos = j + c->y * chunksize, block = c->d[i][j];
		BlockRect = {
			Cam->getWidth() / 2 + (xPos - Cam->getX()) * Cam->getZoom(),
			Cam->getHeight()/ 2 + (yPos - Cam->getY()) * Cam->getZoom(),
			Cam->getZoom(),Cam->getZoom()};
		TxtrRect = {
			(block < COIN ? block : 0)% 8 * Cam->get_TextureSize(),
			(block < COIN ? block : 0)/ 8 * Cam->get_TextureSize(),
			Cam->get_TextureSize(),Cam->get_TextureSize()};
		int d = pow(abs(Cam->getX() - c->x*chunksize-i),2) + pow(abs(Cam->getY() -  c->y*chunksize-j),2);
		int change = max(255-4*d,0);
			SDL_SetTextureColorMod(Cam->get_Tileset(),change,change,change);
		SDL_RenderCopy(
			Cam->Renderer, Cam->get_Tileset(),&TxtrRect, &BlockRect);
			SDL_SetTextureColorMod(Cam->get_Tileset(),255,255,255);
	}
}

int World::Update_Entities(string& NextMap){
	for (Entity *i: Entities){
		int result = i->Update();
		for (Entity *j: Entities)
			if(i != j && i->get_Type() == PLAYER &&
				(result = EntityEntityCollision(i,j)) > 1)
				return result;
	
	if(EntityWallVCollision(i) || EntityWallHCollision(i))
		if(i->get_Type() == PLAYER) 
			return 1;
		else
			Entities.erase(find(Entities.begin(),Entities.end(),i));
	}
	return 0;
}

int World::EntityEntityCollision(Entity *i, Entity *j){
	double halfSizes = (i->getSize() + j->getSize())/2; 	
	if (abs(i->getX() - j->getX()) < halfSizes)
	for (int k = 0; k <= abs(i->getSpeedY()); k++){
		if(abs(k + i->getY() - j->getY()) < halfSizes){
			int a = i->Entity_Collision(j);
			switch(a){
				case 1: Cam->Interface->AddPt(1);
					Entities.erase(find(Entities.begin(),Entities.end(),j));
					break;
				case 2:	Cam->Interface->SetPt(0);
			}
			if(a) return a;
		}
	}
	return 0;
}

int World::EntityWallHCollision(Entity* i){
	int dir = i->getSpeedX()/abs(i->getSpeedX()),
		xBlock = i->getX() + i->getSpeedX() + dir * i->getSize()/2,
		Cbl = GetBlock(xBlock, i->getY() - i->getSize() / 2 + .02);
	if(Cbl < 1 || Cbl >= COIN)
 		Cbl = GetBlock(xBlock, i->getY() + i->getSize() / 2 - .02);
	if(Cbl < 1 || Cbl >= COIN) return 0;
	else return i->Wall_Collision(1, 0, Cbl);
}

int World::EntityWallVCollision(Entity* i){
	double s = i->getSpeedY()-(int)i->getSpeedY();
	int dir = i->getSpeedY()/abs(i->getSpeedY());
	for(int j=0;j < abs(i->getSpeedY()-i->getSize()); j++){
		int yBlock = i->getY() + j + s + dir *i->getSize() / 2;
		int Cbl=GetBlock((i->getX() - i->getSize()/2 + .02),yBlock);
		if(Cbl < 1 || Cbl >= COIN)
			Cbl= GetBlock((i->getX() + i->getSize()/2 - .02), yBlock);
		if(Cbl < 1 || Cbl >= COIN) continue;
		if (i->Wall_Collision(0, j, Cbl)) return 1;
	}
	return 0;
}

short World::Game(int &s_x, int &s_y,string& NextMap){
	event.type = SDL_FIRSTEVENT;
	time = 0;
	LoadDoors(NextMap);
	Entities.push_back(new Player(s_x, s_y));
	PlayerIterator = Entities.size()-1;
	Cam->Set_Target(Entities.back());
	SpawnEntities();
	double T =(double) CLOCKS_PER_SEC / FrameRate;
	unsigned BaseTime = clock() / T;
	unsigned delay = 0, totalDelay = 0;
	while (event.type != SDL_QUIT){
		if ((totalDelay + clock()) / T < time + BaseTime){
 			delay = ((time  +BaseTime)* T - (totalDelay+clock()));
			totalDelay += delay; usleep(delay);
		}
		time++;
		Draw();
		SDL_PollEvent(&event);
		int a = Update_Entities(NextMap);
		if (a && a<3){
			Entities.clear();
			return 2;
		}
		if (a == 3){
			s_x = Entities[PlayerIterator]->getX();
			s_y = Entities[PlayerIterator]->getY();
			NextMap = ((Player*)Entities[PlayerIterator])->getMap();
			
			Entities.clear();
			return 1;
		}
	}
	if (event.type == SDL_QUIT) {
		Entities.clear();
		return 0;
	}
return -1;
}

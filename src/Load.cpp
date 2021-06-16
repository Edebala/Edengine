#include "Edengine.h"

void World::LoadMap(const char* Source_Folder)
{
	Chunks.clear();
	short x, y;
	int DoorNr;
	double Door_x,Door_y,Door_tX, Door_tY;

	if (strlen(Source_Folder)==0){
		chunksize = 8;
		return;
	}

	char* textbfr = (char*) malloc(12+strlen(Source_Folder));
	strcpy(textbfr,"Maps/");
	strcat(textbfr,Source_Folder);
	strcat(textbfr,".CNK");
	ifstream be(textbfr);
	if (!be){
		printf("%s map File was not Found!",textbfr);
		return;	
	}
	printf("%s map File Found!\n",textbfr);
	be >> chunksize;
	be.get();
	short** buffer = new short*[chunksize];

	for (short i = 0; i < chunksize; i++)
		buffer[i] = new short[chunksize];

	while (!be.eof()){
		x = be.get() - '0';
		y = be.get() - '0';
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++){
				buffer[i][j] = be.good()?(be.get() - '0'):1;
				if(buffer[i][j] == LAVA)Lights.push_back(
					new Light(i+x*chunksize,j+y*chunksize,256));
			}
		AddChunk(x, y, buffer);
	}
	printf("Map Loaded Successfully\n");
	be.close();
}

void World::LoadDoors(string Source_Folder)
{
	int DoorNr;
	double Door_x, Door_y, Door_tX, Door_tY;
	string Door_tMap;
	string textbfr = "Maps/" + Source_Folder + ".txt";

	std::ifstream be(textbfr);
	if(!be)return;
	be >> DoorNr;
	for (int i = 0; i < DoorNr; i++)
	{
		be >> Door_x >> Door_y >> Door_tMap >> Door_tX >> Door_tY;
		Entities.push_back(new Door(
			Door_x, Door_y, Door_tMap, Door_tX, Door_tY));
	}
	printf("Doors Summoned Successfully!\n");
}

void World::AddChunk(int x, int y, short** m)
{
	chunk *a = FindChunk(x,y);
	if (a == 0)
	{
		Chunks.push_back(new chunk);
		Chunks.back()->x = x;
		Chunks.back()->y = y;
		Chunks.back()->d = new short *[chunksize];
		a = Chunks.back();
		for (short i = 0; i < chunksize; i++)
			a->d[i] = new short[chunksize];
	}
	if (m != 0)
	{
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++)
				a->d[i][j] = m[i][j];
	}
	else
	{
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++)
				a->d[i][j] = 0;
	}
}

chunk* World::FindChunk(int x, int y)
{
	for (chunk *c: Chunks)
		if (x == c->x and y == c->y)return c;
	return 0;
}

short World::GetBlock(int x, int y){
	chunk* c;
	return ((c= FindChunk(x / chunksize, y / chunksize))==0)?0:
		c-> d[x%chunksize][y%chunksize];
}

short** World::GetChunk(int x, int y){
	chunk* i = FindChunk(x / chunksize, y / chunksize);
	return (i==0)?0:i->d;
}


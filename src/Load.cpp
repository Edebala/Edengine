#include "Edengine.h"

void World::LoadMap(string Source_Folder)
{
	Chunks.clear();
	string textbfr;
	short x, y;
		int DoorNr;
		double Door_x,Door_y,Door_tX, Door_tY;
		char Door_tMap[40];

	if (Source_Folder.empty())
	{
		chunksize = 8;
		return;
	}

		textbfr = "Maps/" + Source_Folder + ".CNK";

	if (exists_test(textbfr))
	{
		cout<<textbfr<<" map File Found!"<<"\n";
		ifstream be(textbfr);
		be >> chunksize;
		be.get();
		short** buffer;
		buffer = new short*[chunksize];
		for (short i = 0; i < chunksize; i++)
			buffer[i] = new short[chunksize];

		while (!be.eof())
		{
			x = be.get() - '0';
			y = be.get() - '0';
			for (short i = 0; i < chunksize; i++)
				for (short j = 0; j < chunksize; j++)
				{
					if (be.good())
						buffer[i][j] = be.get() - '0';
					else
						buffer[i][j] = 1;
				}
			AddChunk(x, y, buffer);
		}
		std::cout<<"Map Loaded Successfully"<<"\n";
		be.close();
	}
	else
	{
			std::cout<<textbfr<<" map File was not Found!"<<"\n";
	}
}

void World::LoadDoors(string Source_Folder)
{
	string textbfr;
	int DoorNr;
	double Door_x, Door_y, Door_tX, Door_tY;
	string Door_tMap;
	std::ifstream be(textbfr);

	textbfr = "Maps/" + Source_Folder + ".txt";

	if (exists_test(textbfr))
	{
		be.open(textbfr);
		be >> DoorNr;
		for (int i = 0; i < DoorNr; i++)
		{
			be >> Door_x >> Door_y >> Door_tMap >> Door_tX >> Door_tY;
			Entities.push_back(new Door(
				Door_x, Door_y, Door_tMap, Door_tX, Door_tY));
		}
	cout<<"Doors Summoned Successfully!"<<endl;
	}
}

void World::AddChunk(int x, int y, short** m)
{
	int a;
		a = FindChunk(x,y);
	if (a == -1)
	{
		Chunks.push_back(new chunk);
		Chunks.back()->x = x;
		Chunks.back()->y = y;
		Chunks.back()->d = new short *[chunksize];
		a = (int)Chunks.size() - 1;
		for (short i = 0; i < chunksize; i++)
			Chunks[a]->d[i] = new short[chunksize];
	}
	if (m != 0)
	{
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++)
				Chunks[a]->d[i][j] = m[i][j];
	}
	else
	{
		for (short i = 0; i < chunksize; i++)
			for (short j = 0; j < chunksize; j++)
				Chunks[a]->d[i][j] = 0;
	}
}

int World::FindChunk(int x, int y)
{
	for (unsigned i = 0; i < Chunks.size(); i++)
		if (x == Chunks[i]->x and y == Chunks[i]->y)
			return i;
	return -1;
}

short World::GetBlock(int x, int y)
{
	int chunk_iterator = FindChunk(
		x / chunksize, y / chunksize);
	if (chunk_iterator == -1) return 0;
	return Chunks[chunk_iterator]->
		d[x%chunksize][y%chunksize];
}

void World::SetBlock(int x, int y)
{
	int chunk_iterator = FindChunk(
		x/chunksize,y/chunksize);
	if (chunk_iterator == -1) return;
	Chunks[chunk_iterator]->
		d[x%chunksize][y%chunksize]++;
	if (Chunks[chunk_iterator]->
		d[x%chunksize][y%chunksize]>1)
	Chunks[chunk_iterator]->
		d[x%chunksize][y%chunksize] = 0;
}

short** World::GetChunk(int x, int y)
{
	int chunk_i = FindChunk(x / chunksize, y / chunksize);
	return (chunk_i == -1)?0:Chunks[chunk_i]->d;
}


#include "Edengine.h"

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

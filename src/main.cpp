#include "Edengine.h"

int main()
{
  int x = 9,y = 24;
  string NextMap = "ASD";
  SDL_Init(0);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  int result = 1;
  World World_Instance(NextMap);
  
  while (result)
  {
    result = World_Instance.Game(x,y,NextMap);
    if (result == 1)
        World_Instance.LoadMap(NextMap);
  }
  return 0;
}

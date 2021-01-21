#include "Edengine.h"

HUD::HUD(SDL_Renderer* NewRenderer,Camera* cam)
{
    if(NewRenderer)
        Renderer = NewRenderer;
    TTF_Init();
    if(!(Font = TTF_OpenFont("ARCADECLASSIC.TTF",32)))std::cout<<TTF_GetError()<<"\n";
    else
      std::cout<<"Font Loaded Successfully\n";
    Point = 0;
    PointUpdate();
    Cam = cam;
}

void HUD::DrawInGame()
{
  ShowPoints();
}

void HUD::ShowPoints()
{
    SDL_Rect CoinRect;
    CoinRect.x = 5;
    CoinRect.y = 5;
    CoinRect.w = ((int)log10(Point)+1)*48;
    CoinRect.h = 64;
    SDL_RenderCopy(Renderer,PointTexture,NULL,&CoinRect);
}
/*
void HUD::LoadText(string source, string message)
{
  SDL_Rect r = {10,60,90,90};  
  TextBoxes.push_back(new TextBox(r,message,source));
}

void HUD::ShowText()
{
  int ScreenWidth,ScreenHeight;
}*/

void HUD::PointUpdate()
{
    SDL_Surface* surf;
    SDL_Color WHITE = {255,255,0};
    std::stringstream a;
    a<<Point;
    if (Font)
    {
      if(!(surf = TTF_RenderText_Solid(Font,a.str().c_str(),WHITE))) std::cout<<TTF_GetError()<<'\n';
    PointTexture = SDL_CreateTextureFromSurface(Renderer,surf);
    SDL_FreeSurface(surf);
    }
}

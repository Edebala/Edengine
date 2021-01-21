#include "Edengine.h"
/*
TextBox :: TextBox(SDL_Rect newRect,string newMessage,string newSpeaker)
{
    SetContent(newSpeaker,newMessage);
    SetRect(TextRect);
}

void TextBox :: SetRect(SDL_Rect newRect)
{
    TextRect = newRect;
}

void TextBox :: SetContent(string newSpeaker,string newMessage)
{
    Message = newMessage;
    Speaker = newSpeaker;
}

void TextBox :: Show(TTF_Font* Font)
{
    SDL_Surface* surf;
    SDL_TextTexture* = TextTexture;
    SDL_Rect RealTextRect = {TextRect.x* MainCamera.getWidth()/100,
                            TextRect.y * MainCamera.getHeight()/100,
                            TextRect.w * MainCamera.getWidth()/100,
                            TextRect.h * MainCamera.getHeight()/100};

    SDL_Color WHITE = {255,255,0};
    stringstream a;
    a<<Message;
      if(!(surf = TTF_RenderText_Solid(Font,a.str().c_str(),WHITE))) std::cout<<TTF_GetError()<<'\n';
        TextTexture = SDL_CreateTextureFromSurface(renderer,surf);
    SDL_RenderCopy(renderer,TextTexture,NULL,&RealTextRect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(TextTexture);
}*/
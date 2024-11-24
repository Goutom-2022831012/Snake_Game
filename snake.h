#pragma once
#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
using namespace std;
const int SNAKE_SIZE =20;
enum direction {UP,DOWN,LEFT,RIGHT};
struct snakeSegment{
    int x;
    int y;
};
//function declaration
void renderSnake(SDL_Renderer* renderer,const vector<snakeSegment>&snake,SDL_Texture* headTexture);
void moveSnake( vector<snakeSegment>&snake,direction dir);
bool checkCollisionWithSelf(const vector<snakeSegment>&snake);
bool checkCollisionWithWall(const snakeSegment &head,int screenWidth,int screenHight);





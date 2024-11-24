#include<snake.h>
void renderSnake(SDL_Renderer* renderer,const vector<snakeSegment>&snake,SDL_Texture* headTexture){
SDL_Rect headRect = {snake[0].x ,snake[0].y ,SNAKE_SIZE,SNAKE_SIZE};
SDL_RenderCopy(renderer,headTexture,nullptr,&headRect);
SDL_SetRenderDrawColor(renderer,200,250,100,255);
for(size_t i=0;i<snake.size();i++) {
    SDL_Rect tailRect = {snake[i].x,snake[i].y,SNAKE_SIZE,SNAKE_SIZE};
    SDL_RenderFillRect(renderer,&tailRect);
}
}
void moveSnake(vector<snakeSegment>&snake,direction dir){
    for(int i=snake.size()-1;i>0;i--){
        snake[i]=snake[i-1];
    }
    switch(dir){
        case UP: snake[0].y-SNAKE_SIZE;break;
        case DOWN: snake[0].y+SNAKE_SIZE;break;
        case LEFT: snake[0].x-SNAKE_SIZE;break;
        case RIGHT: snake[0].x+SNAKE_SIZE;break;
    }
}
bool checkCollisionWithSelf(vector<snakeSegment>&snake){
    for(size_t i=1;i<snake.size();i++){
        if(snake[0].x == snake[i].x &&snake[0].y == snake[i].y){
            return true;
        }
    }
    return false;
}
bool  checkCollisionWithWall(snakeSegment &head,int hight,int width){
    return (head.y>hight||head.y<0||head.x>width||head.x<0);
}
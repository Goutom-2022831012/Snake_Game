#include"snake.h"
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const int INITIAL_SNAKE_LENGTH = 3;
enum gameState{INTRO,SELECT_DIFFICULTY,PLAYING,GAME_OVER};
bool init( SDL_Window** window,SDL_Renderer** renderer,TTF_Font** font){
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false; 
    }
    *window = SDL_CreateWindow("Snake Game",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
 *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == nullptr) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
     if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return false;
    }
    *font = TTF_OpenFont("TPF.ttf", 28); 
    if (*font == nullptr) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        return false;
    } 
     int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }
     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        return false;
    }
    return true;
}
SDL_Texture* loadTexture(const string &path,SDL_Renderer* renderer){
SDL_Texture* newTexture = nullptr;
SDL_Surface* newSurface = IMG_Load(path.c_str());
if(newSurface==nullptr){
            cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
}
else{
    newTexture = SDL_CreateTextureFromSurface(renderer,newSurface);
    if(newTexture==nullptr){
                    cout << "Unable to create texture from image " << path << "! SDL_Error: " << SDL_GetError() << endl;
    }
    SDL_FreeSurface(newSurface);
}
    return newTexture;
}
void renderTexture(SDL_Renderer* renderer,SDL_Texture* newTexture,int x ,int y,int w,int h){
    SDL_Rect imageRect ={x,y,w,h};
    SDL_RenderCopy(renderer,newTexture,nullptr,&imageRect);
}
void renderText(SDL_Renderer* renderer,TTF_Font* font,const string &text,int x,int y){
    SDL_Color textColor ={0,0,0};
    SDL_Surface* newSurface = TTF_RenderText_Solid(font,text.c_str(),textColor);
    if (newSurface== nullptr) {
        cout << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
        return;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, newSurface);
    if (newTexture == nullptr) {
        cout << "Failed to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
    } else {
        SDL_Rect textRect = { x, y, newSurface->w, newSurface->h };
        SDL_RenderCopy(renderer, newTexture, nullptr, &textRect);
        SDL_DestroyTexture(newTexture);
    }
    SDL_FreeSurface(newSurface);
}
void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music) {
    Mix_FreeMusic(music);
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
void generateRandomPosition(int &x, int &y) {
    x = (rand() % (SCREEN_WIDTH / SNAKE_SIZE)) * SNAKE_SIZE;
    y = (rand() % (SCREEN_HEIGHT / SNAKE_SIZE)) * SNAKE_SIZE;
}
struct Obstacle {
    int x, y;
};
vector<Obstacle> obstacles; 
void generateObstacles(vector<Obstacle> &obstacles, int count) {
obstacles.clear();
for (int i = 0; i < count; ++i) {
        Obstacle newObstacle;
        bool overlap;
    do {
     generateRandomPosition(newObstacle.x, newObstacle.y);
        overlap = false;
        for (const auto& obs : obstacles) {
            if (obs.x == newObstacle.x && obs.y == newObstacle.y) {
            overlap = true;break;
                }
            }
              } while (overlap);
    obstacles.push_back(newObstacle);
    }
}

void resetGame(vector<snakeSegment>& snake, direction& dir, int& photoX, int& photoY, int& score) {
    snake.clear();
    snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }); 
    for (int i = 1; i < INITIAL_SNAKE_LENGTH; ++i) {
        snake.push_back({ snake[0].x - i * SNAKE_SIZE, snake[0].y });
    }
    dir = RIGHT;
    generateRandomPosition(photoX, photoY);
    score = 0;
    generateObstacles(obstacles, 10);
}
bool checkCollisionWithObstacle(const snakeSegment &head, const vector<Obstacle> &obstacles) {
    for (const auto &obs : obstacles) {
        if (head.x == obs.x && head.y == obs.y) {
            return true;
        }
    }
    return false;
}
void renderObstacles(SDL_Renderer* renderer, const vector<Obstacle>& obstacles) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    for (const auto& obs : obstacles) {
        SDL_Rect obstacleRect = { obs.x, obs.y, SNAKE_SIZE, SNAKE_SIZE };
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
}
int main(int argc,char* args[]){
    srand(static_cast<unsigned>(time(nullptr)));
    SDL_Window* window=nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    Mix_Music* backgroundMusic = nullptr;
    if(!init(&window,&renderer,&font)){
         cout << "Failed to initialize!" << endl;
        return -1;
    }

SDL_Texture* backgroundTextureIntro = loadTexture("intro_background.bmp", renderer); 
SDL_Texture* backgroundTextureGameOver = loadTexture("game_over_background.bmp", renderer);
SDL_Texture* backgroundTextureGame = loadTexture("background.bmp", renderer); 
SDL_Texture* photoTexture = loadTexture("frog.png", renderer);
SDL_Texture* snakeHeadTexture = loadTexture("image.png", renderer); 
if (!backgroundTextureIntro || !backgroundTextureGameOver || !backgroundTextureGame || !photoTexture || !snakeHeadTexture ) {
        cout << "Failed to load textures!" << endl;
        return -1;
    }
 backgroundMusic = Mix_LoadMUS("background.mp3");
    if (backgroundMusic == nullptr) {
        cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }
Mix_PlayMusic(backgroundMusic, -1);
vector<snakeSegment>snake;
direction dir = RIGHT; 
int photoX, photoY;
generateRandomPosition(photoX, photoY);
int score = 0; 
Uint32 startTime = SDL_GetTicks();
gameState gameState = INTRO;
int snakeSpeed;
bool quit = false;
SDL_Event e;
while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
     if (e.type == SDL_QUIT) {
        quit = true;
        cout<<"Game Quit"<<endl;
        }
else if (e.type == SDL_KEYDOWN) {
 if (gameState == INTRO) {
        if (e.key.keysym.sym == SDLK_RETURN) {
              gameState = SELECT_DIFFICULTY; }// Move to difficulty selection
                cout<<"Selecet Difficulty"<<endl;
                 } 
else if (gameState == SELECT_DIFFICULTY) {                 
 switch (e.key.keysym.sym) {
             case SDLK_1: // Easy
             snakeSpeed = 200;
             resetGame(snake, dir, photoX, photoY, score); 
             gameState = PLAYING;
             cout<<"easy state"<<endl;
             break;
            case SDLK_2: // Medium
             snakeSpeed = 150;
             resetGame(snake, dir, photoX, photoY, score); 
             gameState = PLAYING;
             cout<<"medium state"<<endl;
             break;
             case SDLK_3: // Hard
             snakeSpeed = 100;
            resetGame(snake, dir, photoX, photoY, score); 
            gameState = PLAYING;
            cout<<"hard state"<<endl;
             break;
                    }
                } 
else if (gameState == PLAYING) {
     switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                cout<<"Game Quit"<<endl;
                 break;
     case SDLK_UP: if (dir != DOWN) dir = UP; break;
     case SDLK_DOWN: if (dir != UP) dir = DOWN; break;
     case SDLK_LEFT: if (dir != RIGHT) dir = LEFT; break;
     case SDLK_RIGHT: if (dir != LEFT) dir = RIGHT; break;
                    }
                }      
else if (gameState == GAME_OVER) {
         if (e.key.keysym.sym == SDLK_r) {
            cout<<"Going back to difficulty state"<<endl;
         resetGame(snake, dir, photoX, photoY, score);
         gameState = SELECT_DIFFICULTY;
                } 
    else if (e.key.keysym.sym == SDLK_c) {
        quit = true;
        cout<<"Game Quit"<<endl;
         }
             }      
               }
                  }  
if (gameState == INTRO) {
            SDL_RenderClear(renderer);
            renderTexture(renderer, backgroundTextureIntro, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            renderText(renderer, font, "Press Enter to start the game", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50);
            renderText(renderer, font, "Instructions: Use arrow keys to move", SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2);
            renderText(renderer, font, "Eat the frog to grow, avoid walls and yourself", SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 + 50);
            SDL_RenderPresent(renderer);
        } 
else if (gameState == SELECT_DIFFICULTY) {
            SDL_RenderClear(renderer);
            renderTexture(renderer, backgroundTextureIntro, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            renderText(renderer, font, "Select Difficulty:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100);
            renderText(renderer, font, "1. Easy", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50);
            renderText(renderer, font, "2. Medium", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);
            renderText(renderer, font, "3. Hard", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 50);
            SDL_RenderPresent(renderer);
        } 
else if (gameState == GAME_OVER) {
            SDL_RenderClear(renderer);
            renderTexture(renderer, backgroundTextureGameOver, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            renderText(renderer, font, "Game Over", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 50);
            renderText(renderer, font, "Final Score: " + to_string(score), SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2);
            renderText(renderer, font, "Press R to replay or C to close", SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2 + 50);
            SDL_RenderPresent(renderer);
        }
else if (gameState==PLAYING){
    Uint32 currentTime = SDL_GetTicks();
    if(currentTime-startTime>snakeSpeed){
        startTime = currentTime;
        moveSnake(snake,dir);
 // Check collision with the frog
bool atePhoto = false;
if (snake[0].x == photoX && snake[0].y == photoY) {
            snake.push_back({ snake.back().x, snake.back().y });
             generateRandomPosition(photoX, photoY);
            atePhoto = true;
            score++; 
            cout << "Score: " << score << endl;
                }
if (checkCollisionWithSelf(snake)) {
             cout << "Collision with own body detected! Game over." << endl;
             gameState = GAME_OVER;
                }
if (checkCollisionWithWall(snake[0], SCREEN_WIDTH, SCREEN_HEIGHT)) {
              cout << "Collision with wall detected! Game over." << endl;
            gameState = GAME_OVER;
                }
if (checkCollisionWithObstacle(snake[0], obstacles)) {
    cout << "Collision with obstacle detected!" << endl;
    renderText(renderer, font, "You hit an obstacle! Press Y to continue", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2);
     renderText(renderer, font, " N to quit.", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2+50);
    SDL_RenderPresent(renderer);
    bool validInput = false;
    while (!validInput) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_y) {
                    score = max(0, score - 1); validInput = true;  
            } else if (e.key.keysym.sym == SDLK_n) {
                    gameState = GAME_OVER; validInput = true; 
                }
            }
        }
    }
}
SDL_RenderClear(renderer);
renderTexture(renderer, backgroundTextureGame, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
renderSnake(renderer, snake, snakeHeadTexture); 
renderObstacles(renderer, obstacles); 
if (!atePhoto) {
 renderTexture(renderer, photoTexture, photoX, photoY, SNAKE_SIZE, SNAKE_SIZE);
                }
    renderText(renderer, font, "Score: " + to_string(score), 10, 10); 
    SDL_RenderPresent(renderer);
            }
     }                
}
SDL_DestroyTexture(backgroundTextureIntro);
    SDL_DestroyTexture(backgroundTextureGameOver);
    SDL_DestroyTexture(backgroundTextureGame);
    SDL_DestroyTexture(photoTexture);
    SDL_DestroyTexture(snakeHeadTexture);
    close(window, renderer, font, backgroundMusic);
    return 0;

}
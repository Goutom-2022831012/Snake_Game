#include<snake.h>
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
void renderText(SDL_Renderer* renderer,TTF_Font* font,string &text,int x,int y){
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
void resetGame(vector<snakeSegment>& snake, direction& dir, int& photoX, int& photoY, int& score) {
    snake.clear();
    snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }); 
    for (int i = 1; i < INITIAL_SNAKE_LENGTH; ++i) {
        snake.push_back({ snake[0].x - i * SNAKE_SIZE, snake[0].y });
    }
    dir = RIGHT;
    generateRandomPosition(photoX, photoY);
    score = 0;
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
    SDL_Texture* snakeHeadTexture = loadTexture("snake.png", renderer); 

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



}
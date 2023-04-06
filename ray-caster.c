#include <stdio.h>
#include <stdbool.h>

#include <SDL2\SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "RayCaster"

// window ->

struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
};

struct Window window;

// <- window

// map ->

int map_x = 8, map_y = 8;
int tile_size = 15;

const char map[] = { // want colors
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
    'w', '.', '.', 'w', '.', '.', '.', 'w', 
    'w', '.', '.', 'w', '.', 'r', '.', 'w', 
    'w', '.', '.', '.', '.', 'g', '.', 'w', 
    'w', '.', '.', '.', '.', 'b', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', 'w', 
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
};

void mapDraw() {
    SDL_Rect rect;
    for (int y = 0; y < map_y; ++y) {
        for (int x = 0; x < map_x; ++x) {
            // set draw color
            if (map[y * map_x + x] == '.') SDL_SetRenderDrawColor(window.renderer, 0  , 0  , 0  , 255); // black for .
            if (map[y * map_x + x] == 'w') SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255); // white for w
            if (map[y * map_x + x] == 'r') SDL_SetRenderDrawColor(window.renderer, 255, 0  , 0  , 255); // red for r
            if (map[y * map_x + x] == 'g') SDL_SetRenderDrawColor(window.renderer, 0  , 255, 0  , 255); // green for g
            if (map[y * map_x + x] == 'b') SDL_SetRenderDrawColor(window.renderer, 0  , 0  , 255, 255); // blue for b

            rect.w = tile_size - 1; rect.h = tile_size - 1;
            rect.x = x * tile_size; rect.y = y * tile_size;

            SDL_RenderFillRect(window.renderer, &rect);
        }
    }
}

// <- map

// player ->

struct Player {
    SDL_Rect body;
    int x, y, size;
};

struct Player player;

void playerDraw() {
    SDL_SetRenderDrawColor(window.renderer, 255, 183, 197, 255); // pink
    
    player.body.x = player.x; player.body.y = player.y; // position

    SDL_RenderFillRect(window.renderer, &player.body);
}

// <- player

void init() {
    // SDL initilization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0) {printf("INITALIZATION OF SDL VIDEO AND AUDIO FAILED WITH ERROR: %s", SDL_GetError()); window.quit = true;}
    printf("INITALIZATION OF SDL VIDEO AND AUDIO SUCCESS");
    
    // window ->
    window.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    window.renderer = SDL_CreateRenderer(window.window, -1, 0);
    // <- window

    // player ->
    player.size = 2; player.body.w = player.size; player.body.h = player.size;
    player.x = (int)((map_x * tile_size) / 2); player.y = (int)((map_y * tile_size) / 2);
    // <- player
}

void update() {}

void event() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        
        switch (event.type) {
            // window ->
            case SDL_QUIT:
                window.quit = true;
                break;
            // <- window

            default:
                break;
        }
    }
}

void draw() {
    // window ->
    SDL_SetRenderDrawColor(window.renderer, 10, 10, 10, 255); // a dark grey
    SDL_RenderClear(window.renderer);
    // <- window

    mapDraw();
    playerDraw();

    SDL_RenderPresent(window.renderer);
}

void destroy() {
    SDL_DestroyWindow(window.window);
    SDL_DestroyRenderer(window.renderer);
    SDL_Quit();
} 

int main(int argv, char *argc[]) {
    init();
    while (!window.quit) {
        event();
        update();
        draw();
    }
    destroy();
    return 0;
}
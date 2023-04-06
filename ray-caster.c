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

void init() {
    // SDL initilization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0) {printf("INITALIZATION OF SDL VIDEO AND AUDIO FAILED WITH ERROR: %s", SDL_GetError()); window.quit = true;}
    printf("INITALIZATION OF SDL VIDEO AND AUDIO SUCCESS");
    
    // window ->
    window.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    window.renderer = SDL_CreateRenderer(window.window, -1, 0);
    // <- window
}

void update() {}

void event() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        // window ->
        if(event.type == SDL_QUIT){window.quit = true;}
        // <- window
    }
}

void draw() {
    SDL_SetRenderDrawColor(window.renderer, 10, 10, 10, 255);
    SDL_RenderClear(window.renderer);
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
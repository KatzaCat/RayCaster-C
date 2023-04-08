#include <stdio.h>
#include <stdbool.h>

#include <SDL2\SDL.h>

// math qwq
#include <math.h>
#define PI 3.141592653589793238462643383279502884197
#define P2 PI/2
#define P3 3*PI/2
#define DEG 0.0174533
// fuck radiens


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

int map_x = 10, map_y = 10;
int tile_size = 15;

const char map[] = { // want colors
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
    'w', '.', '.', '.', 'w', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', 'w', '.', '.', 'g', '.', 'w', 
    'w', '.', '.', '.', 'w', '.', '.', 'r', '.', 'w', 
    'w', 'w', '.', 'w', 'w', '.', '.', 'b', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
};

// <- map

// player ->

struct Player {
    float x, y, delta_x, delta_y, angle;
    int speed;
};

struct Player player;

// <- player

// rays ->

float dist(float ax, float ay, float bx, float by, float anglw) {
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void raysDraw() {
    // var initilization
    char m_horizontal, m_vertical;
    int dof, mx, my, mpos;
    float ray_x, ray_y, offset_x, offset_y, ray_angle, dis;

    ray_angle = player.angle - DEG * 30; if (ray_angle < 0) {ray_angle += 2*PI;} if (ray_angle > 2*PI) {ray_angle -= 2*PI;}

    for (int ray = 0; ray < WINDOW_WIDTH; ++ray) {
        // check horizontal ->
        dof = 0;
        float aTan = -1 / tan(ray_angle);

        float disH = 1000000000, hx = player.x, hy = player.y;

        if (ray_angle > PI) {ray_y = (((int)player.y / tile_size) * tile_size) -   0.00001; ray_x = (player.y - ray_y) * aTan + player.x; offset_y = -tile_size; offset_x = -offset_y * aTan;} //looking up
        if (ray_angle < PI) {ray_y = (((int)player.y / tile_size) * tile_size) + tile_size; ray_x = (player.y - ray_y) * aTan + player.x; offset_y =  tile_size; offset_x = -offset_y * aTan;} //looking down
        if (ray_angle == 0 || ray_angle == PI) {ray_x = player.x; ray_y = player.y; dof = 8;} // straght

        while (dof < 8) {
            mx = (int)(ray_x) / tile_size; my = (int)(ray_y) / tile_size; mpos = my * map_x + mx;

            if (mpos > 0 && mpos < map_x * map_y && map[mpos] != '.') {m_horizontal = map[mpos]; hx = ray_x; hy = ray_y; disH = dist(player.x, player.y, hx, hy, ray_angle); dof = 8;} // hit wall
            else {ray_x += offset_x; ray_y += offset_y; dof += 1;}
        }
        // <- check horizontal

        // check vertical ->
        dof = 0;
        float nTan = -tan(ray_angle);

        float disV = 1000000000, vx = player.x, vy = player.y;

        if (ray_angle > P2 && ray_angle < P3) {ray_x = (((int)player.x / tile_size) * tile_size) -   0.00001; ray_y = (player.x - ray_x) * nTan + player.y; offset_x = -tile_size; offset_y = -offset_x * nTan;} //looking left
        if (ray_angle < P2 || ray_angle > P3) {ray_x = (((int)player.x / tile_size) * tile_size) + tile_size; ray_y = (player.x - ray_x) * nTan + player.y; offset_x =  tile_size; offset_y = -offset_x * nTan;} //looking right
        if (ray_angle == 0 || ray_angle == PI) {ray_x = player.x; ray_y = player.y; dof = 8;} // straght

        while (dof < 8) {
            mx = (int)(ray_x) / tile_size; my = (int)(ray_y) / tile_size; mpos = my * map_x + mx;

            if (mpos > 0 && mpos < map_x * map_y && map[mpos] != '.') {m_vertical = map[mpos]; vx = ray_x; vy = ray_y; disV = dist(player.x, player.y, vx, vy, ray_angle); dof = 8;} // hit wall
            else {ray_x += offset_x; ray_y += offset_y; dof += 1;}
        }
        // <- check vertical

        // draw ray
        if (disV < disH) {
            ray_x = vx; ray_y = vy; dis = disV; 
            if (m_vertical == 'w') SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
            if (m_vertical == 'r') SDL_SetRenderDrawColor(window.renderer, 255,   0,   0, 255);
            if (m_vertical == 'g') SDL_SetRenderDrawColor(window.renderer,   0, 255,   0, 255);
            if (m_vertical == 'b') SDL_SetRenderDrawColor(window.renderer,   0,   0, 255, 255);
        }
        if (disV > disH) {
            ray_x = hx; ray_y = hy; dis = disH; 
            if (m_horizontal == 'w') SDL_SetRenderDrawColor(window.renderer, 127, 127, 127, 255);
            if (m_horizontal == 'r') SDL_SetRenderDrawColor(window.renderer, 127,   0,   0, 255);
            if (m_horizontal == 'g') SDL_SetRenderDrawColor(window.renderer,   0, 127,   0, 255);
            if (m_horizontal == 'b') SDL_SetRenderDrawColor(window.renderer,   0,   0, 127, 255);
        }

        // draw walls ->
        float cos_angle = player.angle - ray_angle; if (cos_angle < 0) {cos_angle += 2*PI;} if (cos_angle > 2*PI) {cos_angle -= 2*PI;} dis = dis * cos(cos_angle); // removes fish eye
        float lineH = (tile_size * WINDOW_HEIGHT) / dis; if (lineH > WINDOW_HEIGHT) {lineH = WINDOW_HEIGHT;}
        float lineO = 360 - lineH / 2;
        SDL_RenderDrawLine(window.renderer, ray, lineO, ray, lineH + lineO);
        // <- draw walls

        ray_angle += DEG / 21.3333; if (ray_angle < 0) {ray_angle += 2*PI;} if (ray_angle > 2*PI) {ray_angle -= 2*PI;}
    }
}

// <- rays

void init() {
    // SDL initilization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0) {printf("INITALIZATION OF SDL VIDEO AND AUDIO FAILED WITH ERROR: %s", SDL_GetError()); window.quit = true;}
    printf("INITALIZATION OF SDL VIDEO AND AUDIO SUCCESS");
    
    // window ->
    window.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    window.renderer = SDL_CreateRenderer(window.window, -1, 0);
    // <- window

    // player ->
    player.speed = 2;
    player.x = (map_x * tile_size) / 2; player.y = (map_y * tile_size) / 2;
    player.delta_x = cos(player.angle) * 2; player.delta_y = sin(player.angle) * 2;
    // <- player
}

void event() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        
        switch (event.type) {
            // window ->
            case SDL_QUIT:
                window.quit = true;
                break;
            // <- window

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    // window ->
                    case SDLK_ESCAPE: window.quit = true;
                    // <- window
                    
                    // player ->
                    case SDLK_a: player.angle -= 0.1; if (player.angle < 0   ) {player.angle += 2*PI;} player.delta_x = cos(player.angle) * 1.5; player.delta_y = sin(player.angle) * 1.5; break; // turn left
                    case SDLK_d: player.angle += 0.1; if (player.angle > 2*PI) {player.angle -= 2*PI;} player.delta_x = cos(player.angle) * 1.5; player.delta_y = sin(player.angle) * 1.5; break; // turn right
                    case SDLK_w: player.x += player.delta_x; player.y += player.delta_y; break; // forward
                    case SDLK_s: player.x -= player.delta_x; player.y -= player.delta_y; break; // backward
                    // <- player
                    
                    default:
                        break;
                }

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

    raysDraw();

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
        draw();
    }
    destroy();
    return 0;
}

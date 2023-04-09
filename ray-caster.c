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

// textures ->

// rgb 10 x 10 per
const char brick_wall[] = {
    'r', 'r', 'r', 'r', 'w', 'r', 'r', 'r', 'r', 'w', 
    'r', 'r', 'r', 'r', 'w', 'r', 'r', 'r', 'r', 'w', 
    '.', '.', '.', '.', 'w', '.', '.', '.', '.', 'w', 
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
    'w', 'r', 'r', 'r', 'r', 'w', 'r', 'r', 'r', 'r', 
    'w', 'r', 'r', 'r', 'r', 'w', 'r', 'r', 'r', 'r', 
    'w', '.', '.', '.', '.', 'w', '.', '.', '.', '.',
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w',
    'r', 'r', 'r', 'r', 'w', 'r', 'r', 'r', 'r', 'w', 
    'r', 'r', 'r', 'r', 'w', 'r', 'r', 'r', 'r', 'w', 
};

// <- textures

// window ->

struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
};

struct Window window;

// <- window

// map ->

int map_x = 19, map_y = 19;
int tile_size = 15;

const char map[] = { // want colors
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
    'w', '.', '.', '.', 'w', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', 'w', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', 'w', '.', '.', '.', '.', '.', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', 'w', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', 'w', 'w', 'w', 'w', '.', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
    'w', 'w', 'w', 'w', '.', '.', '.', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', 'w', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', '.', '.', '.', '.', '.', '.', '.', '.', 'w', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', 
    'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 
};

// <- map

// player ->

struct Player {
    float x, y, delta_x, delta_y, angle;
    int speed;
    int x_offset, y_offset;
    int grid_pos_x, grid_pos_add_x_offset, grid_pos_sub_x_offset;
    int grid_pos_y, grid_pos_add_y_offset, grid_pos_sub_y_offset;
}player;

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
        if (ray_angle == 0 || ray_angle == PI) {ray_x = player.x; ray_y = player.y; dof = 22;} // straght

        while (dof < 22) {
            mx = (int)(ray_x) / tile_size; my = (int)(ray_y) / tile_size; mpos = my * map_x + mx;

            if (mpos > 0 && mpos < map_x * map_y && map[mpos] != '.') {m_horizontal = map[mpos]; hx = ray_x; hy = ray_y; disH = dist(player.x, player.y, hx, hy, ray_angle); dof = 22;} // hit wall
            else {ray_x += offset_x; ray_y += offset_y; dof += 1;}
        }
        // <- check horizontal

        // check vertical ->
        dof = 0;
        float nTan = -tan(ray_angle);

        float disV = 1000000000, vx = player.x, vy = player.y;

        if (ray_angle > P2 && ray_angle < P3) {ray_x = (((int)player.x / tile_size) * tile_size) -   0.00001; ray_y = (player.x - ray_x) * nTan + player.y; offset_x = -tile_size; offset_y = -offset_x * nTan;} //looking left
        if (ray_angle < P2 || ray_angle > P3) {ray_x = (((int)player.x / tile_size) * tile_size) + tile_size; ray_y = (player.x - ray_x) * nTan + player.y; offset_x =  tile_size; offset_y = -offset_x * nTan;} //looking right
        if (ray_angle == 0 || ray_angle == PI) {ray_x = player.x; ray_y = player.y; dof = 22;} // straght

        while (dof < 22) {
            mx = (int)(ray_x) / tile_size; my = (int)(ray_y) / tile_size; mpos = my * map_x + mx;

            if (mpos > 0 && mpos < map_x * map_y && map[mpos] != '.') {m_vertical = map[mpos]; vx = ray_x; vy = ray_y; disV = dist(player.x, player.y, vx, vy, ray_angle); dof = 22;} // hit wall
            else {ray_x += offset_x; ray_y += offset_y; dof += 1;}
        }
        // <- check vertical

        // draw ray
        float shade = 1;
        if (disV < disH) {
            ray_x = vx; ray_y = vy; dis = disV; 
            //if (m_vertical == 'w') SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
            //if (m_vertical == 'r') SDL_SetRenderDrawColor(window.renderer, 255,   0,   0, 255);
            //if (m_vertical == 'g') SDL_SetRenderDrawColor(window.renderer,   0, 255,   0, 255);
            //if (m_vertical == 'b') SDL_SetRenderDrawColor(window.renderer,   0,   0, 255, 255);
        }
        if (disV > disH) {
            ray_x = hx; ray_y = hy; dis = disH; 
            shade = 0.5;
            //if (m_horizontal == 'w') SDL_SetRenderDrawColor(window.renderer, 127, 127, 127, 255);
            //if (m_horizontal == 'r') SDL_SetRenderDrawColor(window.renderer, 127,   0,   0, 255);
            //if (m_horizontal == 'g') SDL_SetRenderDrawColor(window.renderer,   0, 127,   0, 255);
            //if (m_horizontal == 'b') SDL_SetRenderDrawColor(window.renderer,   0,   0, 127, 255);
        }

        // draw walls ->
        float cos_angle = player.angle - ray_angle; if (cos_angle < 0) {cos_angle += 2*PI;} if (cos_angle > 2*PI) {cos_angle -= 2*PI;} dis = dis * cos(cos_angle); // removes fish eye
        float lineH = (tile_size * WINDOW_HEIGHT) / dis; 
        float texture_y_step = 10.0 / (float)lineH;
        float texture_y_offset = 0;
        if (lineH > WINDOW_HEIGHT) {texture_y_offset = (lineH - WINDOW_HEIGHT) / 2; lineH = WINDOW_HEIGHT;}
        float lineO = 360 - lineH / 2;

        float texture_y = texture_y_offset * texture_y_step;
        float texture_x;

        if (shade == 1) {texture_x = (int)(ray_y / 2) % 10; if (ray_angle > P2 && ray_angle < P3) texture_x = 9 - texture_x;}
        else            {texture_x = (int)(ray_x / 2) % 10; if (ray_angle > PI                  ) texture_x = 9 - texture_x;}
        for (int y = 0; y < lineH; y++) {
            char color = brick_wall[(int)(texture_y) * 10 + (int)(texture_x)];
            if (color == '.') SDL_SetRenderDrawColor(window.renderer,                  0,                  0,                  0, 255);
            if (color == 'w') SDL_SetRenderDrawColor(window.renderer, (int)(255 * shade), (int)(255 * shade), (int)(255 * shade), 255);
            if (color == 'r') SDL_SetRenderDrawColor(window.renderer, (int)(255 * shade),                  0,                  0, 255);
            if (color == 'g') SDL_SetRenderDrawColor(window.renderer,                  0, (int)(255 * shade),                  0, 255);
            if (color == 'b') SDL_SetRenderDrawColor(window.renderer,                  0,                  0, (int)(255 * shade), 255);
            SDL_RenderDrawPoint(window.renderer, ray, y + lineO);
            texture_y += texture_y_step;
        }
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
    player.x = (3 * tile_size); player.y = (3 * tile_size);
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
                    case SDLK_ESCAPE: window.quit = true; break;
                    // <- window
                    
                    // player ->
                    case SDLK_a: player.angle -= 0.1; if (player.angle < 0   ) {player.angle += 2*PI;} player.delta_x = cos(player.angle) * 1.5; player.delta_y = sin(player.angle) * 1.5; break;
                    case SDLK_d: player.angle += 0.1; if (player.angle > 2*PI) {player.angle -= 2*PI;} player.delta_x = cos(player.angle) * 1.5; player.delta_y = sin(player.angle) * 1.5; break;
                    
                    case SDLK_w: 
                        if (map[player.grid_pos_y            * map_x + player.grid_pos_add_x_offset] == '.') {player.x += player.delta_x;} 
                        if (map[player.grid_pos_add_y_offset * map_x + player.grid_pos_x           ] == '.') {player.y += player.delta_y;}
                        break;
                    case SDLK_s: 
                        if (map[player.grid_pos_y            * map_x + player.grid_pos_sub_x_offset] == '.') {player.x -= player.delta_x;} 
                        if (map[player.grid_pos_sub_y_offset * map_x + player.grid_pos_x           ] == '.') {player.y -= player.delta_y;}
                        break;
                    // <- player
                    
                    default:
                        break;
                }

            default:
                break;
        }
    }
}

void update() {
    player.grid_pos_x = player.x / tile_size; player.grid_pos_add_x_offset = (player.x + player.x_offset) / tile_size; player.grid_pos_sub_x_offset = (player.x - player.x_offset) / tile_size;
    player.grid_pos_y = player.y / tile_size; player.grid_pos_add_y_offset = (player.y + player.y_offset) / tile_size; player.grid_pos_sub_y_offset = (player.y - player.y_offset) / tile_size;
    player.x_offset = 0; if (player.delta_x < 0) player.x_offset = -10; else player.x_offset = 10;
    player.y_offset = 0; if (player.delta_y < 0) player.y_offset = -10; else player.y_offset = 10;
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
        update();
        draw();
    }
    destroy();
    return 0;
}

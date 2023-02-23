#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "src/include/SDL2/SDL.h"

const int HEIGHT = 480;
const int WIDTH = 640;
const int BALLVEL = 130;
const int BALLSIZE = 10;
const int PLAYERPAD_WIDTH = 22;
const int PLAYERPAD_HEIGHT = 75;
const int PLAYERPAD_MARGIN = 10;
const float PLAYERPAD_VELOCITY = 180.0f;
bool gameStart = false;
bool playWithCPU = false;

typedef struct BallStruct
{
    float x;
    float y;
    float xVel;
    float yVel;
    int size;
} BallStruct;

typedef struct PlayerPad
{
    float yPos;
    int score;
} PlayerPad;

bool initialize(void);
void Update(float);
void Shutdown(void);
void scoreUpdate(int, int);

BallStruct InitBall(int size);
void RenderBall(const BallStruct *);
void UpdateBall(BallStruct *, float);

PlayerPad InitPlayerPad(void);
void RenderPlayerPad(void);

void UpdatePlayerPad(float);

void UpdateCPUPad(float);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
BallStruct ball;
PlayerPad pad1;
PlayerPad pad2;

int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));
    atexit(Shutdown);

    if (!initialize())
    {
        exit(1);
    }

    SDL_Event event;
    int quit = false;

    Uint32 prevTick = SDL_GetTicks();
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Loop animation upate to keep frame consistent across format.
        Uint32 currTick = SDL_GetTicks();
        Uint32 diffTick = currTick - prevTick;
        float t_elapsed = diffTick / 1000.0f;
        Update(t_elapsed);
        prevTick = currTick;
    }
    return 0;
}

bool initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "failed initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("SUPER PONG",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH, HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window)
    {
        return false;
    }
    ball = InitBall(BALLSIZE);
    pad1 = InitPlayerPad();
    pad2 = InitPlayerPad();
    return true;
}

void Update(float t_elapse)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    /*SDL_Rect ball = {
        .x = WIDTH/2 - BALL/2,
        .y = HEIGHT/2 - BALL/2,
        .w = BALL,
        .h = BALL
    };
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderPresent(renderer);
    */
    UpdateBall(&ball, t_elapse);
    RenderBall(&ball);

    UpdatePlayerPad(t_elapse);
    UpdateCPUPad(t_elapse);
    RenderPlayerPad();

    SDL_RenderPresent(renderer);
}

void Shutdown(void)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

BallStruct InitBall(int size)
{
    const float VEL = BALLVEL;
    BallStruct ball = {
        .x = WIDTH / 2 - size / 2,
        .y = HEIGHT / 2 - size / 2,
        .size = size,
        .xVel = VEL * ((rand() % 2 == 1 ? true : false) ? 1 : -1),
        .yVel = VEL * ((rand() % 2 == 1 ? true : false) ? 1 : -1)};
    return ball;
}

void RenderBall(const BallStruct *ball)
{
    int size = ball->size;
    SDL_Rect rect = {
        .x = ball->x - size / 2,
        .y = ball->y - size / 2,
        .w = size,
        .h = size,

    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void UpdateBall(BallStruct *ball, float t_elapse)
{
    if (!gameStart)
    {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        return;
    }
    ball->x += ball->xVel * t_elapse;
    ball->y += ball->yVel * t_elapse;

    if (ball->x < BALLSIZE / 2)
        scoreUpdate(2, 1); // ball->xVel = fabs(ball->xVel);
    if (ball->y < BALLSIZE / 2)
        ball->yVel = fabs(ball->yVel);
    if (ball->x > WIDTH - BALLSIZE / 2)
        scoreUpdate(1, 1); // ball->xVel = -fabs(ball->xVel);
    if (ball->y > HEIGHT - BALLSIZE / 2)
        ball->yVel = -fabs(ball->yVel);
}

PlayerPad InitPlayerPad(void)
{
    PlayerPad pad = {
        .yPos = HEIGHT / 2,
    };
    return pad;
}

void RenderPlayerPad(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 100, 255);
    SDL_Rect pad1Rect = {
        .x = PLAYERPAD_MARGIN,
        .y = (int)(pad1.yPos) - PLAYERPAD_HEIGHT / 2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &pad1Rect);

    SDL_SetRenderDrawColor(renderer, 50, 255, 0, 255);
    SDL_Rect pad2Rect = {
        .x = WIDTH - PLAYERPAD_WIDTH - PLAYERPAD_MARGIN,
        .y = (int)(pad2.yPos) - PLAYERPAD_HEIGHT / 2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &pad2Rect);
}

void UpdatePlayerPad(float t_elapse)
{
    const Uint8 *key = SDL_GetKeyboardState(NULL);
    if (key[SDL_SCANCODE_SPACE])
        gameStart = true;
    if (key[SDL_SCANCODE_W])
        pad1.yPos -= PLAYERPAD_VELOCITY * t_elapse;
    if (key[SDL_SCANCODE_S])
        pad1.yPos += PLAYERPAD_VELOCITY * t_elapse;
    // if (key[SDL_SCANCODE_UP]) pad2.yPos -= PLAYERPAD_VELOCITY * t_elapse;
    // if (key[SDL_SCANCODE_DOWN]) pad2.yPos += PLAYERPAD_VELOCITY * t_elapse;

    if (pad1.yPos < PLAYERPAD_HEIGHT / 2)
        pad1.yPos = PLAYERPAD_HEIGHT / 2;
    // if (pad2.yPos<PLAYERPAD_HEIGHT/2) pad2.yPos = PLAYERPAD_HEIGHT/2;

    if (pad1.yPos > HEIGHT - PLAYERPAD_HEIGHT / 2)
        pad1.yPos = HEIGHT - PLAYERPAD_HEIGHT / 2;
    // if (pad2.yPos>HEIGHT-PLAYERPAD_HEIGHT/2) pad2.yPos = HEIGHT - PLAYERPAD_HEIGHT/2;

    // overlaps ball?
    SDL_Rect ballRect = {
        .x = ball.x - ball.size / 2,
        .y = ball.y - ball.size / 2,
        .w = ball.size,
        .h = ball.size,
    };
    SDL_Rect pad1Rect = {
        .x = PLAYERPAD_MARGIN,
        .y = (int)(pad1.yPos) - PLAYERPAD_HEIGHT / 2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &pad1Rect))
        ball.xVel = fabs(ball.xVel);

    SDL_Rect pad2Rect = {
        .x = WIDTH - PLAYERPAD_WIDTH - PLAYERPAD_MARGIN,
        .y = (int)(pad2.yPos) - PLAYERPAD_HEIGHT / 2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &pad2Rect))
        ball.xVel = -fabs(ball.xVel);
}

void UpdateCPUPad(float t_elapse)
{
    if (ball.x > WIDTH / 2)
    {
        if (pad2.yPos <= ball.y)
            pad2.yPos += PLAYERPAD_VELOCITY * t_elapse;

        if (pad2.yPos >= ball.y)
            pad2.yPos -= PLAYERPAD_VELOCITY * t_elapse;
    }
    else
    {
        pad2.yPos = pad2.yPos;
    }
    if (pad2.yPos < PLAYERPAD_HEIGHT / 2)
        pad2.yPos = PLAYERPAD_HEIGHT / 2;
    if (pad2.yPos > HEIGHT - PLAYERPAD_HEIGHT / 2)
        pad2.yPos = HEIGHT - PLAYERPAD_HEIGHT / 2;
}

void scoreUpdate(int pad, int score)
{
    gameStart = false;
    if (pad == 1)
        pad1.score += score;
    if (pad == 2)
        pad2.score += score;

    char *f = "Pad1 Score: %d | Pad2 Score: %d";
    int len = snprintf(NULL, 0, f, pad1.score, pad2.score);
    char buf[len + 1];
    snprintf(buf, len + 1, f, pad1.score, pad2.score);
    SDL_SetWindowTitle(window, buf);
}

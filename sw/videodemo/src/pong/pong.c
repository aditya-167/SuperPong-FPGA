#include "./pong.h"

bool initialize(ball_struct *ball, player_pad *pad1, player_pad *pad2){
    
    *ball = init_ball();
    *pad1 = init_player_pad();
    *pad2 = init_player_pad();
    return true;

}

void update(float t_elapse){

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderClear(renderer);

    UpdateBall(&ball,t_elapse);
    RenderBall(&ball);
    
    UpdatePlayerPad(t_elapse);
    UpdateCPUPad(t_elapse);
    RenderPlayerPad();
    
    SDL_RenderPresent(renderer);
    
}

void shutdown(void){
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

ball_struct init_ball(int size){
    const float VEL= BALLVEL;
    BallStruct ball = {
        .x = WIDTH/2 - size/2,
        .y = HEIGHT/2 - size/2,
        .size = size,
        .xVel = VEL*((rand()%2==1?true:false)?1:-1),
        .yVel = VEL*((rand()%2==1?true:false)?1:-1)
    };
    return ball;
}

void render_ball(const ball_struct *ball){
    int size = ball->size;
    SDL_Rect rect = {
        .x = ball->x - size/2,
        .y = ball->y - size/2,
        .w = size,
        .h = size,
           
    };
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderFillRect(renderer, &rect);
}

void update_ball(ball_struct *ball, float t_elapse){
    if (!gameStart){
        ball->x = WIDTH/2;
        ball->y = HEIGHT/2;
        return;
    }
    ball->x += ball->xVel * t_elapse;
    ball->y += ball->yVel * t_elapse;

    if (ball->x <BALLSIZE/2) scoreUpdate(2,1);//ball->xVel = fabs(ball->xVel);
    if (ball->y <BALLSIZE/2) ball->yVel = fabs(ball->yVel);
    if (ball->x >WIDTH-BALLSIZE/2) scoreUpdate(1,1); //ball->xVel = -fabs(ball->xVel);
    if (ball->y >HEIGHT-BALLSIZE/2) ball->yVel = -fabs(ball->yVel);

}

player_pad init_player_pad(void){
    player_pad pad = {
        .yPos = SCREEN_HEIGHT/2,
    };
    return pad;

}

void render_player_pad(void){
    SDL_SetRenderDrawColor(renderer, 255,0,100,255);
    SDL_Rect pad1Rect = {
        .x = PLAYERPAD_MARGIN,
        .y = (int)(pad1.yPos) - PLAYERPAD_HEIGHT/2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &pad1Rect);

    SDL_SetRenderDrawColor(renderer, 50,255,0,255);
    SDL_Rect pad2Rect = {
        .x = WIDTH - PLAYERPAD_WIDTH- PLAYERPAD_MARGIN,
        .y = (int)(pad2.yPos) - PLAYERPAD_HEIGHT/2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &pad2Rect);

}

void update_player_pad(float t_elapse){
    const Uint8 *key = SDL_GetKeyboardState(NULL);
    if (key[SDL_SCANCODE_SPACE]) gameStart = true;
    if (key[SDL_SCANCODE_W]) pad1.yPos -= PLAYERPAD_VELOCITY * t_elapse;
    if (key[SDL_SCANCODE_S]) pad1.yPos += PLAYERPAD_VELOCITY * t_elapse;
    //if (key[SDL_SCANCODE_UP]) pad2.yPos -= PLAYERPAD_VELOCITY * t_elapse;
    //if (key[SDL_SCANCODE_DOWN]) pad2.yPos += PLAYERPAD_VELOCITY * t_elapse;

    if (pad1.yPos<PLAYERPAD_HEIGHT/2) pad1.yPos = PLAYERPAD_HEIGHT/2;
    //if (pad2.yPos<PLAYERPAD_HEIGHT/2) pad2.yPos = PLAYERPAD_HEIGHT/2;

    if (pad1.yPos>HEIGHT-PLAYERPAD_HEIGHT/2) pad1.yPos = HEIGHT - PLAYERPAD_HEIGHT/2;
    //if (pad2.yPos>HEIGHT-PLAYERPAD_HEIGHT/2) pad2.yPos = HEIGHT - PLAYERPAD_HEIGHT/2;

    //overlaps ball?
    SDL_Rect ballRect = {
        .x = ball.x - ball.size/2,
        .y = ball.y - ball.size/2,
        .w = ball.size,
        .h = ball.size,
    };
    SDL_Rect pad1Rect = {
        .x = PLAYERPAD_MARGIN,
        .y = (int)(pad1.yPos) - PLAYERPAD_HEIGHT/2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };

    if(SDL_HasIntersection(&ballRect,&pad1Rect)) ball.xVel = fabs(ball.xVel);
    
    SDL_Rect pad2Rect = {
        .x = WIDTH - PLAYERPAD_WIDTH- PLAYERPAD_MARGIN,
        .y = (int)(pad2.yPos) - PLAYERPAD_HEIGHT/2,
        .w = PLAYERPAD_WIDTH,
        .h = PLAYERPAD_HEIGHT,
    };
    
    if(SDL_HasIntersection(&ballRect,&pad2Rect)) ball.xVel = -fabs(ball.xVel);
        
}

void update_CPU_pad(float t_elapse){
    if (ball.x>WIDTH/2){
        if (pad2.yPos <= ball.y)
        pad2.yPos += PLAYERPAD_VELOCITY * t_elapse;

        if (pad2.yPos >= ball.y)
        pad2.yPos -= PLAYERPAD_VELOCITY * t_elapse;
 
    }else{
        pad2.yPos = pad2.yPos;
    }
    if (pad2.yPos<PLAYERPAD_HEIGHT/2) pad2.yPos = PLAYERPAD_HEIGHT/2;
    if (pad2.yPos>HEIGHT-PLAYERPAD_HEIGHT/2) pad2.yPos = HEIGHT - PLAYERPAD_HEIGHT/2;
}

void score_update(int pad, int score){
   gameStart = false;
   if (pad ==1) pad1.score +=score;
   if (pad ==2) pad2.score +=score;

    char* f = "Pad1 Score: %d | Pad2 Score: %d";
    int len = snprintf(NULL,0,f,pad1.score,pad2.score);
    char buf[len+1];
    snprintf(buf, len+1,f,pad1.score,pad2.score);
    SDL_SetWindowTitle(window,buf);
}


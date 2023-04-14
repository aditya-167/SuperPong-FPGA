#include "pong.h"

// Initialise the game
bool initialize(game_context* game, PmodJSTK2* player_1_joystick,
		PmodJSTK2* player_2_joystick) {

	ball_struct* ball = init_ball(BALLSIZE);
	player_pad* pad1 = init_player_pad();
	player_pad* pad2 = init_player_pad();

	pad1->xPos = 50;
	pad2->xPos = SCREEN_WIDTH - 50;

	game->ball = ball;
	game->pad1 = pad1;
	game->pad2 = pad2;
	game->player_1_joystick = player_1_joystick;
	game->player_2_joystick = player_2_joystick;
	game->state = RUNNING;
	game->camera_control = 0;

	return true;

}

// Initialise the ball
ball_struct* init_ball(int size) {

	ball_struct ball = { .x = SCREEN_WIDTH / 2 - size / 2, .y = SCREEN_HEIGHT
			/ 2 - size / 2, .size = size, .xVel = BALLVEL, .yVel = BALLVEL,
			.pX = 0, .pY = 0 };

	ball_struct* ball_ptr = malloc(sizeof(ball_struct));
	ball_ptr->size = ball.size;
	ball_ptr->x = ball.x;
	ball_ptr->xVel = -ball.xVel;
	ball_ptr->y = ball.y;
	ball_ptr->yVel = ball.yVel;
	ball_ptr->pX = ball.x;
	ball_ptr->pY = ball.y;

	return ball_ptr;

}

// Initialise the player
player_pad* init_player_pad(void) {

	player_pad pad = { .yPos = SCREEN_HEIGHT / 2, .score = 0 };

	player_pad* pad_ptr = malloc(sizeof(player_pad));
	pad_ptr->score = pad.score;
	pad_ptr->yPos = pad.yPos;
	pad_ptr->w = PLAYERPAD_WIDTH;
	pad_ptr->h = PLAYERPAD_HEIGHT;

	return pad_ptr;
}

// Update the score and print it to the Serial terminal.
void score_update(player_pad* pad1, int score) {
	pad1->score = pad1->score + score;
}

// Update the CPU player's position
void update_CPU_pad(player_pad* pad, ball_struct* ball, int t_elapse, int camera_controlled) {

	pad->pY = pad->yPos;
	pad->pX = pad->xPos;

	if (camera_controlled){
		int val = get_number();
		pad->yPos = val;
		xil_printf("%d\n", pad->yPos);
	}

	else if (pad->xPos < SCREEN_WIDTH / 2) {
		if (ball->x < SCREEN_WIDTH / 2) {
			if (pad->yPos <= ball->y)
				pad->yPos += PLAYERPAD_VELOCITY * t_elapse;

			if (pad->yPos >= ball->y)
				pad->yPos -= PLAYERPAD_VELOCITY * t_elapse;

		}
	} else {
		if (ball->x > SCREEN_WIDTH / 2) {
			if (pad->yPos <= ball->y)
				pad->yPos += PLAYERPAD_VELOCITY * t_elapse;

			if (pad->yPos >= ball->y)
				pad->yPos -= PLAYERPAD_VELOCITY * t_elapse;

		}

	}

	if (pad->yPos < 0)
		pad->yPos = 0;

	if (pad->yPos > SCREEN_HEIGHT - pad->h)
		pad->yPos = SCREEN_HEIGHT - pad->h;

}

// Update the player pad position
void update_player_pad(PmodJSTK2* player, player_pad* pad, ball_struct* ball,
		int t_elapse) {

	u16 Ydata;

	float step;

	pad->pX = pad->xPos;
	pad->pY = pad->yPos;

	Ydata = JSTK2_getY(player);

	step = JOY_STEP * (((float) (Ydata - 128)) / 128);

	pad->yPos -= step;

	if (pad->yPos < 0)
		pad->yPos = 0;

	if (pad->yPos > SCREEN_HEIGHT - pad->h)
		pad->yPos = SCREEN_HEIGHT - pad->h;

}

void check_interesections(game_context* g, graphics_context* gc) {

	// Ball rectangle
	SDL_Rect ballRect = { .x = g->ball->x - g->ball->size / 2, .y = g->ball->y
			+ g->ball->size / 2, .w = g->ball->size, .h = g->ball->size, };

	// Player rectangle
	SDL_Rect pad1Rect = { .x = g->pad1->xPos, .y = g->pad1->yPos, .w =
			g->pad1->w, .h = g->pad1->h };

	// CPU rectangle
	SDL_Rect pad2Rect = { .x = g->pad2->xPos, .y = g->pad2->yPos, .w =
			g->pad1->w, .h = g->pad1->h };

	// Check for intersections
	if (SDL_HasIntersection(&ballRect, &pad1Rect))
		g->ball->xVel = -(g->ball->xVel);

	if (SDL_HasIntersection(&ballRect, &pad2Rect)) {
		g->ball->xVel = -(g->ball->xVel);
	}

}

// Update the ball structure
void update_ball(game_context *game, GAME_STATE state, int t_elapse) {

	ball_struct* ball = game->ball;

	if (state == STOPPED) {
		ball->x = SCREEN_WIDTH / 2;
		ball->y = SCREEN_HEIGHT / 2;
		return;
	}

	// Store previous locations
	ball->pX = ball->x;
	ball->pY = ball->y;

	// Update current locations
	ball->x += game->ball->xVel * t_elapse;
	ball->y += game->ball->yVel * t_elapse;

	// Check x-crossings
	if (ball->x < BALLSIZE) {

		//game->state = RESET;
		ball->xVel = -(ball->xVel);
		score_update(game->pad2, 1);
		game->ball->x = SCREEN_WIDTH / 2;
		game->ball->y = SCREEN_HEIGHT / 2;
		game->ball->xVel += 1;
		game->ball->yVel += 1;

	}
	if (ball->x > SCREEN_WIDTH - BALLSIZE) {

		//game->state = RESET;
		score_update(game->pad1, 1);
		ball->xVel = -(ball->xVel);
		game->ball->x = SCREEN_WIDTH / 2;
		game->ball->y = SCREEN_HEIGHT / 2;
		game->ball->xVel += 1;
		game->ball->yVel += 1;

	}

	// Check y-crossings
	if (ball->y < BALLSIZE) {
		ball->yVel = -(ball->yVel);
	}
	if (ball->y > SCREEN_HEIGHT - BALLSIZE) {
		ball->yVel = -(ball->yVel);
	}

}

// Shutdown the game
void shutdown(game_context* game) {

	free(game->pad1);
	free(game->pad2);
	free(game->ball);
	game->ball = NULL;
	game->pad1 = NULL;
	game->pad2 = NULL;
	game->state = STOPPED;

	xil_printf("Game shut down.\n");

	return;
}

// This function is called repeatedly to update the game
void update(int t_elapse, game_context* game, graphics_context* gc) {

	// Pausing Check
	if (JSTK2_getBtns(game->player_1_joystick) == 2) {
		if (game->state == PAUSED) {
			game->state = RUNNING;
		} else if (game->state == RUNNING) {
			game->state = PAUSED;
			delay(1000);
		}
	} else if (JSTK2_getBtns(game->player_1_joystick) == 1) {
		game->state = STOPPED;
	}

	if (game->state == PAUSED) {
		return;
	}

	else {

		update_ball(game, game->state, t_elapse);
		render_ball(game, gc);

		// If JST == null, player is CPU
		if (game->player_1_joystick == NULL) {
			update_CPU_pad(game->pad1, game->ball, t_elapse, 0);
		} else {
			update_player_pad(game->player_1_joystick, game->pad1, game->ball,
					t_elapse);
		}

		if (game->player_2_joystick == NULL) {
			update_CPU_pad(game->pad2, game->ball, t_elapse, game->camera_control);
		} else {
			update_player_pad(game->player_2_joystick, game->pad2, game->ball,
					t_elapse);
		}

		check_interesections(game, gc);
		render_pads(game, gc);
		render_score(game, gc);

	}

}

void render_score(game_context* game, graphics_context* gc) {

	int scoreA = game->pad1->score;
	int scoreB = game->pad2->score;

	// Draw score rectangles for A
	drawRect(25, 25, 25 * (0 + scoreA), 10, game->pad1->r, game->pad1->g,
			game->pad1->b, gc);
	drawRect(25, 35, 25 * (0 + scoreB), 10, game->pad2->r, game->pad2->g,
			game->pad2->b, gc);

}

void clearScreen(graphics_context* gc) {

	int xcoi, ycoi;
	int iPixelAddr;
	int draw_width = 640 / 1;
	int draw_height = 480 / 1;

	for (xcoi = 0; xcoi < (draw_width * 3); xcoi += 3) {

		iPixelAddr = xcoi;

		for (ycoi = 0; ycoi < draw_height; ycoi++) {

			gc->frame[iPixelAddr] = 0;
			gc->frame[iPixelAddr + 1] = 0;
			gc->frame[iPixelAddr + 2] = 0;

			iPixelAddr += gc->stride;
		}

	}
	Xil_DCacheFlushRange((unsigned int ) gc->frame, DEMO_MAX_FRAME);

}

void render_pads(game_context* game, graphics_context* gc) {

	drawRect(game->pad1->pX, game->pad1->pY, game->pad1->w, game->pad1->h, 0, 0,
			0, gc);
	drawRect(game->pad1->xPos, game->pad1->yPos, game->pad1->w, game->pad1->h,
			game->pad1->r, game->pad1->g, game->pad1->b, gc);

	drawRect(game->pad2->pX, game->pad2->pY, game->pad2->w, game->pad2->h, 0, 0,
			0, gc);
	drawRect(game->pad2->xPos, game->pad2->yPos, game->pad2->w, game->pad2->h,
			game->pad2->r, game->pad2->g, game->pad2->b, gc);

}

void render_ball(game_context* game, graphics_context* gc) {

	drawRect(game->ball->pX, game->ball->pY, game->ball->size, game->ball->size,
			0, 0, 0, gc);
	drawRect(game->ball->x, game->ball->y, game->ball->size, game->ball->size,
			255, 0, 0, gc);

}

void print_state(game_context* g) {

	// Print the game state
	xil_printf("STATE: %d\n", g->state);

	// Print the ball's data
	xil_printf("BALL (X,Y) = (%d,%d)\n", g->ball->x, g->ball->y);
	xil_printf("BALL (vX,vY) = (%d,%d)\n", g->ball->xVel, g->ball->yVel);

	// Print the player's data
	xil_printf("PLAYER (X,Y) = (%d,%d)\n", g->pad1->xPos, g->pad1->yPos);
	// Print the CPU's data
	xil_printf("CPU (vX,vY) = (%d,%d)\n", g->pad2->xPos, g->pad2->yPos);

	xil_printf("--------------\n");

}

int setup_stopwatch(XTmrCtr* TimerCounter) {

	int Status = XTmrCtr_Initialize(TimerCounter, TMRCTR_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return 1;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly, use the 1st timer in the device (0)
	 */
	Status = XTmrCtr_SelfTest(TimerCounter, TMRCTR_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return 1;
	}

	/*
	 * Setup the timer counters.
	 */
	XTmrCtr_SetOptions(TimerCounter, TMRCTR_DEVICE_ID, 0);

	// Reset to count up from 0 every time the timer starts
	XTmrCtr_SetResetValue(TimerCounter, TMRCTR_DEVICE_ID, 0);

	return 0;

}

int start_stopwatch(XTmrCtr* TmrCtrInstancePtr) {

	XTmrCtr_Start(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
	return XTmrCtr_GetValue(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);

}

int end_stopwatch(XTmrCtr* TmrCtrInstancePtr) {

	int answer = XTmrCtr_GetValue(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
	XTmrCtr_Stop(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
	return answer;

}

int get_number() {

	#define SIZE 32
	char abc[4] = { 0, 0, 0, 0 };

	unsigned char userInput[SIZE] = { 0 };
	int n = 0;
	int flag = 0;

	while (flag != 2) {
		unsigned char answer = XUartLite_RecvByte(UART_BASEADDR);
		xil_printf("%c ", answer);
		if (answer == 13) {
			flag++;
			userInput[n] = answer;
			n++;
		} else if (n > SIZE) {
			flag = 2;
		} else {
			userInput[n] = answer;
			n++;
		}
	}
	xil_printf("\n");

	int idxA = -1, idxB = -1;
	for (int i = 0; i < SIZE; i++) {
		if (userInput[i] == 13) {
			if (idxA == -1) {
				idxA = i;
			} else {
				idxB = i;
				break;
			}
		}
	}

	if (idxA > 0 && idxB > 0) {

		int k = 0;
		for (int j = idxA + 1; j < idxA + (idxB - idxA); j++) {
			abc[k] = userInput[j];
			k++;
		}

	}

	return (abc[0]-48)*100 + (abc[1]-48)*10 + (abc[2]-48);


}


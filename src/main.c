#include "raylib.h"
#include <stdio.h>

typedef unsigned char b8;

// modifiable

#define WINDOW_WIDTH 		600
#define WINDOW_HEIGHT 		400

#define PELLET_SIZE 		10
#define PELLET_SPEED 		2.0f
#define PELLET_SPEED_ICRMT	0.25f
#define PLAYER_WIDTH		20
#define PLAYER_HEIGHT		80
#define PLAYER_SPEED		6.0f
#define INTIIAL_PELLET_POS 	(Vector2){0.0f, 0.0f}
#define INITIAL_PELLET_DIR 	(Vector2){1.0f, 1.0f}

// non-modifiable

#define PLAYER_MARGIN		PLAYER_WIDTH
#define PLAYER_SIZE 		(Vector2){(float)PLAYER_WIDTH, (float)PLAYER_HEIGHT}
#define INITIAL_P1_POS		(Vector2){(float)PLAYER_MARGIN, (float)(WINDOW_HEIGHT - PLAYER_HEIGHT) / 2.0f }
#define INITIAL_P2_POS		(Vector2){(float)(WINDOW_WIDTH - PLAYER_WIDTH) - (float)PLAYER_MARGIN, (float)(WINDOW_HEIGHT - PLAYER_HEIGHT) / 2.0f }

#define TOWARDS_P1 			-1.0f
#define TOWARDS_P2 			1.0f

typedef struct {
	Vector2 pellet_pos;
	Vector2 pellet_dir;
	float pellet_speed;
	Vector2 player1_pos;
	Vector2 player2_pos;
	b8 game_over;
} GameState;

void game_over(GameState* state) {
	if (state->pellet_dir.x == TOWARDS_P1) {
		state->game_over = (b8)1;
		DrawText("P1 LOSES", 20, 20, 36, WHITE);
	}
	if (state->pellet_dir.x == TOWARDS_P2) {
		state->game_over = (b8)1;
		DrawText("P2 LOSES", 20, 20, 36, WHITE);
	}
}

void bounce_off_walls(GameState* state) {
	if (state->pellet_pos.x > WINDOW_WIDTH - PELLET_SIZE || state->pellet_pos.x < 0) {
		game_over(state);
	}
	else if (state->pellet_pos.y > WINDOW_HEIGHT - PELLET_SIZE || state->pellet_pos.y < 0) {
		state->pellet_dir.y *= -1.0f;
	}
}

void handle_key_presses(GameState* state) {
	if (IsKeyDown(KEY_S) && state->player1_pos.y < WINDOW_HEIGHT - PLAYER_HEIGHT) { // p1 down
		state->player1_pos.y += PLAYER_SPEED;
	}
	if (IsKeyDown(KEY_W) && state->player1_pos.y > 0) { // p1 up
		state->player1_pos.y -= PLAYER_SPEED;
	}
	if (IsKeyDown(KEY_DOWN) && state->player2_pos.y < WINDOW_HEIGHT - PLAYER_HEIGHT) { // p2 down
		state->player2_pos.y += PLAYER_SPEED;
	}
	if (IsKeyDown(KEY_UP) && state->player2_pos.y > 0) { // p2 up
		state->player2_pos.y -= PLAYER_SPEED;
	}
}

void check_player_collisions(GameState* state) {
	Rectangle pellet = 
		{ state->pellet_pos.x, state->pellet_pos.y, PELLET_SIZE, PELLET_SIZE };
	Rectangle p1 = 
		{ state->player1_pos.x, state->player1_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
	Rectangle p2 = 
		{ state->player2_pos.x, state->player2_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT };

	if (CheckCollisionRecs(pellet, p1)) {
		if (state->pellet_dir.x == TOWARDS_P1) { // moving towards p1
			state->pellet_dir.x *= -1.0f;
			state->pellet_speed += PELLET_SPEED_ICRMT;
		}
	}

	if (CheckCollisionRecs(pellet, p2)) {
		if (state->pellet_dir.x == TOWARDS_P2) { // moving towards p2
			state->pellet_dir.x *= -1.0f;
			state->pellet_speed += PELLET_SPEED_ICRMT;
		}
	}
}

void initialise_game_state(GameState* state) {
	state->pellet_pos = INTIIAL_PELLET_POS;
	state->pellet_dir = INITIAL_PELLET_DIR;
	state->pellet_speed = PELLET_SPEED;
	state->player1_pos = INITIAL_P1_POS;
	state->player2_pos = INITIAL_P2_POS;
}

void on_frame(GameState* state) {
	BeginDrawing();
	ClearBackground(BLACK);

	if (state->game_over) {
		game_over(state);
	}
	else {
		// pellet
		DrawRectangle(
			state->pellet_pos.x += state->pellet_speed * state->pellet_dir.x,
			state->pellet_pos.y += state->pellet_speed * state->pellet_dir.y,
			PELLET_SIZE, PELLET_SIZE, WHITE
		);

		bounce_off_walls(state); // detect and change direction accordingly
		
		// players
		handle_key_presses(state);
		check_player_collisions(state);

		DrawRectangleV(state->player1_pos, PLAYER_SIZE, WHITE);
		DrawRectangleV(state->player2_pos, PLAYER_SIZE, WHITE);

	}	
	
	EndDrawing();
}

int main(void)
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
	SetTargetFPS(60);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raylib Pong");

	GameState state = (GameState){0};
	initialise_game_state(&state);
	
	while (!WindowShouldClose())
		on_frame(&state);

	CloseWindow();
	return 0;
}

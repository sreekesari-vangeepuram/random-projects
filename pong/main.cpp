#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//////// MACROS ////////
#define EXIT_SUCCESS 0
#define SDL_FAILURE  1
#define TTF_FAILURE  2

#define SCREEN_WIDTH  720
#define SCREEN_HEIGHT 720
#define FONT_SIZE     32

#define BALL_SIZE    16
#define BALL_SPEED   16
#define PADDLE_GAP   32
#define PADDLE_SPEED  9
#define PADDLE_WIDTH 12

#define LEFT_TURN  0
#define RIGHT_TURN 1

#define PI 3.14159265358979323846

///// NAMESPACE AUGMENTATION /////
using namespace std;

//// GLOBAL VARIABLES ////
SDL_Renderer *renderer;
SDL_Window   *window;

TTF_Font     *font;
SDL_Color    color;

SDL_Rect left_paddle,  // Player
		 right_paddle, // Computer
		 ball,
		 score_board;

float vel_x, vel_y; // Velocity of `ball`
string score; // To show score
int left_score, right_score, turn; // Individual scores & user turn
int fps, frame_count, last_frame;
bool ok = true;

//////// FUNCTION PROTOTYPES ////////
void update(void);
void user_input(void);
void render(void);
void show_score(string, int, int);
void play(void);

int main(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Couldn't initialize SDL: " << SDL_GetError() << endl;
		return SDL_FAILURE;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) < 0)
	{
		cout << "Failed to open " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << " window: " << SDL_GetError() << endl;
		return SDL_FAILURE;
	}

	SDL_SetWindowTitle(window, "PONG"); // Set window TITLE

	if (TTF_Init() < 0)
	{
		cout << "Couldn't Initialize TTF: " << TTF_GetError() << endl;
		return TTF_FAILURE;
	}

	font = TTF_OpenFont("./fonts/ProggySquareSZ.ttf", FONT_SIZE);

	if (!font)
	{
		cout << "Failed to open font file: " << TTF_GetError() << endl;
		return TTF_FAILURE;
	}

	color.r = color.g = color.b = 200; // rgb(200, 200, 200) -> Grayish-white color

	left_score = right_score = 0;

	left_paddle.w = PADDLE_WIDTH;
	left_paddle.h = SCREEN_HEIGHT / 4;

	left_paddle.x = PADDLE_GAP;
	left_paddle.y = (SCREEN_HEIGHT / 2) - (left_paddle.h / 2);

	right_paddle = left_paddle; // Both have same size
	right_paddle.x = SCREEN_WIDTH - right_paddle.w - PADDLE_GAP;

	ball.w = ball.h = BALL_SIZE; // Set the square ball size (width, height)

	play();

	int last_time = 0;
	while (ok)
	{
		last_frame = SDL_GetTicks();
		if (last_frame >= (last_time + 1000))
		{
			last_time = last_frame;
			fps = frame_count;
			frame_count = 0;
		}

		update();
		user_input();
		render();
	}

	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void update(void)
{
	if (SDL_HasIntersection(&ball, &left_paddle))
	{
		double rel, norm, bounce;
		rel = (left_paddle.y + (left_paddle.h / 2)) - (ball.y + (BALL_SIZE / 2));
		norm = rel / (left_paddle.h / 2); 
		bounce = norm * (5 * (PI / 2));
		vel_x = BALL_SPEED * cos(bounce);
		vel_y = BALL_SPEED * -sin(bounce);
	}

	if (SDL_HasIntersection(&ball, &right_paddle))
	{
		double rel, norm, bounce;
		rel = (right_paddle.y + (right_paddle.h / 2)) - (ball.y + (BALL_SIZE / 2));
		norm = rel / (right_paddle.h / 2); 
		bounce = norm * (5 * (PI / 2));
		vel_x = -BALL_SPEED * cos(bounce);
		vel_y = BALL_SPEED * -sin(bounce);
	}

	////// BALL CONDITIONS //////
	if (ball.y > right_paddle.y + (right_paddle.h / 2))
		right_paddle.y += PADDLE_SPEED;

	if (ball.y < right_paddle.y + (right_paddle.h / 2))
		right_paddle.y -= PADDLE_SPEED;

	///// BALL VELOCITY CONDITIONS /////
	if (ball.x <= 0 || ball.x + BALL_SIZE >= SCREEN_WIDTH)
	{
		if (ball.x <= 0) {
			right_score++;
		} else {
			left_score++;
		}

		play/*again*/();
	}

	if (ball.y <= 0 || ball.y + BALL_SIZE >= SCREEN_HEIGHT)
		vel_y = -vel_y; // Change direction w.r.t. Y-component


	///////// Stringify score /////////
	score = "YOU: " + to_string(left_score) + "   ::   "  + "COMPUTER: " + to_string(right_score);

	///// LEFT PADDLE CONDITIONS /////
	if (left_paddle.y < 0)
		left_paddle.y = 0;

	if (left_paddle.y + left_paddle.h > SCREEN_HEIGHT)
		left_paddle.y = SCREEN_HEIGHT - left_paddle.h;

	///// RIGHT PADDLE CONDITIONS /////
	if (right_paddle.y < 0)
		right_paddle.y = 0;

	if (right_paddle.y + right_paddle.h > SCREEN_HEIGHT)
		right_paddle.y = SCREEN_HEIGHT - right_paddle.h;


	ball.x += vel_x;
	ball.y += vel_y;

	return;
}

void user_input(void)
{
	SDL_Event event;
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event))
		if (event.type == SDL_QUIT)
			ok = false;

	// Checking some keyboard events
	if (keystates[SDL_SCANCODE_ESCAPE])
		ok = false;

	if (keystates[SDL_SCANCODE_UP])
		left_paddle.y -= PADDLE_SPEED;

	if (keystates[SDL_SCANCODE_DOWN])
		left_paddle.y += PADDLE_SPEED;


	return;
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
	SDL_RenderClear(renderer);

	frame_count++;
	int timer_fps = SDL_GetTicks() - last_frame;

	if (timer_fps < (1000 / 60))
		SDL_Delay((1000 / 60) - timer_fps);

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 200);

	SDL_RenderFillRect(renderer, &left_paddle);
	SDL_RenderFillRect(renderer, &right_paddle);
	SDL_RenderFillRect(renderer, &ball);

	show_score(score, FONT_SIZE + (SCREEN_WIDTH / 2), FONT_SIZE * 2);

	SDL_RenderPresent(renderer);

	return;
}

void show_score(string score_text, int sx, int sy)
{
	SDL_Surface *surface;
	SDL_Texture *texture;
	
	const char *text = score_text.c_str();

	surface = TTF_RenderText_Solid(font, text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	score_board.w = surface->w; // Scoreboard width
	score_board.h = surface->h; // Scoreboard height

	score_board.x = sx - score_board.w; // X coordinate of scoreboard
	score_board.y = sy - score_board.h; // Y coordinate of scoreboard

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &score_board);
	SDL_DestroyTexture(texture);

	return;
}

void play(void)
{
	// Player
	left_paddle.x  = PADDLE_GAP;
	left_paddle.y  = (SCREEN_WIDTH / 2) - (left_paddle.h / 2) ;

	// Computer
	right_paddle.x = SCREEN_HEIGHT - right_paddle.w - PADDLE_GAP;
	right_paddle.y = left_paddle.y;	

	switch (turn) {
	case LEFT_TURN:
		ball.x  = left_paddle.x + (left_paddle.w * 4);
		vel_x = BALL_SPEED / 2;
		break;

	case RIGHT_TURN:
		ball.x  = right_paddle.x - (right_paddle.w * 4);
		vel_x = -BALL_SPEED / 2;
		break;

	default: /* Do nothing! */
		cerr << "Unknown TURN_CODE encountered!" << endl;
		break;
	}

	ball.y = (SCREEN_HEIGHT / 2) - (BALL_SIZE / 2);
	vel_y = 0; // Changes only when it hits the paddle
	turn = !turn; // Shift turn

	return;
}

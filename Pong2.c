#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
// ********Functions*********
static Mix_Music *loadMusic(const char *path)
{
	Mix_Music *music = Mix_LoadMUS(path);
	return music;
}

void ptr(int val, char arr[11])
{
	sprintf(arr, "%i", val);
}

void blit(SDL_Renderer *s, SDL_Texture *img, SDL_Rect *rect)
{
	SDL_RenderCopy(s, img, NULL, rect);
}

int main(int argc, char *argv[])
{
	SDL_Delay(1500);
	// ********Initialising********
	int width = 0, height = 0;
	SDL_Init(SDL_INIT_EVERYTHING);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	TTF_Init();
	SDL_Window *window1 = SDL_CreateWindow("Pong", 0, 0, width, height, SDL_WINDOW_SHOWN);
	SDL_GetWindowSize(window1, &width, &height);
	SDL_DestroyWindow(window1);
	SDL_Window *window = SDL_CreateWindow("Pong", 0, 0, 1200, 800, SDL_WINDOW_SHOWN);
	// *******Constants*******
	bool clicked = 0, stop = 0;
	int click_x, click_y, v_max = height / 80;
	int speed_limit = 2 * width / (sqrt((width * width) + (height * height)) / v_max);
	SDL_Renderer *s = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture *ball_img = IMG_LoadTexture(s,
											"pong_assets/ball.png");
	SDL_Texture *padel_img = IMG_LoadTexture(s,
											 "pong_assets/padel.png");
	SDL_Texture *targ_img = IMG_LoadTexture(s,
											"pong_assets/targ.png");
	SDL_Texture *bg = IMG_LoadTexture(s,
									  "pong_assets/bg.jpeg");
	Mix_Music *die = loadMusic("pong_assets/die.wav");
	Mix_Music *strike = loadMusic("pong_assets/strike.wav");
	Mix_Music *point = loadMusic("pong_assets/point.wav");
	Mix_Music *hit = loadMusic("pong_assets/hit.wav");
	// ********Game objects********
	SDL_Rect ball;
	ball.h = height / 16;
	ball.w = ball.h;
	ball.x = width / 2;
	ball.y = 0;
	SDL_Rect padel;
	padel.h = height / 32;
	padel.w = width / 5;
	padel.x = width / 2 - (padel.w / 2);
	padel.y = height - padel.h;
	SDL_Rect targ;
	targ.h = ball.h * 2;
	targ.w = ball.w * 2;
	targ.x = ball.x - padel.w;
	targ.y = padel.h;
	SDL_Rect Message_rect;
	Message_rect.w = targ.h * 2;
	Message_rect.h = targ.h * 2;
	Message_rect.x = (width - Message_rect.w) / 2;
	Message_rect.y = 10;
	// *******Game variables********
	int po = 0,
		hpo = 0,
		ball_xv = (rand() % v_max) - (v_max / 2),
		ball_yv = sqrt(v_max * v_max - ball_xv * ball_xv),
		targ_xv = ball_xv = (rand() % (v_max / 2)) - (v_max / 4),
		targ_yv = sqrt(v_max * v_max - targ_xv * targ_xv) * ((rand() % 3) - 1);
	float padel_xv = 0;
	SDL_Event ev;
	char po_arr[11] = {'0'};
	char *po_ptr = po_arr;
	SDL_Color pink = {150, 255, 0};
	TTF_Font *Sans = TTF_OpenFont("/system/fonts/Roboto-Regular.ttf", 24);
	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, po_ptr, pink);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(s, surfaceMessage);
	// ********Game loop********
	while (1)
	{
		SDL_RenderClear(s);
		ball.x += ball_xv;
		ball.y += ball_yv;
		targ.x += targ_xv;
		targ.y += targ_yv;
		padel.x += padel_xv;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				SDL_DestroyTexture(targ_img);
				SDL_DestroyTexture(ball_img);
				SDL_DestroyTexture(padel_img);
				SDL_DestroyTexture(bg);
				SDL_DestroyRenderer(s);
				SDL_DestroyWindow(window);
				SDL_Quit();
			}
			if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				clicked = 1;
			}
			if (ev.type == SDL_MOUSEBUTTONUP)
			{
				clicked = 0;
			}
		}
		if (clicked)
		{
			SDL_GetMouseState(&click_x, &click_y);
			if ((abs(click_x - Message_rect.x - (Message_rect.w / 2)) <(Message_rect.w / 2)) &&(abs(click_y - Message_rect.y - (Message_rect.h / 2)) <(Message_rect.h / 2)))
			{
				ptr(hpo, po_ptr);
				blit(s, bg, NULL);
				blit(s, ball_img, &ball);
				blit(s, padel_img, &padel);
				blit(s, targ_img, &targ);
				surfaceMessage = TTF_RenderText_Solid(Sans, po_ptr, pink);
				Message = SDL_CreateTextureFromSurface(s, surfaceMessage);
				blit(s, Message, &Message_rect);
				SDL_RenderPresent(s);
				stop = 1;
				while (stop)
				{
					SDL_Delay(20);
					while (SDL_PollEvent(&ev))
					{
						if (ev.type == SDL_MOUSEBUTTONDOWN)
						{
							SDL_GetMouseState(&click_x, &click_y);
							if ((abs(click_x - Message_rect.x - (Message_rect.w / 2)) <(Message_rect.w / 2)) &&(abs(click_y - Message_rect.y - (Message_rect.h / 2)) <(Message_rect.h / 2)))
							{
								stop = 0;
							}
						}
					}
				}
				ptr(po, po_ptr);
				clicked = 0;
			}
			else if (click_x > width / 2)
			{
				if (padel_xv > 0)
				{
					padel_xv += ((float)(speed_limit * 2) - padel_xv) / 32;
				}
				else
				{
					padel_xv += (float)(speed_limit / 12);
				}
			}
			else
			{
				if (padel_xv < 0)
				{
					padel_xv -= ((float)(speed_limit * 2) + padel_xv) / 32;
				}
				else
				{
					padel_xv -= (float)(speed_limit / 12);
				}
			}
		}
		if (ball.x < 0)
		{
			ball_xv *= -1;
			ball.x = 0;
		}
		if (ball.x + ball.w > width)
		{
			ball_xv *= -1;
			ball.x = width - ball.w;
		}
		if ((ball.y >= padel.y - ball.h) && (ball.y <= padel.y - padel.h + v_max))
		{
			if (((padel.x + padel.w) > (ball.x + ball.h / 2))&&((ball.x + ball.h / 2) > padel.x))
			{
				Mix_PlayMusic(hit, 1);
				ball_xv += padel_xv / 3; // Friction
				if (abs(ball_xv) >= v_max * 9 / 10)
				{
					ball_xv = (ball_xv / abs(ball_xv)) * v_max * 8 / 10;
				}
				ball_yv = sqrt(v_max * v_max - ball_xv * ball_xv);
				ball_yv *= -1;
			}
		}
		else if (ball.y < 0)
		{
			if (po >= 1)
			{
				po--;
			}
			ptr(po, po_ptr);
			ball_yv *= -1;
			targ_xv = (rand() % (v_max * 2)) - (v_max);
			targ_yv = sqrt((v_max * v_max) - (targ_xv * targ_xv)) * (rand() % 3 - 1);
			if (targ_xv)
			{
				targ_xv = ((float)(targ_xv) + ((abs(targ_xv) / targ_xv) * ((float)(v_max) / 4))) / 3;
			}
			if (targ_yv)
			{
				targ_yv = ((float)(targ_yv) + ((abs(targ_yv) / targ_yv) * ((float)(v_max) / 4))) / 2;
			}
		}
		if (ball.y > height)
		{
			Mix_PlayMusic(die, 1);
			if (po >= 20)
			{
				po -= 20;
			}
			else
			{
				po = 0;
			}
			ptr(po, po_ptr);
			ball.x = width / 2;
			ball.y = 0;
			ball_xv = (rand() % v_max) - 5;
			ball_yv = sqrt(v_max * v_max - ball_xv * ball_xv);
		}
		if (padel.x < 0)
		{
			padel.x = 0;
			padel_xv = 0;
		}
		else if (padel.x + padel.w > width)
		{
			padel.x = width - padel.w;
			padel_xv = 0;
		}
		if (targ.x < 0)
		{
			targ_xv *= -1;
			targ.x = 0;
		}
		else if (targ.x + targ.w > width)
		{
			targ_xv *= -1;
			targ.x = width - targ.w;
		}
		if (targ.y < 0)
		{
			targ_yv *= -1;
			targ.y = 0;
		}
		else if (targ.y + targ.h > height)
		{
			targ.y = height - targ.h;
			targ_yv *= -1;
		}
		if ((targ.y >= padel.y - targ.h)&&(targ.y <= padel.y - padel.h + v_max))
		{
			if (((padel.x + padel.w) > (targ.x + targ.h / 2))&&((targ.x + targ.h / 2) > padel.x))
			{
				if (po >= 10)
				{
					po -= 10;
				}
				else
				{
					po = 0;
				}
				ptr(po, po_ptr);
				Mix_PlayMusic(strike, 1);
				targ.x = rand() % (width - targ.w);
				targ.y = padel.h;
				targ_xv = (rand() % (v_max * 2)) - (v_max);
				targ_yv = sqrt((v_max * v_max) - (targ_xv * targ_xv)) * (rand() % 3 - 1);
				if (targ_xv)
				{
					targ_xv =
						((float)(targ_xv) + ((abs(targ_xv) / targ_xv) * ((float)(v_max) / 4))) / 3;
				}
				if (targ_yv)
				{
					targ_yv =
						((float)(targ_yv) + ((abs(targ_yv) / targ_yv) * ((float)(v_max) / 4))) / 2;
				}
			}
		}
		if (sqrt(((ball.x + (ball.w / 2) - targ.x - (targ.w / 2)) * (ball.x + (ball.w / 2) - targ.x -
																	 (targ.w / 2)) +
				  (ball.y +
				   (ball.h / 2) -
				   targ.y -
				   (targ.h / 2)) *
					  (ball.y + (ball.h / 2) - targ.y - (targ.h / 2)))) < (ball.h + targ.h) / 2)
		{
			Mix_PlayMusic(point, 1);
			po += 10;
			ptr(po, po_ptr);
			if (po > hpo)
			{
				hpo = po;
			}
			targ.x = rand() % (width - targ.w);
			targ.y = padel.h;
			targ_xv = (rand() % (v_max * 2)) - (v_max);
			targ_yv = sqrt((v_max * v_max) - (targ_xv * targ_xv)) * (rand() % 3 - 1);
			if (targ_xv)
			{
				targ_xv = ((float)(targ_xv) + ((abs(targ_xv) / targ_xv) * ((float)(v_max) / 4))) / 3;
			}
			if (targ_yv)
			{
				targ_yv = ((float)(targ_yv) + ((abs(targ_yv) / targ_yv) * ((float)(v_max) / 4))) / 2;
			}
		}
		blit(s, bg, NULL);
		blit(s, ball_img, &ball);
		blit(s, padel_img, &padel);
		blit(s, targ_img, &targ);
		surfaceMessage = TTF_RenderText_Solid(Sans, po_ptr, pink);
		Message = SDL_CreateTextureFromSurface(s, surfaceMessage);
		blit(s, Message, &Message_rect);
		SDL_RenderPresent(s);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
		SDL_Delay(15);
	}
	return 0;
}
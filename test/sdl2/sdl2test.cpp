// This is not a good Pong implementation.
// This is not even a mediocre Pong implementation.
// This is just a bad Pong implementation.
// But it acts as a good sanity test of the compiler.

#include <SDL.h>

#ifdef main
#undef main
#endif

SDL_Rect paddleBottom, paddleTop, ball;
int velX = 8, velY = 8;

int centreX(const SDL_Rect & rect) {
	return rect.x + (rect.w / 2);
}

void advCore(int mul, int div) {
	ball.x += (velX * mul) / div;
	ball.y += (velY * mul) / div;
}

enum kBallCollider {
	kBallCollider_None,
	kBallCollider_PaddleBottom,
	kBallCollider_PaddleTop,
	kBallCollider_Wall
};

kBallCollider isBallColliding() {
	SDL_Rect temp;
	if (SDL_IntersectRect(&paddleBottom, &ball, &temp) == SDL_TRUE)
		return kBallCollider_PaddleBottom;
	if (SDL_IntersectRect(&paddleTop, &ball, &temp) == SDL_TRUE)
		return kBallCollider_PaddleTop;
	if (ball.x < 0 || ball.x > (800 - 16))
		return kBallCollider_Wall;
	return kBallCollider_None;
}

void advFull() {
	advCore(1, 1);
	kBallCollider coll = isBallColliding();
	if (coll != kBallCollider_None) {
		SDL_Rect ballBackup = ball;
		int step = 1;
		while (step < 64) {
			ball = ballBackup;
			advCore(-(step++), 8);
			kBallCollider newColl = isBallColliding();
			if (newColl == kBallCollider_None)
				break;
			// in case of multiple hits
			coll = newColl;
		}
		if (coll == kBallCollider_PaddleBottom) {
			velX = centreX(ball) - centreX(paddleBottom);
			velX /= 4;
			velY = (velY * -8) / 6;
		} else if (coll == kBallCollider_PaddleTop) {
			velX = centreX(ball) - centreX(paddleTop);
			velX /= 4;
			velY = (velY * -8) / 6;
		} else if (coll == kBallCollider_Wall) {
			velX = -velX;
		}
	}
}

int main(int argc, char ** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
	paddleTop = {400 - (192 / 2), 0, 192, 32};
	paddleBottom = {paddleTop.x, 600 - 32, paddleTop.w, paddleTop.h};
	ball = {400 - 8, 300 - 8, 16, 16};
	bool a = false, d = false;
	while (1) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return 0;
			} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) {
				a = true;
			} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) {
				d = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_a) {
				a = false;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_d) {
				d = false;
			}
		}
		// input
		if (a) {
			paddleBottom.x -= 32;
			paddleTop.x -= 32;
		}
		if (d) {
			paddleBottom.x += 32;
			paddleTop.x += 32;
		}
		// physics
		advFull();
		if (ball.y >= 600 || ball.y <= -16) {
			// out of bounds
			ball.x = 400 - 8;
			ball.y = 300 - 8;
			velX = 8;
			velY = 8;
		}
		// render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &paddleTop);
		SDL_RenderFillRect(renderer, &paddleBottom);
		SDL_RenderFillRect(renderer, &ball);
		SDL_RenderPresent(renderer);
		SDL_Delay(50);
	}
	return 0;
}

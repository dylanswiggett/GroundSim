#include <iostream>
#include <cstdlib>
#include <ctime>

#include "SDL2/SDL.h"

#include "Ground.hpp"
#include "GroundSim.hpp"

// Window visible at startup.
#define SDL_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define SDL_FLAGS SDL_INIT_EVERYTHING
#define SDL_DEFAULT_WIDTH 1000
#define SDL_DEFAULT_HEIGHT 800
#define DEFAULT_SCALE 4
#define FPS 60

using namespace std;

int init_SDL(SDL_Window **w, SDL_Renderer **r) {
  int res;

  res = SDL_Init(SDL_FLAGS);
  if (res) {
    cout << "Failed to initialize SDL context." << endl;
    cout << SDL_GetError() << endl;
    return -1;
  }

  res = SDL_CreateWindowAndRenderer(SDL_DEFAULT_WIDTH, SDL_DEFAULT_HEIGHT,
				    SDL_WINDOW_FLAGS, w, r);

  if (res) {
    cout << "Failed to initialize SDL window or renderer." << endl;
    cout << SDL_GetError() << endl;
    return -1;
  }

  // SUCCESS
  return -1;
}

void destroy_SDL() {
  SDL_Quit();
}

int runGame() {
  bool running, drawing, paused, erasing;
  char const* err_msg = NULL;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  int window_w, window_h;

  GroundSim *sim;

  // Setup
  if (!init_SDL(&window, &renderer)) {
    cout << "Exiting." << endl;
    return 1;
  }
  paused = true;
  running = true;
  drawing = erasing = false;

  window_w = SDL_DEFAULT_WIDTH;
  window_h = SDL_DEFAULT_HEIGHT;

  int map_w = window_w / DEFAULT_SCALE;
  int map_h = window_h / DEFAULT_SCALE;

  sim = new GroundSim(map_w, map_h);
  cout << "Making map of size " << map_w << "x" << map_h << "." << endl;

  // Initial map
  for (int x = 0; x < map_w; x++) {
    for (int y = 0; y < map_h / 2; y++) {
      Ground *g = new Ground();
      sim->setGround(x, y, g);
    }
  }

  int c_x = 0;
  int c_y = 0;

  try {
    srand(time(NULL));

    // Main loop
    while (running) {
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
        case SDL_QUIT:
          running = false;
	  break;
	case SDL_MOUSEBUTTONDOWN:
	  drawing = true;
	  if (((SDL_MouseButtonEvent*)&event)->button == SDL_BUTTON_LEFT) {
	    drawing = true;
	    erasing = false;
	  } else if (((SDL_MouseButtonEvent*)&event)->button == SDL_BUTTON_RIGHT) {
	    drawing = true;
	    erasing = true;
	  }
	  break;
	case SDL_MOUSEBUTTONUP:
	  drawing = false;
	  break;
	case SDL_KEYDOWN:
	  SDL_KeyboardEvent *e = (SDL_KeyboardEvent*)&event;
	  if (e->keysym.sym == SDLK_SPACE)
	    paused = !paused;
	}
      }

      SDL_SetRenderDrawColor(renderer, 0,0,0,0);
      SDL_RenderClear(renderer);

      SDL_GetWindowSize(window, &window_w, &window_h);

      if (drawing) {
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);
	for (int x = m_x - 3; x <= m_x + 3; x++) {
	  for (int y = m_y - 3; y <= m_y + 3; y++) {
	    if (x >= 0 && y >= 0 && x < window_w && y < window_h) {
	      Ground *g = NULL;
	      if (!erasing) {
		g = new Ground();
	      }
	      sim->setGround(x / DEFAULT_SCALE, (window_h - y - 1) / DEFAULT_SCALE, g);
	    }
	  }
	}
      }

      sim->draw(window_w, window_h, c_x, c_y, DEFAULT_SCALE, renderer);

      //for (int i = 0; i < 30; i++)
	sim->updateForces();
      
      if (paused) {
	SDL_Rect r;
	r.w = 10;
	r.h = 40;
	r.y = 20;
	r.x = window_w - 50;
	SDL_SetRenderDrawColor(renderer, 150,150,150,255);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	SDL_RenderDrawRect(renderer, &r);
	r.x = window_w - 30;
	SDL_SetRenderDrawColor(renderer, 150,150,150,255);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	SDL_RenderDrawRect(renderer, &r);
      } else {
	sim->update();
      }

      SDL_RenderPresent(renderer);
      SDL_Delay(1000.0 / FPS);
    }

  } catch (char const *msg) {
    err_msg = msg;
  }

  // Cleanup
  destroy_SDL();

  if (err_msg != NULL)
    throw err_msg;
  else
    return 0;
}

/* Top level just prints out any uncaught string error messages */
int main(int argc, char **argv) {
  try {
    return runGame();
  } catch (char const *msg) {
    cout << "Terminating after uncaught exception:\n> " << msg << endl;
    return -1;
  }
}

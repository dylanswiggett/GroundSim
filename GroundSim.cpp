#include <iostream>

#include "SDL2/SDL.h"

#include "GroundSim.hpp"

using namespace std;

GroundSim::GroundSim(int w, int h)
{
  w_ = w;
  h_ = h;
  
  map = new Ground**[w];
  for (int x = 0; x < w; x++) {
    map[x] = new Ground*[h];
  }

  clear_map();
}

GroundSim::~GroundSim()
{
  for (int x = 0; x < w_; x++) {
    for (int y = 0; y < h_; y++) {
      if (map[x][y])
	delete map[x][y];
      // Don't need to do the same for next, since it contains the same pointers.
    }
    delete [] map[x];
  }

  delete [] map;
}

void GroundSim::clear_map()
{
  for (int x = 0; x < w_; x++) {
    for (int y = 0; y < h_; y++) {
      map[x][y] = NULL;
    }
  }
}

void GroundSim::setGround(int x, int y, Ground *g)
{
  if (x < 0 || x >= w_ || y < 0 || y >= h_)
    throw "Tried to place ground outside map.";

  if (map[x][y] != NULL)
    delete map[x][y];

  g->setP(x, y);
  map[x][y] = g;
}

void GroundSim::updateForces()
{
  // Top->Bottom
  for (int x = 0; x < w_; x++) {
    for (int y = h_ - 1; y >= 0; y--) {
      if (map[x][y] != NULL) {
	map[x][y]->updateF(map, w_, h_);
      }
    }
  }

  // Bottom->Top
  for (int x = 0; x < w_; x++) {
    for (int y = 0; y < h_; y++) {
      if (map[x][y] != NULL) {
	map[x][y]->updateF(map, w_, h_);
      }
    }
  }
}


void GroundSim::update()
{
  for (int x = 0; x < w_; x++) {
    for (int y = 0; y < h_; y++) {
      if (map[x][y] != NULL) {
	map[x][y]->updateP(map, w_, h_);
      }
    }
  }
}

void GroundSim::draw(int w_w, int w_h, int c_x, int c_y, float scale,
		     SDL_Renderer *renderer)
{
  SDL_Rect rect;
  rect.w = scale;
  rect.h = scale;

  // TODO: Add actual colors.

  for (int x = c_x; x < w_w / scale + 1 + c_x; x++) {
    for (int y = c_y; y < w_h / scale + 1 + c_y; y++) {
      if (x >= 0 && x < w_ && y >= 0 && y < h_) {
	// We're inside the map.
	if (map[x][y] != NULL) {
	  // And we've got ground to draw!
	  rect.x = scale * (x - c_x);
	  rect.y = scale * (y - c_y);

	  // Adjust for inverted y
	  rect.y = w_h - rect.y - 1 - scale;

	  if (map[x][y]->isFalling()) {
	    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	    SDL_RenderFillRect(renderer, &rect);
	  } else {
	    SDL_SetRenderDrawColor(renderer,
				   255,
				   255 * map[x][y]->getForceDown(),
				   map[x][y]->isActive() ? 255 : 0,
				   255);
	    SDL_RenderDrawRect(renderer, &rect);
	  }
	}
      }
    }
  }
}

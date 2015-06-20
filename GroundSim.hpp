#ifndef GROUND_SIM_H_
#define GROUND_SIM_H_

#include "SDL2/SDL.h"

#include "Ground.hpp"

class GroundSim {
public:
  GroundSim(int w, int h);
  virtual ~GroundSim();

  void setGround(int x, int y, Ground *g);

  void updateForces();
  void update();
  void draw(int w_w, int w_h, int c_x, int c_y, float scale,
	    SDL_Renderer *renderer);
private:
  void clear_map();
  
  Ground ** *map;
  int w_, h_;
};

#endif // GROUND_SIM_H_

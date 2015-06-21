#include <iostream>

#include "Ground.hpp"

using namespace std;

Ground::Ground()
{
  x = y = 0;
  forceUp = forceDown = 0;
  mass = 1;
  active = true;
  falling = false;
}

Ground::~Ground()
{
  // Nada! (yet)
}

void Ground::setP(int newX, int newY)
{
  // TODO: Update neighbor info?
  x = newX;
  y = newY;
}

void Ground::updateF(Ground ***map, int w, int h)
{
  // TODO
  addF(0, GRAVITY);

  Ground *below = getAt(map, 0, -1, w, h);
  Ground *above = getAt(map, 0, 1, w, h);
  Ground *left = getAt(map, -1, 0, w, h);
  Ground *right = getAt(map, 1, 0, w, h);

  float f_lu, f_ru, f_uu, f_du,
    f_ld, f_rd, f_ud, f_dd;

  f_lu = f_ru = f_uu = f_du = f_ld = f_rd = f_ud = f_dd = 0;

  if (below) {
    f_dd += 1;
  }

  if (above) {
    f_uu += 1;
    f_ud += .8;
  }

  if (left) {
    f_ld += .3;
    f_lu += .3;
  }

  if (right) {
    f_rd += .3;
    f_ru += .3;
  }

  float total_u = f_lu + f_ru + f_uu + f_du;
  float total_d = f_ld + f_rd + f_ud + f_dd;

  if (total_u > 1) {
    f_lu /= total_u;
    f_ru /= total_u;
    f_du /= total_u;
    f_uu /= total_u;
    total_u = 1;
  }

  if (total_d > 1) {
    f_ld /= total_d;
    f_rd /= total_d;
    f_dd /= total_d;
    f_ud /= total_d;
    total_d = 1;
  }

  float damping = .97;
  float fu = forceUp * damping;
  float fd = forceDown * damping;

  if (above) above->addF(fu * f_uu, fd * f_ud);
  if (below) below->addF(fu * f_du, fd * f_dd);
  if (left)   left->addF(fu * f_lu, fd * f_ld);
  if (right) right->addF(fu * f_ru, fd * f_rd);
  
  addF(-forceUp * total_u, -forceDown * total_d);

  if (falling && forceDown > FALL_THRESH)
    forceDown = FALL_THRESH;

  /*
  float subtract = 0;

  if (below != NULL) {
    below->addF(0, forceDown);
    addF(0, -forceDown);
  } else if (y == 0) {
    addF(2 * forceDown, 0);
  }

  if (left != NULL) {
    subtract += .31;
    left->addF(forceUp * .3, forceDown * .3);
  }
  if (right != NULL) {
    subtract += .31;
    right->addF(forceUp * .3, forceDown * .3);
  }

  addF(forceUp * -subtract, forceDown * -subtract);

  if (above != NULL) {
    above->addF(forceUp, 0);
    addF(-forceUp, 0);
  } else {
    addF(-forceUp, 0);
  }
  */
}

void Ground::addF(float upF, float downF)
{
  forceUp += upF;
  forceDown += downF;
}

Ground *Ground::getAt(Ground ***map, int dx, int dy, int w, int h)
{
  if (y + dy >= 0 && x + dx >= 0 && x + dx < w && y + dy < h)
    return map[x + dx][y + dy];
  return NULL;
}

bool Ground::tryMoveTo(Ground ***map, int dx, int dy)
{
  if (map[x + dx][y + dy] != NULL)
    return false;

  map[x + dx][y + dy] = this;
  map[x][y] = NULL;
  x += dx;
  y += dy;
  return true;
}

void Ground::updateP(Ground ***map, int w, int h)
{
  if (forceUp > forceDown) {
    forceUp -= forceDown;
    forceDown = 0;
  } else if (forceDown > forceUp) {
    forceDown -= forceUp;
    forceUp = 0;
  }

  if (forceDown > 255)
    forceDown = 255;

  if (y == 0 || getAt(map, 0, -1, w, h) != NULL)
    falling = false;
  else if (forceDown > FALL_THRESH)
    falling = true;

  if (falling)
    tryMoveTo(map, 0, -1);
}

void Ground::getP(int *x_out, int *y_out)
{
  *x_out = x;
  *y_out = y;
}

float Ground::getForceUp()
{
  return forceUp;
}

float Ground::getForceDown()
{
  return forceDown;
}

bool Ground::isActive()
{
  return active;
}

bool Ground::isFalling()
{
  return falling;
}

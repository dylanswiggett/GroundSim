#ifndef GROUND_H_
#define GROUND_H_

#define GRAVITY 1
#define FALL_THRESH 25

class Ground {
private:
  int x, y;
  float forceUp, forceDown;
  float mass;
  bool active, falling;
public:
  Ground();
  virtual ~Ground();

  void setP(int newX, int newY);
  void updateF(Ground ***map, int w, int h);
  void updateP(Ground ***map, int w, int h);

  void getP(int *x, int *y);
  float getForceUp();
  float getForceDown();
  bool isActive();
  bool isFalling();
private:
  void addF(float upF, float downF);
  Ground *getAt(Ground ***map, int dx, int dy, int w, int h);
  bool tryMoveTo(Ground ***map, int dx, int dy);
};

#endif // GROUND_H_

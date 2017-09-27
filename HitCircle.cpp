#include "HitCircle.h"

HitCircle::HitCircle(unsigned x, unsigned y, unsigned startTime,  unsigned type, unsigned endTime = 0)
  : HitObject(x, y, startTime, endTime, type) {}


HitCircle::~HitCircle() {

}

void HitCircle::hit() {

}

#include "collisiondetection.h"

using namespace HybridAStar;

CollisionDetection::CollisionDetection() {
  this->grid = nullptr;
  // 生成LUT
  Lookup::collisionLookup(collisionLookup);
}

bool CollisionDetection::configurationTest(float x, float y, float t) const {
  int X = (int)x;
  int Y = (int)y;
  // 1m cell 10 等份
  int iX = (int)((x - (long)x) * Constants::positionResolution);
  iX = iX > 0 ? iX : 0;
  int iY = (int)((y - (long)y) * Constants::positionResolution);
  iY = iY > 0 ? iY : 0;
  // 0.0872 rad = 10 度；36 等份
  int iT = (int)(t / Constants::deltaHeadingRad);
  int idx = iY * Constants::positionResolution * Constants::headings + iX * Constants::headings + iT;
  int cX;
  int cY;

  for (int i = 0; i < collisionLookup[idx].length; ++i) {
    // TODO： X 是基础偏移， idx 代表具体1/100 细分cell 位置
    cX = (X + collisionLookup[idx].pos[i].x);
    cY = (Y + collisionLookup[idx].pos[i].y);

    // make sure the configuration coordinates are actually on the grid
    if (cX >= 0 && (unsigned int)cX < grid->info.width && cY >= 0 && (unsigned int)cY < grid->info.height) {
      if (grid->data[cY * grid->info.width + cX]) {
        return false;
      }
    }
  }

  return true;
}
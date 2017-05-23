#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

class TankObject;
class TransportObject;
class ThreeD_Camera;

void SetupAnimation(TankObject &duskTank, TankObject &dawnTank, TransportObject &duskTransport, TransportObject &dawnTransport, ThreeD_Camera &camera);

#endif // ANIMATION_H

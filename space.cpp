#include "space.h"

//Not exactly accurate to physics, I don't think, but oh well
void handleCollision(Asteroid* ast1, Asteroid* ast2) {
	double a1s = ast1->speed;
	double a2s = ast2->speed;
	double a1h = ast1->heading;
	double a2h = ast2->heading;
	double relspeed = addVectorsScalarPart(
			a1s, a1h, 
			a2s, a2h);
	double relangle = addVectorsAnglePart(
			a1s, a1h, 
			a2s, a2h);
	double impulse = relspeed / (1/ast1->mass + 1/ast1->mass);
	double a1dv = ELASTICITY * impulse / ast1->mass;
	double a2dv = ELASTICITY * -impulse / ast2->mass;
	ast1->speed = addVectorsScalarPart(a1s, a1h, a1dv, relangle);
	ast1->heading = addVectorsAnglePart(a1s, a1h, a1dv, relangle);
	ast2->speed = addVectorsScalarPart(a2s, a2h, a2dv, relangle);
	ast2->heading = addVectorsAnglePart(a2s, a2h, a2dv, relangle);
}

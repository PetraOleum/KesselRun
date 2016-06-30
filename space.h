#ifndef SPACE_H
#define SPACE_H

#include <cmath>

#define GRAVITATIONAL_CONSTANT 0.0000000001

/// @brief Struct for a particle, e.g. black hole, asteroid, etc
struct Asteroid {
	/// @brief Mass of the object
	double mass;
	/// @brief Linear speed of the object
	double speed;
	/// @brief Heading of the object (in radians)
	double heading;
	/// @brief Radius of the object
	double radius;
	/// @brief X-coordinate of the object
	double x;
	/// @brief Y-coordinate of the object
	double y;
};

/// @brief Calculate the distance between the centres of two Asteroids
///
/// @param ast1 Asteroid number one
/// @param ast2 Asteroid number two
///
/// @return The distance
inline double distance(const Asteroid& ast1, const Asteroid ast2) {
	return hypot(ast1.x - ast2.x, ast1.y - ast2.y);
}

/// @brief Determines if two circular Asteroids are overlapping
///
/// @param ast1 Asteroid number one
/// @param ast2 Asteroid number two
///
/// @return True if overlapping, false if only touching exactly or not touching at all
inline bool collision(const Asteroid& ast1, const Asteroid ast2) {
	return distance(ast1, ast2) < ast1.radius + ast2.radius;
}

/// @brief Calculate the amount of acceleration due to gravity that 'from' is creating on 'on'
///
/// @param from The Asteroid that is producing the gravitational force
/// @param on The Asteroid that the force is acting upon
///
/// @return The (scalar) value of the acceleration
inline double gravitationalAcceleration(const Asteroid& from, const Asteroid& on) {
	return GRAVITATIONAL_CONSTANT * from.mass / pow(distance(from, on), 2);
}

/// @brief Calculate the amount of gravitational force that 'from' is creating on 'on' - note that, unlike acceleration, this is symmetrical
///
/// @param from The Asteroid that is producing the gravitational force
/// @param on The Asteroid that the force is acting upon
///
/// @return The (scalar) value of the force
inline double gravitationalForce(const Asteroid& from, const Asteroid& on) {
	return gravitationalAcceleration(from, on) * on.mass;
}

/// @brief Determine the heading between two Asteroids in radians. Useful for angle part of vector
///
/// @param from The Asteroid whose POV is being used
/// @param to The other Asteroid
///
/// @return The heading, in radians
inline double headingTo(const Asteroid& from, const Asteroid& to) {
	return atan2(to.y - from.y, to.x - from.x);
}

/// @brief Calculate the new speed and heading for an Asteroid, after applying a change in velocity
///
/// @param ast The original asteroid (constant)
/// @param speed The speed part of the delta-v vector
/// @param heading The angle part of the vector
///
/// @return Return a new Asteroid based on the original, with modified velocity
inline Asteroid deltaV(const Asteroid& ast, double speed, double heading) {
	Asteroid na = ast;
	na.speed = sqrt(pow(ast.speed, 2) + pow(speed, 2) + 2 * ast.speed * speed * cos(heading - ast.heading));
	na.heading = ast.heading + atan2(speed * sin(heading - ast.heading), ast.speed + speed * cos(heading - ast.heading));
	return na;
}


#endif

#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <list>
#include "Util.h"

// Represents the character (as a vine), currently incomplete.
struct Vine
{
	ScenePrimitive *tip;
	Vector3 dest;
    double radius;
    double speed;

	Vine();
	Vine(CollisionScene *scene, Vector3 pos, double radius);
	
	ScenePrimitive *getTip() const;
	Vector3 getPos() const;
	void setPos(Vector3 value);
	Vector3 getDest() const;
	void setDest(Vector3 value);
    double getRadius() const;

	void move(Vector3 delta);
	
	void update(Number elapsed);

	void addToCollisionScene(CollisionScene *scene);

	void removeFromCollisionScene(CollisionScene * scene);
};
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

// Represents the character (as a vine), currently incomplete.
struct Vine
{
	ScenePrimitive *tip;
	Vector3 dest;

	Vine();
	Vine(CollisionScene *scene, Vector3 pos, Number length, Number radius);

	Vector3 getDest() const;
	void setDest(Vector3 value);

	void move(Vector3 delta);
	
	void Vine::update(Number elapsed);

	void addToCollisionScene(CollisionScene *scene);

	void removeFromCollisionScene(CollisionScene * scene);
};
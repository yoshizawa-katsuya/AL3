#include "Lerp.h"

Vector2 Leap(const Vector2& p0, const Vector2& p1, float t)
{

	Vector2 anser;

	anser.x = (1 - t) * p0.x + t * p1.x;
	anser.y = (1 - t) * p0.y + t * p1.y;

	return anser;
}

Vector3 Leap(const Vector3& p0, const Vector3& p1, float t)
{

	Vector3 anser;

	anser.x = (1 - t) * p0.x + t * p1.x;
	anser.y = (1 - t) * p0.y + t * p1.y;
	anser.z = (1 - t) * p0.z + t * p1.z;

	return anser;
}
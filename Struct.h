#pragma once
#include <Vector2.h>
#include <Vector3.h>

/*
struct intVector2 {
	int x;
	int y;
};

struct Vector2 {
	float x;
	float y;
};

struct Vector3{
	float x;
	float y;
	float z;

	Vector3& operator*=(float s) { x *= s;  y *= s; z *= s; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator/=(float s) { x /= s;  y /= s; z /= s; return *this; }

};

struct Matrix3x3
{
	float m[3][3];
};

struct Matrix4x4
{
	float m[4][4];
};
*/
struct Ball
{

	Vector2 pos;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float radius;
	unsigned int color;

};

struct Box
{
	Vector2 pos;
	Vector2 size;
	Vector2 valocity;
	Vector2 acceleration;
	float mass;
	unsigned int color;
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Plane {
	Vector3 normal;	//法線
	float distance;	//距離
};

struct Triangle {
	Vector3 vertices[3];	//頂点
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;	//中心点
	Vector3 orientations[3];	//座標軸。正規化・直交必須
	Vector3 size;	//座標軸方向の長さの半分。中心から面までの距離
};
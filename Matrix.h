#pragma once
//#include "Struct.h"
#include <assert.h>
//#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include "Matrix4x4.h"
#include "Vector3.h"
/*
Matrix3x3 MakeTranslateMatrix(Vector2 translate);

Matrix3x3 Invarse(Matrix3x3 matrix);

Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);

Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height);

Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2);

Vector2 Transform(Vector2 vector, Matrix3x3 matrix);

Matrix3x3 MakeRotateMatrix(float theta);
*/
//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

//行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

//行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

//転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

//単位行列の作成
Matrix4x4 MakeIdentity4x4();

//平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//回転とスケールのみの座標変換
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);

//void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

//X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

//Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

//Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

//アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
/*
//グリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//球の描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//線分の描画
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//AABBの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//OBBの描画
void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//球と球の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

//球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

//直線と平面の衝突判定
bool IsCollision(const Line& line, const Plane& plane);

//半直線と平面の衝突判定
bool IsCollision(const Ray& ray, const Plane& plane);

//線分と平面の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane);

//三角形と直線の衝突判定
bool IsCollision(const Triangle& triangle, const Line& line);

//三角形と半直線の衝突判定
bool IsCollision(const Triangle& triangle, const Ray& ray);

//三角形と線分の衝突判定
bool IsCollision(const Triangle& triangle, const Segment& segment);

//AABB同士の衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

//AABBと球の衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);

//AABBと直線の衝突判定
bool IsCollision(const AABB& aabb, const Line& line);

//AABBと半直線の衝突判定
bool IsCollision(const AABB& aabb, const Ray& ray);

//AABBと線分の衝突判定
bool IsCollision(const AABB& aabb, const Segment& segment);

//OBBと球の衝突判定
bool IsCollision(const OBB& obb, const Sphere& sphere);

//OBBと直線の衝突判定
bool IsCollision(const OBB& obb, const Line& line);

//OBBと半直線の衝突判定
bool IsCollision(const OBB& obb, const Ray& ray);

//OBBと線分の衝突判定
bool IsCollision(const OBB& obb, const Segment& segemnt);
*/
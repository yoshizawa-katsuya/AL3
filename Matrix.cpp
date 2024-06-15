#include "Matrix.h"
#include "Vector.h"
#include <algorithm>
/*
Matrix3x3 MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 matrix;

	matrix.m[0][0] = 1;
	matrix.m[0][1] = 0;
	matrix.m[0][2] = 0;
	matrix.m[1][0] = 0;
	matrix.m[1][1] = 1;
	matrix.m[1][2] = 0;
	matrix.m[2][0] = translate.x;
	matrix.m[2][1] = translate.y;
	matrix.m[2][2] = 1;

	return matrix;
}

Matrix3x3 Invarse(Matrix3x3 matrix) {
	Matrix3x3 invarse;
	float A_1 = 1 / (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

	invarse.m[0][0] = A_1 * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
	invarse.m[0][1] = A_1 * -(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]);
	invarse.m[0][2] = A_1 * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);
	invarse.m[1][0] = A_1 * -(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]);
	invarse.m[1][1] = A_1 * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
	invarse.m[1][2] = A_1 * -(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]);
	invarse.m[2][0] = A_1 * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
	invarse.m[2][1] = A_1 * -(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]);
	invarse.m[2][2] = A_1 * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);

	return invarse;
}

Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom) {
	Matrix3x3 matrix;

	matrix.m[0][0] = 2 / (right - left);
	matrix.m[0][1] = 0;
	matrix.m[0][2] = 0;
	matrix.m[1][0] = 0;
	matrix.m[1][1] = 2 / (top - bottom);
	matrix.m[1][2] = 0;
	matrix.m[2][0] = (left + right) / (left - right);
	matrix.m[2][1] = (top + bottom) / (bottom - top);
	matrix.m[2][2] = 1;

	return matrix;
}

Matrix3x3 MakeViewportMatrix(float left, float top, float wigth, float height) {
	Matrix3x3 matrix;

	matrix.m[0][0] = wigth / 2;
	matrix.m[0][1] = 0;
	matrix.m[0][2] = 0;
	matrix.m[1][0] = 0;
	matrix.m[1][1] = -(height / 2);
	matrix.m[1][2] = 0;
	matrix.m[2][0] = left + (wigth / 2);
	matrix.m[2][1] = top + (height / 2);
	matrix.m[2][2] = 1;

	return matrix;


}

Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {
	Matrix3x3 matrix;

	matrix.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0];
	matrix.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1];
	matrix.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2];
	matrix.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0];
	matrix.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1];
	matrix.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2];
	matrix.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0];
	matrix.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1];
	matrix.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2];

	return matrix;
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;

	return result;
}

Matrix3x3 MakeRotateMatrix(float theta) {
	Matrix3x3 Anser;

	Anser.m[0][0] = cosf(theta);
	Anser.m[0][1] = sinf(theta);
	Anser.m[0][2] = 0;
	Anser.m[1][0] = -sinf(theta);
	Anser.m[1][1] = cosf(theta);
	Anser.m[1][2] = 0;
	Anser.m[2][0] = 0;
	Anser.m[2][1] = 0;
	Anser.m[2][2] = 1;

	return Anser;
}
*/
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 matrix;

	matrix.m[0][0] = m1.m[0][0] + m2.m[0][0];
	matrix.m[0][1] = m1.m[0][1] + m2.m[0][1];
	matrix.m[0][2] = m1.m[0][2] + m2.m[0][2];
	matrix.m[0][3] = m1.m[0][3] + m2.m[0][3];

	matrix.m[1][0] = m1.m[1][0] + m2.m[1][0];
	matrix.m[1][1] = m1.m[1][1] + m2.m[1][1];
	matrix.m[1][2] = m1.m[1][2] + m2.m[1][2];
	matrix.m[1][3] = m1.m[1][3] + m2.m[1][3];

	matrix.m[2][0] = m1.m[2][0] + m2.m[2][0];
	matrix.m[2][1] = m1.m[2][1] + m2.m[2][1];
	matrix.m[2][2] = m1.m[2][2] + m2.m[2][2];
	matrix.m[2][3] = m1.m[2][3] + m2.m[2][3];

	matrix.m[3][0] = m1.m[3][0] + m2.m[3][0];
	matrix.m[3][1] = m1.m[3][1] + m2.m[3][1];
	matrix.m[3][2] = m1.m[3][2] + m2.m[3][2];
	matrix.m[3][3] = m1.m[3][3] + m2.m[3][3];

	return matrix;

}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 matrix;

	matrix.m[0][0] = m1.m[0][0] - m2.m[0][0];
	matrix.m[0][1] = m1.m[0][1] - m2.m[0][1];
	matrix.m[0][2] = m1.m[0][2] - m2.m[0][2];
	matrix.m[0][3] = m1.m[0][3] - m2.m[0][3];

	matrix.m[1][0] = m1.m[1][0] - m2.m[1][0];
	matrix.m[1][1] = m1.m[1][1] - m2.m[1][1];
	matrix.m[1][2] = m1.m[1][2] - m2.m[1][2];
	matrix.m[1][3] = m1.m[1][3] - m2.m[1][3];

	matrix.m[2][0] = m1.m[2][0] - m2.m[2][0];
	matrix.m[2][1] = m1.m[2][1] - m2.m[2][1];
	matrix.m[2][2] = m1.m[2][2] - m2.m[2][2];
	matrix.m[2][3] = m1.m[2][3] - m2.m[2][3];

	matrix.m[3][0] = m1.m[3][0] - m2.m[3][0];
	matrix.m[3][1] = m1.m[3][1] - m2.m[3][1];
	matrix.m[3][2] = m1.m[3][2] - m2.m[3][2];
	matrix.m[3][3] = m1.m[3][3] - m2.m[3][3];

	return matrix;

}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 matrix;

	matrix.m[0][0] = (m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]);
	matrix.m[0][1] = (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]);
	matrix.m[0][2] = (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]);
	matrix.m[0][3] = (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]);

	matrix.m[1][0] = (m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]);
	matrix.m[1][1] = (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]);
	matrix.m[1][2] = (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]);
	matrix.m[1][3] = (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]);

	matrix.m[2][0] = (m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]);
	matrix.m[2][1] = (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]);
	matrix.m[2][2] = (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]);
	matrix.m[2][3] = (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]);

	matrix.m[3][0] = (m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]);
	matrix.m[3][1] = (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]);
	matrix.m[3][2] = (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]);
	matrix.m[3][3] = (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3]);

	return matrix;

}

Matrix4x4 Inverse(const Matrix4x4& m) {

	Matrix4x4 inverse;
	float A_1 = 1 / ((m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])
		- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])
		+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])
		- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]));


	inverse.m[0][0] = A_1 * ((m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[1][3] * m.m[2][1] * m.m[3][2])
						   - (m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2]));
	inverse.m[0][1] = A_1 * (-(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[2][1] * m.m[3][2])
						    + (m.m[0][3] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2]));
	inverse.m[0][2] = A_1 * ((m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[3][2])
						   - (m.m[0][3] * m.m[1][2] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2]));
	inverse.m[0][3] = A_1 * (-(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) - (m.m[0][3] * m.m[1][1] * m.m[2][2])
						    + (m.m[0][3] * m.m[1][2] * m.m[2][1]) + (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2]));

	inverse.m[1][0] = A_1 * (-(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[1][3] * m.m[2][0] * m.m[3][2])
							+ (m.m[1][3] * m.m[2][2] * m.m[3][0]) + (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2]));
	inverse.m[1][1] = A_1 * ((m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) + (m.m[0][3] * m.m[2][0] * m.m[3][2])
						   - (m.m[0][3] * m.m[2][2] * m.m[3][0]) - (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2]));
	inverse.m[1][2] = A_1 * (-(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) - (m.m[0][3] * m.m[1][0] * m.m[3][2])
							+ (m.m[0][3] * m.m[1][2] * m.m[3][0]) + (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2]));
	inverse.m[1][3] = A_1 * ((m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) + (m.m[0][3] * m.m[1][0] * m.m[2][2])
						   - (m.m[0][3] * m.m[1][2] * m.m[2][0]) - (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2]));

	inverse.m[2][0] = A_1 * ((m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[1][3] * m.m[2][0] * m.m[3][1])
						   - (m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1]));
	inverse.m[2][1] = A_1 * (-(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) - (m.m[0][3] * m.m[2][0] * m.m[3][1])
							+ (m.m[0][3] * m.m[2][1] * m.m[3][0]) + (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1]));
	inverse.m[2][2] = A_1 * ((m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) + (m.m[0][3] * m.m[1][0] * m.m[3][1])
						   - (m.m[0][3] * m.m[1][1] * m.m[3][0]) - (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1]));
	inverse.m[2][3] = A_1 * (-(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) - (m.m[0][3] * m.m[1][0] * m.m[2][1])
							+ (m.m[0][3] * m.m[1][1] * m.m[2][0]) + (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1]));

	inverse.m[3][0] = A_1 * (-(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) - (m.m[1][2] * m.m[2][0] * m.m[3][1])
							+ (m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1]));
	inverse.m[3][1] = A_1 * ((m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) + (m.m[0][2] * m.m[2][0] * m.m[3][1])
						   - (m.m[0][2] * m.m[2][1] * m.m[3][0]) - (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1]));
	inverse.m[3][2] = A_1 * (-(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) - (m.m[0][2] * m.m[1][0] * m.m[3][1])
							+ (m.m[0][2] * m.m[1][1] * m.m[3][0]) + (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1]));
	inverse.m[3][3] = A_1 * ((m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) + (m.m[0][2] * m.m[1][0] * m.m[2][1])
						   - (m.m[0][2] * m.m[1][1] * m.m[2][0]) - (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1]));

	return inverse;

}

Matrix4x4 Transpose(const Matrix4x4& m) {

	Matrix4x4 matrix;

	matrix.m[0][0] = m.m[0][0];
	matrix.m[0][1] = m.m[1][0];
	matrix.m[0][2] = m.m[2][0];
	matrix.m[0][3] = m.m[3][0];

	matrix.m[1][0] = m.m[0][1];
	matrix.m[1][1] = m.m[1][1];
	matrix.m[1][2] = m.m[2][1];
	matrix.m[1][3] = m.m[3][1];

	matrix.m[2][0] = m.m[0][2];
	matrix.m[2][1] = m.m[1][2];
	matrix.m[2][2] = m.m[2][2];
	matrix.m[2][3] = m.m[3][2];

	matrix.m[3][0] = m.m[0][3];
	matrix.m[3][1] = m.m[1][3];
	matrix.m[3][2] = m.m[2][3];
	matrix.m[3][3] = m.m[3][3];

	return matrix;
}

Matrix4x4 MakeIdentity4x4() {

	Matrix4x4 matrix = { 0 };

	matrix.m[0][0] = 1;
	matrix.m[1][1] = 1;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;

	return matrix;

}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 m = { 0 };

	m.m[0][0] = 1;
	m.m[1][1] = 1;
	m.m[2][2] = 1;
	m.m[3][3] = 1;

	m.m[3][0] = translate.x;
	m.m[3][1] = translate.y;
	m.m[3][2] = translate.z;

	return m;

}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 m = { 0 };

	m.m[0][0] = scale.x;
	m.m[1][1] = scale.y;
	m.m[2][2] = scale.z;
	m.m[3][3] = 1;

	return m;

}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;

}

Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2];


	return result;

}
/*
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}
*/
Matrix4x4 MakeRotateXMatrix(float radian) {

	Matrix4x4 m = { 0 };

	m.m[0][0] = 1;
	m.m[3][3] = 1;

	m.m[1][1] = std::cos(radian);
	m.m[1][2] = std::sin(radian);
	m.m[2][1] = -std::sin(radian);
	m.m[2][2] = std::cos(radian);

	return m;

}

Matrix4x4 MakeRotateYMatrix(float radian) {

	Matrix4x4 m = { 0 };

	m.m[1][1] = 1;
	m.m[3][3] = 1;

	m.m[0][0] = std::cos(radian);
	m.m[0][2] = -std::sin(radian);
	m.m[2][0] = std::sin(radian);
	m.m[2][2] = std::cos(radian);

	return m;

}

Matrix4x4 MakeRotateZMatrix(float radian) {

	Matrix4x4 m = { 0 };

	m.m[2][2] = 1;
	m.m[3][3] = 1;

	m.m[0][0] = std::cos(radian);
	m.m[0][1] = std::sin(radian);
	m.m[1][0] = -std::sin(radian);
	m.m[1][1] = std::cos(radian);

	return m;

}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 m;

	Matrix4x4 scalematrix = MakeScaleMatrix(scale);

	Matrix4x4 rotatematrix = Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));

	Matrix4x4 translatematrix = MakeTranslateMatrix(translate);

	m = Multiply(Multiply(scalematrix, rotatematrix), translatematrix);

	return m;

}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 m = { 0 };

	m.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2));
	m.m[1][1] = 1 / std::tan(fovY / 2);
	m.m[2][2] = farClip / (farClip - nearClip);
	m.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	m.m[2][3] = 1;

	return m;

}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	Matrix4x4 m = { 0 };

	m.m[0][0] = 2 / (right - left);
	m.m[1][1] = 2 / (top - bottom);
	m.m[2][2] = 1 / (farClip - nearClip);
	m.m[3][3] = 1;

	m.m[3][0] = (left + right) / (left - right);
	m.m[3][1] = (top + bottom) / (bottom - top);
	m.m[3][2] = nearClip / (nearClip - farClip);

	return m;

}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {

	Matrix4x4 m = { 0 };

	m.m[0][0] = width / 2;
	m.m[1][1] = -(height / 2);
	m.m[2][2] = maxDepth - minDepth;
	m.m[3][3] = 1;

	m.m[3][0] = left + (width / 2);
	m.m[3][1] = top + (height / 2);
	m.m[3][2] = minDepth;

	return m;

}
/*
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		Vector3 starting{ -kGridHalfWidth + (kGridEvery * xIndex), 0.0f, -kGridHalfWidth };
		starting = Transform(starting, viewProjectionMatrix);
		starting = Transform(starting, viewportMatrix);


		Vector3 end{ -kGridHalfWidth + (kGridEvery * xIndex), 0.0f, kGridHalfWidth };
		end = Transform(end, viewProjectionMatrix);
		end = Transform(end, viewportMatrix);

		if (kSubdivision / 2 != xIndex) {
			Novice::DrawLine(int(starting.x), int(starting.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
		else {
			Novice::DrawLine(int(starting.x), int(starting.y), int(end.x), int(end.y), BLACK);
		}
	}
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		Vector3 starting{ -kGridHalfWidth, 0.0f, -kGridHalfWidth + (kGridEvery * xIndex) };
		starting = Transform(starting, viewProjectionMatrix);
		starting = Transform(starting, viewportMatrix);


		Vector3 end{ kGridHalfWidth, 0.0f, -kGridHalfWidth + (kGridEvery * xIndex) };
		end = Transform(end, viewProjectionMatrix);
		end = Transform(end, viewportMatrix);

		if (kSubdivision / 2 != xIndex) {
			Novice::DrawLine(int(starting.x), int(starting.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
		else {
			Novice::DrawLine(int(starting.x), int(starting.y), int(end.x), int(end.y), BLACK);
		}
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	const uint32_t kSubdivision = 10;
	const float kLonEvery = float(2.0f * M_PI) / kSubdivision;
	const float kLatEvery = float(M_PI) / kSubdivision;
	
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(-M_PI) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			Vector3 a, b, c;

			a = { sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon), sphere.center.y + sphere.radius * std::sin(lat), sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon) };
			b = { sphere.center.x + sphere.radius * std::cos(lat + kLatEvery) * std::cos(lon), sphere.center.y + sphere.radius * std::sin(lat + kLatEvery), sphere.center.z + sphere.radius * std::cos(lat + kLatEvery) * std::sin(lon) };
			c = { sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon + kLonEvery), sphere.center.y + sphere.radius * std::sin(lat), sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon + kLonEvery) };

			a = Transform(a, viewProjectionMatrix);
			a = Transform(a, viewportMatrix);

			b = Transform(b, viewProjectionMatrix);
			b = Transform(b, viewportMatrix);

			c = Transform(c, viewProjectionMatrix);
			c = Transform(c, viewportMatrix);

			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(a.x), int(a.y), int(c.x), int(c.y), color);

		}
	}

}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);

}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);

}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Triangle drawTriangle;
	drawTriangle.vertices[0] = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	drawTriangle.vertices[1] = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	drawTriangle.vertices[2] = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle(static_cast<int>(drawTriangle.vertices[0].x), static_cast<int>(drawTriangle.vertices[0].y), static_cast<int>(drawTriangle.vertices[1].x), static_cast<int>(drawTriangle.vertices[1].y), static_cast<int>(drawTriangle.vertices[2].x), static_cast<int>(drawTriangle.vertices[2].y), color, kFillModeWireFrame);

}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 draw[8];
	draw[0] = { aabb.min.x, aabb.min.y, aabb.min.z };
	draw[1] = { aabb.max.x, aabb.min.y, aabb.min.z };
	draw[2] = { aabb.max.x, aabb.max.y, aabb.min.z };
	draw[3] = { aabb.min.x, aabb.max.y, aabb.min.z };
	draw[4] = { aabb.min.x, aabb.min.y, aabb.max.z };
	draw[5] = { aabb.max.x, aabb.min.y, aabb.max.z };
	draw[6] = { aabb.max.x, aabb.max.y, aabb.max.z };
	draw[7] = { aabb.min.x, aabb.max.y, aabb.max.z };

	for (int i = 0; i < 8; i++) {
		draw[i] = Transform(Transform(draw[i], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 4; i++) {

		if (i == 3) {
			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[0].x), static_cast<int>(draw[0].y), color);
		
			Novice::DrawLine(static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), static_cast<int>(draw[4].x), static_cast<int>(draw[4].y), color);

			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), color);

		}
		else {
			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[i + 1].x), static_cast<int>(draw[i + 1].y), color);
		
			Novice::DrawLine(static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), static_cast<int>(draw[i + 5].x), static_cast<int>(draw[i + 5].y), color);

			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), color);

		}

	}

}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{

	Vector3 draw[8];
	draw[0] = Add(Add(Add(obb.center, Multiply(-obb.size.x, obb.orientations[0])), Multiply(-obb.size.y, obb.orientations[1])), Multiply(-obb.size.z, obb.orientations[2]));
	draw[1] = Add(Add(Add(obb.center, Multiply(obb.size.x, obb.orientations[0])), Multiply(-obb.size.y, obb.orientations[1])), Multiply(-obb.size.z, obb.orientations[2]));
	draw[2] = Add(Add(Add(obb.center, Multiply(obb.size.x, obb.orientations[0])), Multiply(obb.size.y, obb.orientations[1])), Multiply(-obb.size.z, obb.orientations[2]));
	draw[3] = Add(Add(Add(obb.center, Multiply(-obb.size.x, obb.orientations[0])), Multiply(obb.size.y, obb.orientations[1])), Multiply(-obb.size.z, obb.orientations[2]));
	draw[4] = Add(Add(Add(obb.center, Multiply(-obb.size.x, obb.orientations[0])), Multiply(-obb.size.y, obb.orientations[1])), Multiply(obb.size.z, obb.orientations[2]));
	draw[5] = Add(Add(Add(obb.center, Multiply(obb.size.x, obb.orientations[0])), Multiply(-obb.size.y, obb.orientations[1])), Multiply(obb.size.z, obb.orientations[2]));
	draw[6] = Add(Add(Add(obb.center, Multiply(obb.size.x, obb.orientations[0])), Multiply(obb.size.y, obb.orientations[1])), Multiply(obb.size.z, obb.orientations[2]));
	draw[7] = Add(Add(Add(obb.center, Multiply(-obb.size.x, obb.orientations[0])), Multiply(obb.size.y, obb.orientations[1])), Multiply(obb.size.z, obb.orientations[2]));

	for (int i = 0; i < 8; i++) {
		draw[i] = Transform(Transform(draw[i], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 4; i++) {

		if (i == 3) {
			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[0].x), static_cast<int>(draw[0].y), color);

			Novice::DrawLine(static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), static_cast<int>(draw[4].x), static_cast<int>(draw[4].y), color);

			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), color);

		}
		else {
			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[i + 1].x), static_cast<int>(draw[i + 1].y), color);

			Novice::DrawLine(static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), static_cast<int>(draw[i + 5].x), static_cast<int>(draw[i + 5].y), color);

			Novice::DrawLine(static_cast<int>(draw[i].x), static_cast<int>(draw[i].y), static_cast<int>(draw[i + 4].x), static_cast<int>(draw[i + 4].y), color);

		}

	}


}

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	//2つの球の中心点間の距離を求める
	float distance = Length(Subtract(s2.center, s1.center));
	//半径の合計よりも短ければ衝突
	if (!(distance <= s1.radius + s2.radius)) {
		return false;
	}
	return true;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

	float k = Dot(plane.normal, sphere.center) - plane.distance;
	if (k < 0) {
		k *= -1;
	}

	if (!(k <= sphere.radius)) {
		return false;
	}
	return true;
}

bool IsCollision(const Line& line, const Plane& plane) {

	float dot = Dot(line.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}
	return true;

}

bool IsCollision(const Ray& ray, const Plane& plane) {

	float dot = Dot(ray.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	if (0 <= t) {
		return true;
	}
	return false;


}

bool IsCollision(const Segment& segment, const Plane& plane) {

	float dot = Dot(segment.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (0 <= t && t <= 1) {
		return true;
	}
	return false;

}

bool IsCollision(const Triangle& triangle, const Line& line) {

	Plane plane;
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	float dot = Dot(line.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	

	Vector3 p = Add(line.origin, Multiply(t, line.diff));
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	

	return false;


}

bool IsCollision(const Triangle& triangle, const Ray& ray) {

	Plane plane;
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	float dot = Dot(ray.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	if (t >= 0) {

		Vector3 p = Add(ray.origin, Multiply(t, ray.diff));
		Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
		Vector3 v1p = Subtract(p, triangle.vertices[1]);
		Vector3 v2p = Subtract(p, triangle.vertices[2]);
		Vector3 v0p = Subtract(p, triangle.vertices[0]);

		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);

		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {
			return true;
		}

	}

	return false;


}

bool IsCollision(const Triangle& triangle, const Segment& segment) {

	Plane plane;
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	float dot = Dot(segment.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (0 <= t && t <= 1) {
		
		Vector3 p = Add(segment.origin, Multiply(t, segment.diff));
		Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
		Vector3 v1p = Subtract(p, triangle.vertices[1]);
		Vector3 v2p = Subtract(p, triangle.vertices[2]);
		Vector3 v0p = Subtract(p, triangle.vertices[0]);

		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);

		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {
			return true;
		}

	}

	return false;

}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}
	return false;

}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {

	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
						  std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
						  std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)};

	float distance = Length(Subtract(sphere.center, closestPoint));

	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}

bool IsCollision(const AABB& aabb, const Line& line) {


	float txMin = (aabb.min.x - line.origin.x) / line.diff.x;
	float txMax = (aabb.max.x - line.origin.x) / line.diff.x;
	float tyMin = (aabb.min.y - line.origin.y) / line.diff.y;
	float tyMax = (aabb.max.y - line.origin.y) / line.diff.y;
	float tzMin = (aabb.min.z - line.origin.z) / line.diff.z;
	float tzMax = (aabb.max.z - line.origin.z) / line.diff.z;

	float tNearX = (std::min)(txMin, txMax);
	float tFarX = (std::max)(txMin, txMax);
	float tNearY = (std::min)(tyMin, tyMax);
	float tFarY = (std::max)(tyMin, tyMax);
	float tNearZ = (std::min)(tzMin, tzMax);
	float tFarZ = (std::max)(tzMin, tzMax);

	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		return true;
	}

	return false;


}

bool IsCollision(const AABB& aabb, const Ray& ray) {

	float txMin = (aabb.min.x - ray.origin.x) / ray.diff.x;
	float txMax = (aabb.max.x - ray.origin.x) / ray.diff.x;
	float tyMin = (aabb.min.y - ray.origin.y) / ray.diff.y;
	float tyMax = (aabb.max.y - ray.origin.y) / ray.diff.y;
	float tzMin = (aabb.min.z - ray.origin.z) / ray.diff.z;
	float tzMax = (aabb.max.z - ray.origin.z) / ray.diff.z;

	float tNearX = (std::min)(txMin, txMax);
	float tFarX = (std::max)(txMin, txMax);
	float tNearY = (std::min)(tyMin, tyMax);
	float tFarY = (std::max)(tyMin, tyMax);
	float tNearZ = (std::min)(tzMin, tzMax);
	float tFarZ = (std::max)(tzMin, tzMax);

	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		if (tmax >= 0.0f) {
			return true;
		}
	}

	return false;

}

bool IsCollision(const AABB& aabb, const Segment& segment) {

	float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float txMax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tyMax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tzMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = (std::min)(txMin, txMax);
	float tFarX = (std::max)(txMin, txMax);
	float tNearY = (std::min)(tyMin, tyMax);
	float tFarY = (std::max)(tyMin, tyMax);
	float tNearZ = (std::min)(tzMin, tzMax);
	float tFarZ = (std::max)(tzMin, tzMax);

	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		if ((tmin >= 0.0f && tmin <= 1.0f) || (tmax >= 0.0f && tmax <= 1.0f)) {
			return true;
		}
		else if (tmin < 0.0f && tmax > 1.0f) {
			return true;
		}
	}

	return false;

}

bool IsCollision(const OBB& obb, const Sphere& sphere) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 centerInOBBLocalSpace = Transform(sphere.center, obbWorldMatrixInverse);

	AABB aabbOBBLocal{ Multiply(-1.0f, obb.size), obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };
	//ローカル空間で衝突判定
	return IsCollision(aabbOBBLocal, sphereOBBLocal);

}

bool IsCollision(const OBB& obb, const Line& line) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 localOrigin = Transform(line.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(line.origin, line.diff), obbWorldMatrixInverse);

	AABB localAABB{ Multiply(-1.0f, obb.size), obb.size };

	Line localLine;
	localLine.origin = localOrigin;
	localLine.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localLine);

}

bool IsCollision(const OBB& obb, const Ray& ray) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 localOrigin = Transform(ray.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(ray.origin, ray.diff), obbWorldMatrixInverse);

	AABB localAABB{ Multiply(-1.0f, obb.size), obb.size };

	Ray localRay;
	localRay.origin = localOrigin;
	localRay.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localRay);

}

bool IsCollision(const OBB& obb, const Segment& segemnt) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 localOrigin = Transform(segemnt.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(segemnt.origin, segemnt.diff), obbWorldMatrixInverse);

	AABB localAABB{ Multiply(-1.0f, obb.size), obb.size };

	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localSegment);

}
*/
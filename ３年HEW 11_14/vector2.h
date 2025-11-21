#pragma once

#include <math.h>

// Vector2クラスーーー簡単な2Dベクトルクラス

class Vector2
{
public:
	float x;
	float y;

	Vector2() = default;
	Vector2(const Vector2& a) : x(a.x), y(a.y) {};
	Vector2(float nx, float ny) : x(nx), y(ny) {}

	// 標準的なオブジェクトの保守

	// 代入（Cの慣習に従い値への参照を返す）
	Vector2& operator = (const Vector2& a)
	{
		x = a.x;
		y = a.y;

		return *this;
	}

	//等しさのチェック

	bool operator == (const Vector2& a) const {
		return x == a.x && y == a.y;
	}

	bool operator != (const Vector2& a) const {
		return x != a.x || y != a.y;
	}

	//ベクトル操作
	//ベクトルを0に設定する
	void zero() {
		x = y = 0.0f;
	}

	// 単項式のマイナスは、反転したベクトルを返す
	Vector2 operator -() const { return Vector2(-x, -y); }

	// 二項式の+と-はベクトルを加算し、減算する
	Vector2 operator +(const Vector2& a) const {
		return Vector2(x + a.x, y + a.y);
	}

	Vector2 operator -(const Vector2& a) const {
		return Vector2(x - a.x, y - a.y);
	}

	// スカラーによる乗算と除算
	Vector2 operator *(float a) const {
		return Vector2(x * a, y * a);
	}

	Vector2 operator /(float a) const {
		float oneOverA = 1.0f / a; // 注意:ここではゼロ除算のチェックはしていない
		return Vector2(x * oneOverA, y * oneOverA);
	}

	// Cの表記法に準拠するための
	// 組み合わせ代入演算
	Vector2& operator +=(const Vector2& a) {
		x += a.x; y += a.y;
		return *this;
	}

	Vector2& operator -= (const Vector2& a) {
		x -= a.x; y -= a.y;
		return *this;
	}

	Vector2& operator *= (float a) {
		x *= a; y *= a;
		return *this;
	}

	Vector2& operator /=(float a) {
		float oneOverA = 1.0f / a;
		x *= oneOverA; y *= oneOverA;
		return *this;
	}

	// ベクトルを正規化する
	void  normalize() {
		float magSq = x * x + y * y ;
		if (magSq > 0.0f) {//0除算をチェックする
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
		}
	}

	float length() const
	{
		return sqrtf(x * x + y * y);
	}

	// ベクトルの内積
	// 標準の乗算記号をこれにオーバーロードする

	float operator *(const Vector2& a) const {
		return x * a.x + y * a.y;
	}

	//スカラー値
	static float cross(const Vector2& a, const Vector2& b) {
		return a.x * b.y - a.y * b.x;
	}

	// 内積
	static float dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	// 距離
	static float distance(const Vector2& a, const Vector2& b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return sqrtf(dx * dx + dy * dy);
	}
};
// グローバル変数
// グローバルなゼロベクトル定数を提供する
extern const Vector2 kZeroVector2;
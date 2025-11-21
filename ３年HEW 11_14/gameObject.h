//ver1.1　変更点：3Dオブジェクトと２Dオブジェクトの分離
//ver1.2　変更点：ゲームオブジェクトのCPUとGPUで行うことの分離
#pragma once
#include "texture.h"

#include "vector3.h"
#include "vector2.h"

#include <DirectXMath.h>

using namespace DirectX;

class GameObject
{
protected:
	//特定のaddObjectの情報を取るよう
	std::wstring m_ObjectName = L"NullName";

	bool m_Destroy = false;
public:
	GameObject* SetName(const std::wstring& name) 
	{ 
		m_ObjectName = name;
		return this;
	}

	const std::wstring& GetName() const { return m_ObjectName; }

	// オブジェクトを消す予約をする
	void SetDestroy() { m_Destroy = true; };

	bool Destroy()
	{
		// オブジェクトを消す
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}

		// オブジェクトを消さない
		return false;
	}
	//動的生成をするものはこっち
	virtual void Init() {};
	//注意：動的に配置するものには使わない
	virtual void InitCPU() {};
	virtual void InitGPU() {};

	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};

class GameObject3D : public GameObject
{
protected:
	Vector3 m_Position = { 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale = { 1.0f, 1.0f, 1.0f };
public:
	void SetPosition(Vector3 position) { m_Position = position; }
	void SetRotation(Vector3 rotation) { m_Rotation = rotation; }
	void SetScale(Vector3 scale) { m_Scale = scale; }

	Vector3 GetPosition() { return m_Position; }
	Vector3 GetRotation() { return m_Rotation; }
	Vector3 GetScale() { return m_Scale; }

	Vector3 GetRight()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 right;
		XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);

		return right;
	}

	Vector3 GetForward()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);

		return forward;
	}

	float GetDistance(Vector3 position)
	{
		// オペレーターオーバーロード
		// -のオーバーロードでVector3が返される
		// それによってVector3のlength()が使えるようになる
		return (m_Position - position).length();
	}

	virtual float GetZ(Vector3 Position, Vector3 Forward)
	{
		Vector3 direction = m_Position - Position;
		return Vector3::dot(direction, Forward);
	}
};

class GameObject2D : public GameObject
{
protected:
	Vector2 m_Position = { 0.0f, 0.0f };
	Vector2 m_Rotation = { 0.0f, 0.0f };
	Vector2 m_Scale = { 100.0f, 100.0f };

	ID3D11ShaderResourceView* m_Texture = nullptr;
public:
	void SetPosition(Vector2 position) { m_Position = position; }
	void SetRotation(Vector2 rotation) { m_Rotation = rotation; }
	void SetScale(Vector2 scale) { m_Scale = scale; }

	Vector2 GetPosition() { return m_Position; }
	Vector2 GetRotation() { return m_Rotation; }
	Vector2 GetScale() { return m_Scale; }

};
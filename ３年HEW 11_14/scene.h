//ver1.1 変更点：initCPU,GPUを呼び出し
#pragma once

#include <list>
#include <vector>
#include "gameObject.h"

#define LAYER_MAX (10)

class Scene
{
private:
	std::list<GameObject3D*> m_GameObject3D[LAYER_MAX];
	std::list<GameObject2D*> m_GameObject2D[LAYER_MAX];
public:
	virtual void Init();
	virtual void InitCPU() {};
	virtual void InitGPU() {};

	virtual void Uninit();

	virtual void Update();
	virtual void Draw();

	// テンプレート （型を生成して送れる！）
	template<typename T>
	T* AddGameObject3D(int layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject3D[layer].push_back(gameObject);

		return gameObject;
	}

	template<typename T>
	T* AddGameObject2D(int layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject2D[layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto gameObject : m_GameObject3D[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					return find;
				}
			}
			for (auto gameObject : m_GameObject2D[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					return find;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	T* GetGameObject(const std::wstring& name)
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto gameObject : m_GameObject3D[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					if (find->GetName() == name)
						return find;
				}
			}
			for (auto gameObject : m_GameObject2D[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					if (find->GetName() == name)
						return find;
				}
			}
		}
		return nullptr;
	}
	// バグった場合デバッグがめんどくさいのであまり多用しない！
	// この中はなるべく少なく書く！

	template <typename T>
	std::vector<T*> GetGameObjects() // 複数返せるようになる
	{
		std::vector<T*> finds;
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto gameObject : m_GameObject3D[i])
			{
				// dynamic_castは型変換できるかをチェックする
				T* find = dynamic_cast<T*>(gameObject); // gameObjectをTに型変換　型変換できない場合はfindにnullが入る
				if (find != nullptr)
				{
					finds.push_back(find);
				}

			}
			for (auto gameObject : m_GameObject2D[i])
			{
				// dynamic_castは型変換できるかをチェックする
				T* find = dynamic_cast<T*>(gameObject); // gameObjectをTに型変換　型変換できない場合はfindにnullが入る
				if (find != nullptr)
				{
					finds.push_back(find);
				}

			}
		}

		return finds;
	}
};
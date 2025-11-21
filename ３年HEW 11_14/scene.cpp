#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"

void Scene::Init()
{
	
}

void Scene::Uninit()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (auto gameObject : m_GameObject3D[i])
		{
			gameObject->Uninit();
			delete gameObject;
		}
		for (auto gameObject : m_GameObject2D[i])
		{
			gameObject->Uninit();
			delete gameObject;
		}
		m_GameObject3D[i].clear();
		m_GameObject2D[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (auto gameObject : m_GameObject3D[i])
		{
			gameObject->Update();
		}
		for (auto gameObject : m_GameObject2D[i])
		{
			gameObject->Update();
		}
	}
	for (int i = 0; i < LAYER_MAX; i++)
	{
		// ラムダ式
		m_GameObject3D[i].remove_if([](GameObject3D* object)
		{
			// リソース開放
			return object->Destroy();
		});	

		m_GameObject2D[i].remove_if([](GameObject2D* object)
		{
			// リソース開放
			return object->Destroy();
		});
	}
}

void Scene::Draw()
{
	// Zソート
	Camera* camera = GetGameObject<Camera>();

	if (camera != nullptr)
	{
		Vector3 cameraPosition = camera->GetPosition();
		Vector3 cameraForward = camera->GetForward();

		m_GameObject3D[1].sort([&](GameObject3D* object1, GameObject3D* object2)
			{
				return object1->GetZ(cameraPosition, cameraForward) > object2->GetZ(cameraPosition, cameraForward);
			});
	}

	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (auto gameObject : m_GameObject3D[i])
		{
			gameObject->Draw();
		}
	}
	
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (auto gameObject : m_GameObject2D[i])
		{
			gameObject->Draw();
		}
	}
}


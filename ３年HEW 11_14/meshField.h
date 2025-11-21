//#pragma once
//
//#include "gameObject.h"
//#include <vector>
//
//#define FIELD_RES (10)
//#define FIELD_CELL_SIZE (1.0f)
//
//extern float g_FieldHeight[FIELD_RES][FIELD_RES];
//
//class MeshField : public GameObject3D
//{
//private:
//	ID3D11Buffer* m_VertexBuffer{};
//	ID3D11Buffer* m_IndexBuffer{};
//	ID3D11ShaderResourceView* m_Texture{};
//
//	ID3D11InputLayout* m_VertexLayout;
//	ID3D11VertexShader* m_VertexShader;
//	ID3D11PixelShader* m_PixelShader;
//	
//	std::vector <VERTEX_3D> m_Vertex[FIELD_RES][FIELD_RES];
//
//	unsigned int m_Index[((FIELD_RES + 1) * 2) * (FIELD_RES - 1) - 2];
//
//public:
//	void InitCPU() override;
//	void InitGPU() override;
//	
//	//void Init() override;
//
//	void Uninit() override;
//	void Update() override;
//	void Draw() override;
//
//	float GetHeight(Vector3 Position);
//};

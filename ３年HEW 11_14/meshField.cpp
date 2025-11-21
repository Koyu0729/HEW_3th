//#include "main.h"
//
//#include "meshField.h"
//#include "renderer.h"
//
//#include "scene.h"
//#include "manager.h"
//
//#include "player.h"
//#include "input.h"
//
////https://rastertek.com/tutterr.html
//
//float g_FieldHeight[FIELD_RES][FIELD_RES] = {};
//
//#pragma region INITの残骸
////void MeshField::Init()
////{
////	// 頂点バッファ生成
////	{
////		for (int x = 0; x <= FIELD_RES-1; x++)
////		{
////			for (int z = 0; z <= FIELD_RES-1; z++)
////			{
////				float y = g_FieldHeight[x][z];
////				m_Vertex[x][z].Position = XMFLOAT3(
////					(x - FIELD_RES / 2) * 2.0f,
////					 y,
////					(z - FIELD_RES / 2) * -2.0f
////				);//ポリゴン増やしたから修正
////				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);//法線ベクトル
////				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
////				m_Vertex[x][z].TexCoord = XMFLOAT2((float)x / (FIELD_RES - 1), (float)z / (FIELD_RES - 1));//ポリゴン増やしたから修正
////			}
////		}
////
////		for (int x = 1; x < FIELD_RES - 1; x++) // 端は計算できないので-1
////		{
////			for (int z = 1; z < FIELD_RES - 1; z++)
////			{
////				Vector3 vx, vz, vn;
////				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
////				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
////				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;
////
////				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
////				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
////				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;
////				
////				vn = Vector3::cross(vz, vx); // 外積　出力が逆になって出てくる
////				vn.normalize(); // 正規化(長さを1にする)　法線は長さが1でないと出力されない
////
////				m_Vertex[x][z].Normal.x = vn.x;
////				m_Vertex[x][z].Normal.y = vn.y;
////				m_Vertex[x][z].Normal.z = vn.z;
////			}
////		}
////
////		D3D11_BUFFER_DESC bd;
////		ZeroMemory(&bd, sizeof(bd));
////		bd.Usage = D3D11_USAGE_DEFAULT;
////		bd.ByteWidth = sizeof(VERTEX_3D) * FIELD_RES * FIELD_RES;
////		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
////		bd.CPUAccessFlags = 0;
////
////		D3D11_SUBRESOURCE_DATA sd;
////		ZeroMemory(&sd, sizeof(sd));
////		sd.pSysMem = m_Vertex;
////
////		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
////	}
////
////
////
////
////	// インデックスバッファ生成
////	{
////		unsigned int index[((FIELD_RES+1) * 2) * (FIELD_RES - 1) - 2 ];
////
////		int i = 0;
////		for (int x = 0; x < FIELD_RES - 1; x++)
////		{
////			for (int z = 0; z < FIELD_RES; z++)
////			{
////				index[i] = x * FIELD_RES + z;
////				i++;
////
////				index[i] = (x + 1) * FIELD_RES + z;
////				i++;
////			}
////
////			if (x == FIELD_RES-2)
////				break;
////
////			// 縮退ポリゴン 余計な頂点データをいれる
////			index[i] = (x + 1) * FIELD_RES + FIELD_RES-1;
////			i++;
////
////			index[i] = (x + 1) * FIELD_RES;
////			i++;
////		}
////
////		D3D11_BUFFER_DESC bd;
////		ZeroMemory(&bd, sizeof(bd));
////		bd.Usage = D3D11_USAGE_DEFAULT;
////		bd.ByteWidth = sizeof(unsigned int) * ((FIELD_RES * 2) * (FIELD_RES - 1) - 2);
////		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
////		bd.CPUAccessFlags = 0;
////
////		D3D11_SUBRESOURCE_DATA sd;
////		ZeroMemory(&sd, sizeof(sd));
////		sd.pSysMem = index;
////
////		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
////	}
////
////	// テクスチャ読み込み
////	TexMetadata metadata;
////	ScratchImage image;
////	LoadFromWICFile(L"asset\\texture\\wall.jpg", WIC_FLAGS_NONE, &metadata, image);
////	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
////	assert(m_Texture);
////
////	// シェーダー読み込み
////	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
////	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
////
////
////	MakeRoad();
////}
//#pragma endregion
//
//void MeshField::InitCPU()
//{
//	{
//		for (int x = 0; x <= FIELD_RES - 1; x++)
//		{
//			for (int z = 0; z <= FIELD_RES - 1; z++)
//			{
//				float y = g_FieldHeight[x][z];
//				m_Vertex[x][z].Position = XMFLOAT3(
//					(x - FIELD_RES / 2) * FIELD_CELL_SIZE,
//					 y,
//					(z - FIELD_RES / 2) * -FIELD_CELL_SIZE
//				);//ポリゴン増やしたから修正
//				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);//法線ベクトル
//				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//				m_Vertex[x][z].TexCoord = XMFLOAT2((float)x / (FIELD_RES - 1), (float)z / (FIELD_RES - 1));//ポリゴン増やしたから修正
//			}
//		}
//
//
//		for (int x = 1; x < FIELD_RES - 1; x++) // 端は計算できないので-1
//		{
//			for (int z = 1; z < FIELD_RES - 1; z++)
//			{
//				Vector3 vx, vz, vn;
//				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
//				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
//				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;
//
//				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
//				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
//				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;
//
//				vn = Vector3::cross(vz, vx); // 外積　出力が逆になって出てくる
//				vn.normalize(); // 正規化(長さを1にする)　法線は長さが1でないと出力されない
//
//				m_Vertex[x][z].Normal.x = vn.x;
//				m_Vertex[x][z].Normal.y = vn.y;
//				m_Vertex[x][z].Normal.z = vn.z;
//			}
//		}
//	}
//
//	// インデックスバッファ生成
//	{
//
//		int i = 0;
//		for (int x = 0; x < FIELD_RES - 1; x++)
//		{
//			for (int z = 0; z < FIELD_RES; z++)
//			{
//				m_Index[i] = x * FIELD_RES + z;
//				i++;
//
//				m_Index[i] = (x + 1) * FIELD_RES + z;
//				i++;
//			}
//
//			if (x == FIELD_RES - 2)
//				break;
//
//			// 縮退ポリゴン 余計な頂点データをいれる
//			m_Index[i] = (x + 1) * FIELD_RES + FIELD_RES - 1;
//			i++;
//
//			m_Index[i] = (x + 1) * FIELD_RES;
//			i++;
//		}
//	}
//}
//
//// GPUだけ
//void MeshField::InitGPU()
//{
//	// 頂点バッファ
//	D3D11_BUFFER_DESC vbd;
//	ZeroMemory(&vbd, sizeof(vbd));
//	vbd.Usage = D3D11_USAGE_DEFAULT;
//	vbd.ByteWidth = sizeof(VERTEX_3D) * FIELD_RES * FIELD_RES;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA vsd;
//	ZeroMemory(&vsd, sizeof(vsd));
//	vsd.pSysMem = m_Vertex;
//
//	Renderer::GetDevice()->CreateBuffer(&vbd, &vsd, &m_VertexBuffer);
//
//
//	D3D11_BUFFER_DESC ibd;
//	ZeroMemory(&ibd, sizeof(ibd));
//	ibd.Usage = D3D11_USAGE_DEFAULT;
//	ibd.ByteWidth = sizeof(unsigned int) * ((FIELD_RES * 2) * (FIELD_RES - 1) - 2);
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA isd;
//	ZeroMemory(&isd, sizeof(isd));
//	isd.pSysMem = m_Index;
//
//	Renderer::GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);
//
//	// テクスチャ読み込み
//	TexMetadata metadata;
//	ScratchImage image;
//	LoadFromWICFile(L"asset\\texture\\wall.jpg", WIC_FLAGS_NONE, &metadata, image);
//	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
//	assert(m_Texture);
//
//	// シェーダー読み込み
//	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
//	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
//}
//
//
//void MeshField::Uninit()
//{
//	m_VertexBuffer->Release();
//	m_IndexBuffer->Release();
//	m_Texture->Release();
//
//	m_VertexLayout->Release();
//	m_VertexShader->Release();
//	m_PixelShader->Release();
//}
//
//
//void MeshField::Update()
//{
//	Player* player = Manager::GetScene()->GetGameObject<Player>();
//
//	int px = (int)(player->GetPosition().x / FIELD_CELL_SIZE + FIELD_RES / 2);
//	int pz = (int)(player->GetPosition().z / -FIELD_CELL_SIZE + FIELD_RES / 2);
//
//	if (Input::GetKeyPress('V'))
//	{
//		system("cls");
//
//		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//		for (int i = 0; i < FIELD_RES; i++)
//		{
//			for (int j = 0; j < FIELD_RES; j++)
//			{
//				if (j == px && i == pz)
//				{
//					SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // 赤
//					printf("P"); // プレイヤー
//					SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//				}
//				
//				else
//				{
//					printf("%d", (int)g_FieldHeight[j][i]);
//				}
//			}
//			printf("\n");
//		}
//	}
//}
//
//void MeshField::Draw()
//{
//	// 入力レイアウト設定
//	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
//
//	// シェーダ設定
//	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
//	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
//
//	// マトリクスの設定
//	XMMATRIX world, scale, rot, trans;
//	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
//	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z); // ラジアン角
//	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
//	world = scale * rot * trans;
//
//	Renderer::SetWorldMatrix(world);
//
//	// 頂点バッファ設定
//	UINT stride = sizeof(VERTEX_3D);
//	UINT offset = 0;
//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
//
//	// インデックスバッファ設定
//	Renderer::GetDeviceContext()->IASetIndexBuffer(
//		m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0); // R32 = 32bit
//
//	// マテリアル設定
//	MATERIAL material;
//	ZeroMemory(&material, sizeof(material));
//	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	material.TextureEnable = true;
//	Renderer::SetMaterial(material);
//
//	// テクスチャ設定
//	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
//
//	// プリミティブトポロジ設定
//	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//
//	D3D11_RASTERIZER_DESC rasterDesc = {};
//	rasterDesc.FillMode = D3D11_FILL_SOLID;
//	rasterDesc.CullMode = D3D11_CULL_NONE;  // ← カリング無効
//	ID3D11RasterizerState* rasterState;
//	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &rasterState);
//	Renderer::GetDeviceContext()->RSSetState(rasterState);
//	// ポリゴン描画
//	Renderer::GetDeviceContext()->DrawIndexed(
//		(FIELD_RES * 2) * (FIELD_RES - 1) - 2, 0, 0);
//}
//
//// 当たり判定
//float MeshField::GetHeight(Vector3 Position)
//{
//	/*int x, z;*/
//
//	// ブロック番号算出
//	// ①プレイヤーの座標からどの位置にいるかを求める
//	//x = Position.x / 5.0f + 10.0f;
//	//z = Position.z / -5.0f + 10.0f;
//
//	//XMFLOAT3 pos0, pos1, pos2, pos3;
//
//	//pos0 = m_Vertex[x][z].Position;
//	//pos1 = m_Vertex[x + 1][z].Position;
//	//pos2 = m_Vertex[x][z + 1].Position;
//	//pos3 = m_Vertex[x + 1][z + 1].Position;
//
//	//flaotからintに直す方法書き換えやってることは一緒
//	float fx = Position.x / FIELD_CELL_SIZE + FIELD_RES * 0.5f;
//	float fz = -Position.z / FIELD_CELL_SIZE + FIELD_RES * 0.5f;
//
//	int x = (int)floorf(fx);
//	int z = (int)floorf(fz);
//
//	if (x < 0 || z < 0 || x >= FIELD_RES - 1 || z >= FIELD_RES - 1) {
//		return 0.0f; // あるいはフィールドの最低高さ
//	}
//	// セル内の小数部分 (0.0 ~ 1.0)
//	float u = fx - x;
//	float v = fz - z;
//
//	XMFLOAT3 pos0 = m_Vertex[x][z].Position;
//	XMFLOAT3 pos1 = m_Vertex[x + 1][z].Position;
//	XMFLOAT3 pos2 = m_Vertex[x][z + 1].Position;
//	XMFLOAT3 pos3 = m_Vertex[x + 1][z + 1].Position;
//	// 頂点を横断する斜めのベクトル、頂点からプレイヤーまでのベクトル
//	Vector3 v12, v1p;
//	v12.x = pos2.x - pos1.x;
//	v12.y = pos2.y - pos1.y;
//	v12.z = pos2.z - pos1.z;
//
//	v1p.x = Position.x - pos1.x;
//	v1p.y = Position.y - pos1.y;
//	v1p.z = Position.z - pos1.z;
//
//	// 外積 ②垂直なベクトルを求める 左手座標系
//	// 例.v12*v1p=cyは-方向、v1p*v12=cyは+方向
//	float cy = v12.z * v1p.x - v12.x * v1p.z;
//
//	float py;
//	Vector3 n;
//
//	// 法線ベクトルを算出
//	if (cy > 0.0f)
//	{
//		// 左上ポリゴン
//		Vector3 v10;
//		v10.x = pos0.x - pos1.x;
//		v10.y = pos0.y - pos1.y;
//		v10.z = pos0.z - pos1.z;
//
//		// 外積
//		n = Vector3::cross(v10, v12);
//	}
//	else
//	{
//		// 右下ポリゴン
//		Vector3 v13;
//		v13.x = pos3.x - pos1.x;
//		v13.y = pos3.y - pos1.y;
//		v13.z = pos3.z - pos1.z;
//
//		// 外積
//		n = Vector3::cross(v12, v13);
//	}
//
//	if (fabs(n.y) < 1e-6f)//1を100万で割った値
//	{
//		return pos1.y;
//	}
//	// ③高さ取得
//	/* n・v1p = 0 内積
//	 ↓分解
//	 v1p=(n・(Position - pos1= 0))
//	 ↓分解
//	 n.x * (Position.x - pos1.x) +
//	 n.y * (Position.y - pos1.y) +
//	 n.z * (Position.z - pos1.z) = 0
//	  ↓求めたいものを計算
//	*/
//	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;
//
//	return py;
//}

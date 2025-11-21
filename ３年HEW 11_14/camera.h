#ifndef CAMERA_H
#define CAMERA_H

#include "gameObject.h"

class Camera : public GameObject3D
{
private:
	XMMATRIX m_Projection;
	XMMATRIX m_View;
	Vector3 m_Target = {0.0f, 0.0f, 0.0f};

	//XMFLOAT3	Position;		//カメラの座標
	//XMFLOAT3	AtPosition;		//カメラの注視点
	//XMFLOAT3	UpVector;		//上方ベクトル
	//float		Fov;			//視野角
	//float		Nearclip;		//どこまで近くが見えるか
	//float		Farclip;		//どこまで遠くが見えるか
	float		Rotation;		//回転
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	XMMATRIX GetViewMatrix() { return m_View; }
};

#endif // FILED_H

#pragma once
#include "scene.h"

#include "nodeCreate.h"
#include "nodeView.h"
#include "drawLine.h"
//ノードマネージャー
#include "nodeManager.h"

class NodeCreateList;

class NodeEditor : public Scene
{
private:
    //ノードマネージャー
    NodeManager* m_NodeManager;

    //生成されているノードビューを格納する配列
    std::vector<NodeView*> m_NodeViews;

    //選択してるノード
    NodeView* m_SelectedNode = nullptr;

    //接続線の描画
    DrawLine* m_DragLine = nullptr;            //ドラッグ中
    std::vector<DrawLine*> m_ConnectedLines;   //接続済みの線

    //接続されているか
    bool m_Connecting = false;

    //接続線の引く起点
    NodeView* m_StartNode = nullptr;      //接続線の引く起点のノード
    Port m_StartPort = Port::PortNull;    //接続線の引く起点のポート

    //生成するときの判定
    bool m_NodeCreateMode = false;

    //最終的にリストになるけど、今は適当な画像を出している
    NodeCreateList* m_CreateList = nullptr;
public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;
private:
    //ノード接続処理
    void NodeConnect();
    //選択中ノード
    void NodeSelect();
    //生成
    void NodeCreateInput();
    //消去
    void NodeDelete();
    //線の更新(接続線の更新はこちら側でやる)
    void UpdateConnections();
};

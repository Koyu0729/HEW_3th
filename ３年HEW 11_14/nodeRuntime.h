#pragma once
#include "nodeManager.h"
#include "player.h"
// ノード実行用クラス（ビジュアル接続結果をもとに実行）
class NodeRuntime
{
private:
    Player* m_Player;
    std::vector<NodeBase*> m_RunNodes;

    float delta = 0.0f;
public:
    // ノード実行の開始
    void Run(std::vector<NodeBase*> runNodes);

private:
    NodeBase* FindStartNode();
};

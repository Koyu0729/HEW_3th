#pragma once
#include "nodeManager.h"
#include "nodeView.h"
#include "nodeIncludes.h" // Attack1,Move1,If1などまとめてインクルード

class NodeCreate
{
public:
    // ノードを生成(内部処理＋見た目）
    static NodeView* Create(NodeCategory category, NodeType type, NodeManager* manager);
};

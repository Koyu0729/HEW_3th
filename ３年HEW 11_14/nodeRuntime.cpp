#include "main.h"
#include "nodeRuntime.h"
#include <iostream>

void NodeRuntime::Run(std::vector<NodeBase*> runNodes)
{
    m_RunNodes = runNodes;

    if (m_RunNodes.empty())
    {
        printf("実行ノードがない\n");
        return;
    }

    printf("\n\n[ノード処理実行開始]\n\n");

    // スタートノード探索
    NodeBase* current = FindStartNode();
    if (!current)
    {
        printf("Start ない\n");
        return;
    }

    NodeBase* last = nullptr;

    while (current)
    {
        printf("現在 : %ls (ID:%d)\n", current->GetName().c_str(), current->GetID());

        // 無限ループ防止
        if (current == last)
        {
            printf("同一ノードで停止: %ls\n", current->GetName().c_str());
            break;
        }

        NodeBase* next = current->Execute();

        if (!next)
        {
            printf("最後のノード: %ls (ID:%d)\n", current->GetName().c_str(), current->GetID());
            break;
        }

        last = current;
        current = next;
    }

    printf("\n[ノード処理実行終了]\n");
}

//スタートノードから開始
NodeBase* NodeRuntime::FindStartNode()
{
    for (auto* n : m_RunNodes)
    {
        if (n && n->GetCategory() == NodeCategory::Start)
            return n;
    }
    return nullptr;
}

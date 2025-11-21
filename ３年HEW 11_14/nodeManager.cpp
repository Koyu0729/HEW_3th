//内部処理の実行順のセーブ
//ver1.1 変更点：レジスターコネクトに再接続処理の追加

//json　:　参考Url　:　https://qiita.com/yohm/items/0f389ba5c5de4e2df9cf

#include "nodeManager.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;
void NodeManager::RegisterNode(NodeBase* node)
{
    if (!node) return;

    // 重複登録を防ぐ
    for (auto* n : m_Nodes)
    {
        if (n == node) return;
    }
    m_Nodes.push_back(node);
    printf("ノード登録: %ls\n", node->GetName().c_str());
}

void NodeManager::RegisterConnect(NodeBase* from, NodeBase* to, Port fromPort)
{
    if (!from || !to || from == to) return;

#pragma region セーブデータ登録処理
    //循環防止
    for (auto it = m_SaveData.begin(); it != m_SaveData.end(); )
    {
        // 逆方向の接続入れ替え
        if (it->fromID == to->GetID() && it->toID == from->GetID())
        {
            it = m_SaveData.erase(it);
            break; 
        }
        else
        {
            ++it;
        }
    }

    //再接続＋それに伴ったデータの消去
    for (auto it = m_SaveData.begin(); it != m_SaveData.end(); )
    {
        bool sameFromAndPort = (it->fromID == from->GetID() && it->fromPort == fromPort);
        bool sameToInput = (it->toID == to->GetID());

        if (sameFromAndPort || sameToInput)
        {
            it = m_SaveData.erase(it);
            printf("古い接続削除: From(ID:%d, Port:%d) → To(ID:%d)\n",
                from->GetID(), (int)fromPort, to->GetID());
        }
        else
        {
            ++it;
        }
    }

    //新規登録(完全に新しいなら追加)
    NodeConnectionSaveData newConnect;
    newConnect.fromID = from->GetID();
    newConnect.toID = to->GetID();
    newConnect.fromPort = fromPort;
    m_SaveData.push_back(newConnect);
#pragma endregion

    if (from->GetCategory() == NodeCategory::If)
    {
        from->SetNext(to, fromPort);  // OutTrue / OutFalse をそのまま反映
    }
    else
    {
        from->SetNext(to);
    }

    printf("新規接続: %d(%d) → %d\n", from->GetID(), (int)fromPort, to->GetID());
}

//やってること、現在選択してるノードの情報を全消し（destroy）
//もし、選択しているノードを接続先に指定しているノードがあったら、接続解除
void NodeManager::UnregisterConnect(NodeBase* selectNode)
{
    if (!selectNode) return;

    //セレクトノードに関連している接続をすべて消去（セーブデータ）
    for (auto it = m_SaveData.begin(); it != m_SaveData.end();)
    {
        if (it->fromID == selectNode->GetID())
        {
            //ifノードを消去する場合
            it = m_SaveData.erase(it);
        }
        else if (it->toID == selectNode->GetID())
        {
            //
            it = m_SaveData.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 実行用のポインタ消去
    for (auto* n : m_Nodes)
    {
        if (!n) continue;

        // 通常ノードの場合
        if (n->GetCategory() != NodeCategory::If)
        {
            if (n->GetNext() == selectNode)
            {
                n->SetNext(nullptr, Port::OutDef);
            }
        }
        else
        {
            // Ifノードの場合
            // true/false それぞれに selectNode が接続されてたら個別に切る
            if (n->GetNext() == selectNode)
            {
                n->SetNext(nullptr, Port::OutDef);
            }

            n->SetNext(
                n->GetNext() == selectNode ? nullptr : n->GetNext(),
                Port::OutTrue
            );

            // False側だけ selectNode に繋がっている？
            n->SetNext(
                n->GetNext() == selectNode ? nullptr : n->GetNext(),
                Port::OutFalse
            );
        }
    }

    printf("ノード(ID:%d) の接続を削除しました\n", selectNode->GetID());
}

NodeBase* NodeManager::GetNodeUseID(int id) const
{
    for (auto* node : m_Nodes)
    {
        if (node->GetID() == id)
            return node;
    }
    return nullptr;
}

void NodeManager::Clear()
{
    m_Nodes.clear();
    m_SaveData.clear();
    m_AddNextID = 0;
}

void NodeManager::PrintConnections() const
{
    printf("\n=== 接続一覧 ===\n");
    for (const auto& c : m_SaveData)
    {
        printf("From(ID:%d, Port:%d) → To(ID:%d)\n", c.fromID, (int)c.fromPort, c.toID);
    }
    printf("================\n\n");
}
void NodeManager::PrintAllConnections() const
{
    printf("\n=== [ノード接続情報一覧] ===\n");

    for (auto* n : m_Nodes)
    {
        if (!n) continue;

        // ノードの基本情報
        printf("ノードID:%d [%ls] (カテゴリ:%d)\n",
            n->GetID(), n->GetName().c_str(), (int)n->GetCategory());

        // If ノード（分岐ノード）の場合
        if (n->GetCategory() == NodeCategory::If)
        {
            // true/false 出力を個別に出す
            NodeBase* trueNext = n->GetNext(); // 一時的な使い方でもOK
            NodeBase* falseNext = nullptr;

            // Port対応のSetNextにしてる場合 →直接持ってるはず
            n->SetNext(nullptr, Port::OutTrue);
            n->SetNext(nullptr, Port::OutFalse);

            // もし true/false の getter があるなら:
            // auto* trueNext = branch->GetTrueNext();
            // auto* falseNext = branch->GetFalseNext();

            if (trueNext)
            {
                printf("  ├─ True側 → %ls (ID:%d)\n",
                    trueNext->GetName().c_str(), trueNext->GetID());
            }
            else
            {
                printf("  ├─ True側 → [なし]\n");
            }

            if (falseNext)
            {
                printf("  └─ False側 → %ls (ID:%d)\n",
                    falseNext->GetName().c_str(), falseNext->GetID());
            }
            else
            {
                printf("  └─ False側 → [なし]\n");
            }
        }
        else
        {
            // 通常ノードの次ノード表示
            NodeBase* next = n->GetNext();
            if (next)
            {
                printf("  └─ Next → %ls (ID:%d)\n",
                    next->GetName().c_str(), next->GetID());
            }
            else
            {
                printf("  └─ Next → [なし]\n");
            }
        }
    }

    printf("=== [ノード接続情報 終了] ===\n\n");
}

//void NodeManager::ToJson(const std::string& path)
//{
//    json j;
//
//    for (auto* node : m_Nodes)
//    {
//        j["Nodes"].push_back({
//            {"Category", (int)node->GetCategory()},
//            {"Type", (int)node->GetType()},
//            {"Name", node->GetName()},
//            {"ID", node->GetID()},
//            {"Next", (node->GetNext() ? node->GetNext()->GetID() : -1)}
//            });
//    }
//
//    for (const auto& c : m_SaveData)
//    {
//        j["Connections"].push_back({
//            {"fromID", c.fromID},
//            {"toID", c.toID},
//            {"fromPort", static_cast<int>(c.fromPort)}
//            });
//    }
//
//    std::ofstream file(path);
//    file << std::setw(4) << j;
//}

void NodeManager::LoadConnections()
{
    for (auto& c : m_SaveData)
    {
        NodeBase* from = GetNodeUseID(c.fromID);
        NodeBase* to = GetNodeUseID(c.toID);
        if (!from || !to) continue;

        if (from->GetCategory() == NodeCategory::If)
        {
            // 分岐側は保存していた fromPort を尊重して復元
            from->SetNext(to, c.fromPort);
        }
        else
        {
            from->SetNext(to);
        }
    }
    printf("セーブデータから接続を再構築しました。\n");
}

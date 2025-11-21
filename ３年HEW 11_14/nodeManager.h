//内部処理の登録（できてる）＆セーブ（まだ）
#pragma once
#include <vector>
#include <algorithm>
#include "nodeBase.h"


//セーブ用（基本実行はノードのポインタ参照で動くから実行だけを見るなら気にしなくていい）
struct NodeConnectionSaveData {
    int fromID;
    int toID;
    Port fromPort;
};

class NodeManager
{
private:
    // 内部処理格納用配列(生成したノードの処理が入ってる)
    std::vector<NodeBase*> m_Nodes;

    //接続構造体
    std::vector<NodeConnectionSaveData> m_SaveData;

    // ノードにつき１つ渡すID（かぶらない）
    int m_AddNextID = 0;
public:
    int CreateNodeID() { return m_AddNextID++; };

    //クリエイトノードで使う（内部処理の登録）
    void RegisterNode(NodeBase* node);

    //エディターで使う（実行する内部処理の連結登録）
    void RegisterConnect(NodeBase* from, NodeBase* to, Port fromPort);
    void UnregisterConnect(NodeBase* selectNode);

    NodeBase* GetNodeUseID(int id) const;

    //ノード単体
    const std::vector<NodeBase*>& GetNodes() const{ return m_Nodes; }

    //シーン閉じるときに呼ぶ
    void Clear();

    //デバック用
    void PrintConnections() const;
    void PrintAllConnections() const;

    //セーブ用配列のgetter
    const std::vector<NodeConnectionSaveData>& GetSaveConnections() const { return m_SaveData; }

    // セーブ＆ロード
    void ToJson(const std::string& path);
    void LoadConnections();
};

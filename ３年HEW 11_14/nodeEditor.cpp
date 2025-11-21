//ノードを接続するシーン　基本マネージャにデータを送って内部処理は実行する
//ver1.1　変更点：接続先を書き換えたときの処理完成！！！！！！！！！！！！！！
//ver1.2  変更点：分岐ノード追加
#include "main.h"
#include "manager.h"
#include "renderer.h"

//シーン
#include "NodeEditor.h"

//次のシーン
#include "gameMenu.h"

//シーン背景
#include "polyTex.h"

//ロゴ
#include "moji.h"

//UI
#include "camera.h"
#include "input.h"
#include "button.h"
#include "nodeCreateList.h"

//オブジェクト
//#include 特になし

//ラン
#include "nodeRuntime.h"

static NodeRuntime* m_Run = new NodeRuntime();

void NodeEditor::Init()
{
	AddGameObject3D<Camera>(0);

	//壁紙
	AddGameObject2D<PolyTex>(1)
		->SetTexRule(
            Vector2(0.0f, 0.0f),
            Vector2(0.0f, 0.0f),
            Vector2(SCREEN_WIDTH, SCREEN_HEIGHT),
            "asset\\texture\\wallpaper\\editor.jpg"
        );

    m_NodeManager = new NodeManager;
}

void NodeEditor::Uninit()
{
	Scene::Uninit();

	delete m_NodeManager;
	m_NodeManager = nullptr;
}

void NodeEditor::Update()
{
    Scene::Update();

    NodeDelete();
    NodeSelect();
    NodeConnect();
    NodeCreateInput();

  
    const auto& connections = m_NodeManager->GetSaveConnections();
    if (m_ConnectedLines.size() != connections.size())
    {
        UpdateConnections();
    }
    if (Input::GetKeyPress('1'))
    {
        Manager::SetRuntimeNodes(m_NodeManager->GetNodes());

        Manager::LoadingSetScene<GameMenu>();
    }
    if (Input::GetKeyPress('0'))
    {
        m_Run->Run(m_NodeManager->GetNodes());
    }   
    
    if (Input::GetKeyPress('9'))
    {
        m_NodeManager->PrintConnections();
    }
    if (Input::GetKeyPress('8'))
    {
        m_NodeManager->PrintAllConnections();
    }
}

void NodeEditor::Draw()
{
    Scene::Draw();

    if (!m_NodeManager) return;

    // ノード間の接続線を描画
    const auto& connections = m_NodeManager->GetSaveConnections();

    // 線の再利用（既存数と一致）
    if (m_ConnectedLines.size() != connections.size())
        UpdateConnections();

    for (int i = 0; i < connections.size(); ++i)
    {
        NodeBase* fromNode = m_NodeManager->GetNodeUseID(connections[i].fromID);
        NodeBase* toNode = m_NodeManager->GetNodeUseID(connections[i].toID);
        if (!fromNode || !toNode) continue;

        NodeView* fromView = nullptr;
        NodeView* toView = nullptr;

        for (auto* view : m_NodeViews)
        {
            if (view->GetNode() == fromNode) fromView = view;
            if (view->GetNode() == toNode)   toView = view;
        }

        if (fromView && toView && i < m_ConnectedLines.size())
        {
            Vector2 start = fromView->GetPortCenter(connections[i].fromPort);
            Vector2 end = toView->GetPortCenter(Port::In);
            m_ConnectedLines[i]->SetPoints(start, end);
        }
    }
}

//選択中ノード
void NodeEditor::NodeSelect()
{
    if (g_MouseLeftDown)
    {
        NodeView* clickedNode = nullptr;

        for (auto it = m_NodeViews.begin(); it != m_NodeViews.end(); ++it)
        {
            NodeView* view = *it;
            if (view && view->GetRect()->Inside2D())
            {
                clickedNode = view;
                break;
            }
        }

        // ノードがあれば選択、なければ解除
        m_SelectedNode = clickedNode;

        // 全ノードの見た目を更新
        for (auto* view : m_NodeViews)
        {
            view->SetHighlighted(view == m_SelectedNode);
        }
    }
}

//ノード接続処理
void NodeEditor::NodeConnect()
{
    POINT pos = GetMousePos();
    Vector2 mouse((float)pos.x, (float)pos.y);

    // 接続開始（押した瞬間）
    if (!m_Connecting && g_MouseLeftDown)
    {
        for (auto* view : m_NodeViews)
        {
            //
            if (view->InsidePort(Port::OutDef)  || view->InsidePort(Port::OutTrue) ||
                view->InsidePort(Port::OutFalse)|| view->InsidePort(Port::In))
            {
                m_Connecting = true;
                m_StartNode = view;

                if (view->InsidePort(Port::OutDef))
                {
                    m_StartPort = Port::OutDef;
                }
                else if (view->InsidePort(Port::OutTrue))
                {
                    m_StartPort = Port::OutTrue;
                }
                else if (view->InsidePort(Port::OutFalse))
                {
                    m_StartPort = Port::OutFalse;
                }
                else
                {
                    m_StartPort = Port::In;
                }

                // 線生成
                m_DragLine = AddGameObject2D<DrawLine>(8);
                m_DragLine->SetColor(XMFLOAT4(1, 1, 1, 1));
                m_DragLine->SetPoints(m_StartNode->GetPortCenter(m_StartPort), mouse);
                break;
            }
        }
    }

    // 接続中プレビュー線（押してる間）
    if (m_Connecting && g_MouseLeft)
    {
        if (m_DragLine && m_StartNode)
        {
            Vector2 start = m_StartNode->GetPortCenter(m_StartPort);
            m_DragLine->SetPoints(start, mouse);
        }
    }
    
    // 接続確定
    if (m_Connecting && g_MouseLeftUp)
    {
        //繋いだ時にoutになるノード
        NodeView* targetView = nullptr;
        //繋いだ時にoutになるノードのポート
        Port toPort = Port::PortNull;

        // 接続先探索　(　他ノードについてるポートの当たり判定がtrueだったら　)
        for (auto* view : m_NodeViews)
        {
            if (view == m_StartNode) continue;

            if (view->InsidePort(Port::In))
            {
                targetView = view;
                toPort = Port::In;
                break;
            }
            else if (view->InsidePort(Port::OutDef) || view->InsidePort(Port::OutTrue) || view->InsidePort(Port::OutFalse))
            {
                targetView = view;

                if (view->InsidePort(Port::OutTrue))
                {
                    toPort = Port::OutTrue;
                }
                else if (view->InsidePort(Port::OutFalse))
                {
                    toPort = Port::OutFalse;
                }
                else
                {
                    toPort = Port::OutDef;
                }
                break;
            }
        }

        //inからin、outからoutの接続を無効化
        if (m_StartPort == Port::OutDef && toPort == Port::OutDef || 
            m_StartPort == Port::In     && toPort == Port::In)
        {
            printf("out>out,in>in ダメ絶対\n");
            m_DragLine = nullptr;
            m_Connecting = false;
            m_StartNode = nullptr;
            m_StartPort = Port::PortNull;
            return;
        }

        //登録処理
        if (m_DragLine && m_StartNode && targetView)
        {
            // 方向判定
            NodeBase* from = nullptr;
            NodeBase* to = nullptr;
            Port fromPort = m_StartPort;
            //inからoutでも接続可能
            if (m_StartPort == Port::OutDef   && toPort == Port::In ||
                m_StartPort == Port::OutTrue  && toPort == Port::In ||
                m_StartPort == Port::OutFalse && toPort == Port::In)
            {
                from = m_StartNode->GetNode();
                to = targetView->GetNode();
                fromPort = m_StartPort;
            }
            else if (m_StartPort == Port::In &&
                    (toPort == Port::OutDef || toPort == Port::OutTrue || toPort == Port::OutFalse))
            {
                from = targetView->GetNode();
                to = m_StartNode->GetNode();
                fromPort = toPort;
            }

            //=====================================
            //ここが接続処理の根幹
            //再接続 or 新規接続処理
            //=====================================
            if (from && to && from != to)
            {
                m_NodeManager->RegisterConnect(from, to, fromPort);

                Vector2 startPos;
                Vector2 endPos;

                if (m_StartPort == Port::OutDef || m_StartPort == Port::OutTrue || m_StartPort == Port::OutFalse)
                {
                    startPos = m_StartNode->GetPortCenter(fromPort);
                    endPos = targetView->GetPortCenter(Port::In);
                }
                else
                {
                    startPos = targetView->GetPortCenter(fromPort);
                    endPos = m_StartNode->GetPortCenter(Port::In);
                }

                m_DragLine->SetPoints(startPos, endPos);
                m_ConnectedLines.push_back(m_DragLine);
            }
            else
            {
                // 接続失敗　線を消す
                if (m_DragLine)
                {
                    m_DragLine->SetDestroy();
                }
            }
        }
        if (!targetView)
        {
            if (m_DragLine)
            {
                m_DragLine->SetDestroy();
                m_DragLine = nullptr;
            }
        }
        //リセット
        m_DragLine = nullptr;
        m_Connecting = false;
        m_StartNode = nullptr;
        m_StartPort = Port::PortNull;
    }
}

//生成
void NodeEditor::NodeCreateInput()
{
    POINT pos = GetMousePos();
    Vector2 mouse((float)pos.x, (float)pos.y);

    //右クリ押してる間
    if (!m_NodeCreateMode && g_MouseRightDown)
    {
        m_NodeCreateMode = true;

        //リスト
        m_CreateList = AddGameObject2D<NodeCreateList>(9);
    }

    //右クリ押してる間に、ノード生成（listをクリックしてできるようにする）
    if (m_NodeCreateMode)
    {
        //ここで生成処理（絶対、変える）
        if (Input::GetKeyPress('4'))
        {
            bool hasStart = false;

            for (auto* view : m_NodeViews)
            {
                if (view->GetNode()->GetCategory() == NodeCategory::Start)
                {
                    hasStart = true;
                    break;
                }
            }
            if (!hasStart)
            {
                m_NodeViews.push_back(NodeCreate::Create(NodeCategory::Start, NodeType::Start, m_NodeManager));
            }
            else
            {
                printf("スタートもうある");
            }
            m_NodeCreateMode = false;
        }
        if (Input::GetKeyPress('5'))
        {
            m_NodeViews.push_back(NodeCreate::Create(NodeCategory::Attack, NodeType::Attack1, m_NodeManager));
            m_NodeCreateMode = false;
        }
        else if (Input::GetKeyPress('6'))
        {
            m_NodeViews.push_back(NodeCreate::Create(NodeCategory::Movement, NodeType::Forward, m_NodeManager));
            m_NodeCreateMode = false;
        }
        else if (Input::GetKeyPress('7'))
        {
            m_NodeViews.push_back(NodeCreate::Create(NodeCategory::Movement, NodeType::Back, m_NodeManager));
            m_NodeCreateMode = false;
        }
        else if (Input::GetKeyPress('8'))
        {
            m_NodeViews.push_back(NodeCreate::Create(NodeCategory::If, NodeType::If1, m_NodeManager));
            m_NodeCreateMode = false;
        }

        //生成する前に離したら（リスト以外をクリックしたらにする）
        if (g_MouseRightUp)
        {
            m_NodeCreateMode = false;
        }

        if (!m_NodeCreateMode)
        {
            m_CreateList->SetDestroy();
        }
    }
}

//繋がっているものの途中を崩すって難しいんだなって思いました。
//[消すもの]
//・NodeManager.m_Connections[selectNode]
//・NodeEditor.m_SelectedNode[selectNode]
void NodeEditor::NodeDelete()
{
    if (Input::GetKeyPress(VK_DELETE) && m_SelectedNode)
    {
        NodeBase* node = m_SelectedNode->GetNode();


        //次に内部
        m_NodeManager->UnregisterConnect(node);

        for (int i = 0; i < m_NodeManager->GetSaveConnections().size(); ++i)
        {
            const auto& c = m_NodeManager->GetSaveConnections()[i];
            if (c.fromID == node->GetID() || c.toID == node->GetID())
            {
                if (i < m_ConnectedLines.size())
                {
                    m_ConnectedLines[i]->SetDestroy();
                }
            }
        }

        //最後に、ノード削除
        m_SelectedNode->SetDestroy();
        m_NodeViews.erase(
            std::remove(m_NodeViews.begin(), m_NodeViews.end(), m_SelectedNode),
            m_NodeViews.end()
        );
        m_SelectedNode = nullptr;
    }
}

void NodeEditor::UpdateConnections()
{
    const auto& connects = m_NodeManager->GetSaveConnections();

    // 線数の再構築
    if (m_ConnectedLines.size() != connects.size())
    {
        while (m_ConnectedLines.size() < connects.size())
            m_ConnectedLines.push_back(AddGameObject2D<DrawLine>(8));

        while (m_ConnectedLines.size() > connects.size())
        {
            m_ConnectedLines.back()->SetDestroy();
            m_ConnectedLines.pop_back();
        }
    }

    // 線の再配置（ポート情報対応）
    for (int i = 0; i < connects.size(); ++i)
    {
        NodeBase* fromNode = m_NodeManager->GetNodeUseID(connects[i].fromID);
        NodeBase* toNode = m_NodeManager->GetNodeUseID(connects[i].toID);
        if (!fromNode || !toNode) continue;

        NodeView* fromView = nullptr;
        NodeView* toView = nullptr;

        for (auto* v : m_NodeViews)
        {
            if (v->GetNode() == fromNode) fromView = v;
            if (v->GetNode() == toNode)   toView = v;
        }

        if (fromView && toView && i < m_ConnectedLines.size())
        {
            Vector2 start = fromView->GetPortCenter(connects[i].fromPort);
            Vector2 end = toView->GetPortCenter(Port::In);
            m_ConnectedLines[i]->SetPoints(start, end);
        }
    }
}

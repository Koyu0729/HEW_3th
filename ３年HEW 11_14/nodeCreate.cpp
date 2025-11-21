#include "main.h"
#include "nodeCreate.h"
#include "nodeIncludes.h"
#include "manager.h"

//関数一つとはいえ何行になるのでしょう？
NodeView* NodeCreate::Create(NodeCategory category, NodeType type, NodeManager* manager)
{
    if (!manager) return nullptr;

    NodeBase* logic = nullptr;
    int id = manager->CreateNodeID();

    switch (category)
    {
    case NodeCategory::Start:
        if (type == NodeType::Start)
        {
            logic = new Start(id);
        }
        break;

    case NodeCategory::If:
        if (type == NodeType::If1)
        {
            logic = new If1(id);
        }
        else if (type == NodeType::If2)
        {

        }
        break;

    case NodeCategory::Attack:       
        if (type == NodeType::Attack1)
        {
            logic = new Attack1(id);
        }
        break;

    case NodeCategory::Movement:
        if (type == NodeType::Forward)
        {
            logic = new MoveForward(id);
        }
        else if (type == NodeType::Back)
        {
            logic = new MoveBack(id);
        }
        break;

    default:
        printf("カテゴリーがない");
        break;
    }

    // ノードマネージャに登録
    manager->RegisterNode(logic);
    // NodeView作成
    NodeView* view = Manager::GetScene()->AddGameObject2D<NodeView>(4);
    view->Init(logic);

    printf("\n%d:%d\n", view->GetNode()->GetCategory(), view->GetNode()->GetType());
    return view;
}

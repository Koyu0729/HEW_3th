#pragma once
#include "moveBase.h"

class MoveBack : public MoveBase
{
private:
    Player* m_Player = nullptr;
public:
    MoveBack(int id) : MoveBase(id, NodeType::Back)
    {
    }

    NodeBase* Execute() override;
};

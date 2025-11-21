#pragma once
#include "moveBase.h"

class MoveForward : public MoveBase
{
private:
    Player* m_Player = nullptr;

public:
    MoveForward (int id) : MoveBase(id, NodeType::Forward)
    {
    }

    NodeBase* Execute() override;
};

#pragma once
#include "moveBase.h"

class Jump : public MoveBase
{
private:
    Player* m_Player = nullptr;
    int m_CoolTime = 0;
    bool m_IsJumping = false;
public:
    Jump(int id) : MoveBase(id, NodeType::Jump)
    {
    }

    NodeBase* Execute() override;
};

#pragma once
#include "attackBase.h"

class Attack1 : public AttackBase
{
public:
    Attack1(int id)
        : AttackBase(id, NodeType::Attack1)
    {
        m_Damage = 10.0f;
    }

    NodeBase* Execute() override;
};

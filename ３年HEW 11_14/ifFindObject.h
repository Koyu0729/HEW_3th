//ç°ÇÃÇ∆Ç±ÇÎìGÇæÇØ
#pragma once
#include "branchBase.h"

class IfFindObject : public BranchBase
{
public:
    IfFindObject(int id) : BranchBase(id, NodeType::If1)
    {
    }

    bool Branch() override;

    NodeBase* Execute() override;
};


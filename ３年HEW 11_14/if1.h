#pragma once
#include "branchBase.h"

class If1 : public BranchBase
{
public:
    If1(int id) : BranchBase(id, NodeType::If1)
    {
    }

    bool Branch() override;

    NodeBase* Execute() override;
};

#include "main.h"
#include "ifFindObject.h"
#include "input.h"

bool IfFindObject::Branch()
{
    bool condition = false;

    if (Input::GetKeyPress('B'))
    {
        condition = true;
    }

    return condition;
}

NodeBase* IfFindObject::Execute()
{
    printf("ifÇPÇÃèàóùÇ≈Ç∑");

    m_BranchResult = Branch();

    if (m_BranchResult && m_TrueNext)
    {
        printf("Å® True ë§Ç÷\n");
        return m_TrueNext;
    }
    else if (!m_BranchResult && m_FalseNext)
    {
        printf("Å® False ë§Ç÷\n");
        return m_FalseNext;
    }
    else
    {
        return nullptr;
    }
}

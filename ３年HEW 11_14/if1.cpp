#include "main.h"
#include "if1.h"
#include "input.h"

bool If1::Branch()
{
    bool condition = false;

    if (Input::GetKeyPress('B'))
    {
        condition = true;
    }

    return condition;
}

NodeBase* If1::Execute()
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

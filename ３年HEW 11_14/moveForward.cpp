#include "moveForward.h"
#include "vector3.h"

#include "player.h"
#include "input.h"

NodeBase* MoveForward::Execute()
{
    m_Player = GetPlayer();

    Vector3 forward = m_Player->GetForward();
    m_Player->SetPosition(m_Player->GetPosition() + forward * m_Player->GetMoveSpeed());
    m_Name = L"‘O‚Éi‚Ş";

    return GetNext();
}

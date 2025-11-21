#include "moveBack.h"
#include "vector3.h"

#include "player.h"
#include "input.h"

NodeBase* MoveBack::Execute()
{
    m_Player = GetPlayer();

    Vector3 forward = m_Player->GetForward();
    m_Player->SetPosition(m_Player->GetPosition() - forward * m_Player->GetMoveSpeed());


    return GetNext();
}

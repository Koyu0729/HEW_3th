#include "jump.h"
#include "vector3.h"

#include "player.h"
#include "input.h"

Player* aplayer = nullptr;

int m_CoolTime;

NodeBase* Jump::Execute()
{
    if (!m_Player)
        m_Player = GetPlayer();

    Vector3 pos = m_Player->GetPosition();

    // ジャンプ開始
    if (!m_IsJumping)
    {
        m_IsJumping = true;
        m_CoolTime = 0;
    }

    if (m_IsJumping)
    {
        // 上昇中
        if (m_CoolTime < 10)
        {
            pos.y += 0.2f;
            m_CoolTime++;
        }
        // 下降中
        else if (m_CoolTime < 20)
        {
            pos.y -= 0.2f;
            m_CoolTime++;
        }
        // 終了
        else
        {
            m_IsJumping = false;
            return GetNext(); // 次のノードへ
        }

        m_Player->SetPosition(pos);
    }

    return GetNext();
}

//#include "moveToTarget.h"
//#include "vector3.h"
//
//#include "manager.h"
//#include "player.h"
//#include "enemy.h"
//
//NodeBase* MoveToTarget::Execute()
//{
//    m_Player = GetPlayer();
//
//#pragma region エネミー
//
//    Enemy* target = Manager::GetScene()->GetGameObject<Enemy>();
//
//    if (!target)
//    {
//        printf("ターゲット無い");
//        return GetNext();
//    }
//
//    Vector3 selfPos = m_Player->GetPosition();
//    Vector3 targetPos = target->GetPosition();
//
//    Vector3 dir = targetPos - selfPos;
//    float dist = dir.length();
//
//    if (dist < 0.5f)
//        return GetNext();  // 到着扱い
//
//    dir.normalize();
//
//    float speed = m_Player->GetMoveSpeed();
//    float delta = m_Runtime->GetDeltaTime();
//
//    selfPos += dir * speed * delta;
//
//    m_Player->SetPosition(selfPos);
//
//
//#pragma endregion
//
//    return GetNext();
//}

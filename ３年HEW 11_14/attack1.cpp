#include "attack1.h"

NodeBase* Attack1::Execute()
{
    printf("Attack1‚Ìˆ—‚Å‚·\n");
    printf("ƒ_ƒ[ƒW:%.1f\n", m_Damage);

    return GetNext();
}

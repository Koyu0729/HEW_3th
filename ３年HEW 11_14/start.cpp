#include "start.h"

NodeBase* Start::Execute()
{
    printf("スタートノードの処理です\n");

    return GetNext();
}

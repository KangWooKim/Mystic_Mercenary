#include "UTemporaryBuffManager.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"

void UTemporaryBuffManager::ApplyRandomBuff(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        // ���� ȿ���� �����ϰ� ���� (��: ���ݷ� ����, �ӵ� ���� ��)
        float RandomBuffValue = FMath::RandRange(5.0f, 20.0f);
        Character->AddTemporaryBuff(RandomBuffValue);
    }
}
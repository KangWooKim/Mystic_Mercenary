#include "UTemporaryBuffManager.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"

void UTemporaryBuffManager::ApplyRandomBuff(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        // 버프 효과를 랜덤하게 적용 (예: 공격력 증가, 속도 증가 등)
        float RandomBuffValue = FMath::RandRange(5.0f, 20.0f);
        Character->AddTemporaryBuff(RandomBuffValue);
    }
}
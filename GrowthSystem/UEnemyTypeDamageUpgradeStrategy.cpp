#include "UEnemyTypeDamageUpgradeStrategy.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"

UEnemyTypeDamageUpgradeStrategy::UEnemyTypeDamageUpgradeStrategy()
    : DamageIncrease(10.0f) // 기본 데미지 증가량 설정
{
}

void UEnemyTypeDamageUpgradeStrategy::ApplyUpgrade(AGreatSwordCharacter* Character)
{
    if (Character && Character->IsEnemyType(EnemyType))
    {
        Character->IncreaseDamageAgainstEnemyType(EnemyType, DamageIncrease);
    }
}
#include "UEnemyTypeDamageUpgradeStrategy.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"

UEnemyTypeDamageUpgradeStrategy::UEnemyTypeDamageUpgradeStrategy()
    : DamageIncrease(10.0f) // �⺻ ������ ������ ����
{
}

void UEnemyTypeDamageUpgradeStrategy::ApplyUpgrade(AGreatSwordCharacter* Character)
{
    if (Character && Character->IsEnemyType(EnemyType))
    {
        Character->IncreaseDamageAgainstEnemyType(EnemyType, DamageIncrease);
    }
}
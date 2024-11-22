#pragma once

#include "CoreMinimal.h"
#include "IUpgradeStrategy.h"
#include "UEnemyTypeDamageUpgradeStrategy.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API UEnemyTypeDamageUpgradeStrategy : public UObject, public IUpgradeStrategy
{
    GENERATED_BODY()

public:
    UEnemyTypeDamageUpgradeStrategy();

    virtual void ApplyUpgrade(class AGreatSwordCharacter* Character) override;

    void SetEnemyType(FName InEnemyType) { EnemyType = InEnemyType; }
    void SetDamageIncrease(float InDamageIncrease) { DamageIncrease = InDamageIncrease; }

private:
    FName EnemyType;
    float DamageIncrease;
};
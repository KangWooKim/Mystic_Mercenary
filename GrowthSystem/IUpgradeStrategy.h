#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IUpgradeStrategy.generated.h"

class AGreatSwordCharacter;

UINTERFACE(MinimalAPI)
class UUpgradeStrategy : public UInterface
{
    GENERATED_BODY()
};

class MYSTIC_MERCENARY_API IUpgradeStrategy
{
    GENERATED_BODY()

public:
    // 업그레이드를 적용하는 메서드
    virtual void ApplyUpgrade(AGreatSwordCharacter* Character) = 0;
};
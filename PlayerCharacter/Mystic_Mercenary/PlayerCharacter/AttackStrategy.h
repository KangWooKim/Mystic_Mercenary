#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackStrategy.generated.h"

class AGreatSwordCharacter;

UINTERFACE(MinimalAPI)
class UAttackStrategy : public UInterface
{
    GENERATED_BODY()
};

/**
 * 공격 전략 인터페이스
 */
class MYSTIC_MERCENARY_API IAttackStrategy : public IInterface
{
    GENERATED_BODY()

public:
    

    virtual void ExecuteAttack(AGreatSwordCharacter* Character) = 0;
};

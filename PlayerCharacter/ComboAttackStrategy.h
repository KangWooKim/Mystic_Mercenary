// ComboAttackStrategy.h

#pragma once

#include "CoreMinimal.h"
#include "AttackStrategy.h"
#include "GreatSwordCharacter.h"

class ComboAttackStrategy : public IAttackStrategy
{
public:
    ComboAttackStrategy(EComboType InComboType);

    virtual void ExecuteAttack(AGreatSwordCharacter* Character) override;

private:
    EComboType ComboType;
};

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "HitState.generated.h"

UCLASS()
class UHitState : public UCharacterState
{
    GENERATED_BODY()

public:
    virtual void EnterState(AGreatSwordCharacter* Character) override;
    virtual void UpdateState(AGreatSwordCharacter* Character, float DeltaTime) override;
    virtual void ExitState(AGreatSwordCharacter* Character) override;
};
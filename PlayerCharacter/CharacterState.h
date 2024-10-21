#pragma once

#include "CoreMinimal.h"
#include "CharacterState.generated.h"

class AGreatSwordCharacter;

UCLASS()
class UCharacterState : public UObject
{
    GENERATED_BODY()

public:
    virtual void EnterState(AGreatSwordCharacter* Character) PURE_VIRTUAL(UCharacterState::EnterState, );
    virtual void UpdateState(AGreatSwordCharacter* Character, float DeltaTime) PURE_VIRTUAL(UCharacterState::UpdateState, );
    virtual void ExitState(AGreatSwordCharacter* Character) PURE_VIRTUAL(UCharacterState::ExitState, );
};
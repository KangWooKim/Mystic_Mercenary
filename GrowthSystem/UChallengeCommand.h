#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"
#include "UChallengeCommand.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API UChallengeCommand : public UObject
{
    GENERATED_BODY()

public:
    virtual void Execute(AGreatSwordCharacter* Character);
};
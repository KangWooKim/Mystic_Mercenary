#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UTemporaryBuffManager.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API UTemporaryBuffManager : public UObject
{
    GENERATED_BODY()

public:
    void ApplyRandomBuff(class AGreatSwordCharacter* Character);
};
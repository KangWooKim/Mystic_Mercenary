#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"
#include "URogueMetaProgressionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyDefeatedDelegate, FName, EnemyType, AGreatSwordCharacter*, Character);

UCLASS()
class MYSTIC_MERCENARY_API URogueMetaProgressionManager : public UObject
{
    GENERATED_BODY()

public:
    static URogueMetaProgressionManager* Get();

    void OnEnemyDefeated(FName EnemyType, AGreatSwordCharacter* Character);

    UPROPERTY(BlueprintAssignable)
        FEnemyDefeatedDelegate EnemyDefeated;

private:
    static URogueMetaProgressionManager* Instance;
};

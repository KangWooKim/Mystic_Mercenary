#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DungeonGameMode.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API ADungeonGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    void GenerateDungeonMap();

private:
    FVector GetNextRoomPosition(FVector CurrentPosition);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FieldGameMode.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API AFieldGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFieldGameMode();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
        void GenerateFieldMap();

private:
    UFUNCTION()
        void LoadRandomSegments();

    UFUNCTION()
        void OnSegmentLoaded();

    UFUNCTION()
        void CheckLevelLoaded(FName SegmentName);

    UPROPERTY(EditAnywhere, Category = "Segments")
        TArray<FName> SegmentNames;

    UPROPERTY(EditAnywhere, Category = "Segments")
        FVector SegmentSize;

    TMap<FName, FVector> SegmentPositions;
    int32 SegmentsToLoad;
    int32 LoadedSegmentsCount;
};

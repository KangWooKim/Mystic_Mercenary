#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "FieldMapGenerator.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API AFieldMapGenerator : public AActor
{
    GENERATED_BODY()

public:
    AFieldMapGenerator();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Level Segments")
        TArray<FString> SegmentNames;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        TArray<UStaticMesh*> TreeMeshes;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        int32 NumberOfTrees;

    UPROPERTY()
        UHierarchicalInstancedStaticMeshComponent* TreeHISMComponent;

    UFUNCTION()
        void LoadRandomSegments();

    UFUNCTION()
        void PositionSegment(const FString& SegmentName, const FVector& Position);

    void ShuffleArray(TArray<FString>& Array);
    void SpawnRandomTrees();
    float GetSurfaceZAtLocation(const FVector& Location); // 함수 선언 추가

    UFUNCTION()
        void OnSegmentLoaded(); // 세그먼트 로드 완료 후 호출될 함수 선언

    int32 SegmentsToLoad; // 로드해야 할 세그먼트의 수
    TArray<FVector> SegmentPositions; // 세그먼트 위치 저장
};

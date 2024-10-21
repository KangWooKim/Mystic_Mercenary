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
    float GetSurfaceZAtLocation(const FVector& Location); // �Լ� ���� �߰�

    UFUNCTION()
        void OnSegmentLoaded(); // ���׸�Ʈ �ε� �Ϸ� �� ȣ��� �Լ� ����

    int32 SegmentsToLoad; // �ε��ؾ� �� ���׸�Ʈ�� ��
    TArray<FVector> SegmentPositions; // ���׸�Ʈ ��ġ ����
};

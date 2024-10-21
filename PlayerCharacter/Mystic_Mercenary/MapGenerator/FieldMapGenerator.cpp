#include "FieldMapGenerator.h"
#include "Engine/World.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h" // ����׿� ���

// �ʿ��� ��� ���� �߰�
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"

AFieldMapGenerator::AFieldMapGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    // HISM ������Ʈ �ʱ�ȭ
    TreeHISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("TreeHISMComponent"));
    RootComponent = TreeHISMComponent;
}

void AFieldMapGenerator::BeginPlay()
{
    Super::BeginPlay();
    LoadRandomSegments();

    // ��ī�̹ڽ� �߰�
    ASkyLight* SkyLight = GetWorld()->SpawnActor<ASkyLight>(ASkyLight::StaticClass());
    if (SkyLight)
    {
        SkyLight->GetLightComponent()->SetIntensity(0.2f); // ��ο� ����
        SkyLight->GetLightComponent()->SetLightColor(FLinearColor(0.1f, 0.1f, 0.3f)); // Ǫ������ ���� ��ο� ��
    }

    // ���� �߰�
    AExponentialHeightFog* HeightFog = GetWorld()->SpawnActor<AExponentialHeightFog>(AExponentialHeightFog::StaticClass());
    if (HeightFog)
    {
        HeightFog->GetComponent()->SetFogDensity(0.05f); // £�� �Ȱ�
        HeightFog->GetComponent()->SetFogInscatteringColor(FLinearColor(0.1f, 0.1f, 0.2f)); // ��ο� ȸ��/Ǫ���� �Ȱ�
    }

    // Directional Light �߰�
    ADirectionalLight* DirectionalLight = GetWorld()->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0.0f, 0.0f, 1000.0f), FRotator(-45.0f, 0.0f, 0.0f));
    if (DirectionalLight)
    {
        UDirectionalLightComponent* DirectionalLightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent());
        if (DirectionalLightComponent)
        {
            DirectionalLightComponent->SetIntensity(0.1f); // ���� ����
            DirectionalLightComponent->SetLightColor(FLinearColor(0.2f, 0.2f, 0.5f)); // Ǫ������ ���� ��
            DirectionalLightComponent->CastShadows = true; // �׸��� ����
        }
    }
}

void AFieldMapGenerator::LoadRandomSegments()
{
    // ���׸�Ʈ �̸� �迭�� �����ݴϴ�.
    ShuffleArray(SegmentNames);

    // ���׸�Ʈ ũ�� ���� (�������� ��ȯ�� ũ��)
    int32 SegmentSizeX = 9200; // X ���� ũ��
    int32 SegmentSizeY = 6100; // Y ���� ũ��

    FVector CurrentPosition = FVector::ZeroVector;
    SegmentsToLoad = SegmentNames.Num(); // �ε��ؾ� �� ���׸�Ʈ �� �ʱ�ȭ

    for (int32 i = 0; i < SegmentNames.Num(); ++i)
    {
        FString SegmentName = SegmentNames[i];
        FVector SegmentPosition = FVector((i % 3) * SegmentSizeX, (i / 3) * SegmentSizeY, 0.0f);

        PositionSegment(SegmentName, SegmentPosition);
        SegmentPositions.Add(SegmentPosition); // ���׸�Ʈ ��ġ ����
    }
}

void AFieldMapGenerator::PositionSegment(const FString& SegmentName, const FVector& Position)
{
    FTransform SegmentTransform(FRotator::ZeroRotator, Position);
    bool bOutSuccess;

    FString FullPath = SegmentName;
    if (!SegmentName.StartsWith("/Game/"))
    {
        FullPath = FString("/Game/") + SegmentName;
    }

    // ����� �α׷� ��ü ��� Ȯ��
    UE_LOG(LogTemp, Warning, TEXT("Trying to load segment: %s"), *FullPath);

    ULevelStreamingDynamic* LoadedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        TSoftObjectPtr<UWorld>(FSoftObjectPath(FullPath)),
        SegmentTransform,
        bOutSuccess,
        SegmentName,  // ���⼭ ��ü ��� ��� �̸��� ����
        ULevelStreamingDynamic::StaticClass(),
        true
    );

    if (LoadedLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("Loaded segment: %s at position X=%f Y=%f Z=%f"), *FullPath, Position.X, Position.Y, Position.Z);
        if (!LoadedLevel->IsLevelLoaded())
        {
            LoadedLevel->OnLevelLoaded.AddDynamic(this, &AFieldMapGenerator::OnSegmentLoaded);
        }
        else
        {
            OnSegmentLoaded(); // �̹� �ε�� ��� �������� �̺�Ʈ ȣ��
        }
        LoadedLevel->SetShouldBeVisible(true);
        LoadedLevel->SetShouldBeLoaded(true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load segment: %s"), *FullPath);
    }
}

void AFieldMapGenerator::OnSegmentLoaded()
{
    SegmentsToLoad--;
    UE_LOG(LogTemp, Warning, TEXT("Segment loaded. Remaining segments: %d"), SegmentsToLoad);

    if (SegmentsToLoad <= 0)
    {
        // ��� ���׸�Ʈ�� �ε�� �� ������ ����
        UE_LOG(LogTemp, Warning, TEXT("All segments loaded. Spawning trees."));
        SpawnRandomTrees();
    }
}

void AFieldMapGenerator::ShuffleArray(TArray<FString>& Array)
{
    for (int32 i = Array.Num() - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        Array.Swap(i, j);
    }
}

void AFieldMapGenerator::SpawnRandomTrees()
{
    if (TreeMeshes.Num() == 0) return;

    int32 SegmentSizeX = 9200;
    int32 SegmentSizeY = 6100;

    for (int32 i = 0; i < NumberOfTrees; ++i)
    {
        // ���׸�Ʈ �� �ϳ��� �������� ����
        FVector SegmentPosition = SegmentPositions[FMath::RandRange(0, SegmentPositions.Num() - 1)];
        float RandomX = FMath::RandRange(-SegmentSizeX / 2, SegmentSizeX / 2); // ���׸�Ʈ ũ�� ������ ���� ��ġ ����
        float RandomY = FMath::RandRange(-SegmentSizeY / 2, SegmentSizeY / 2); // ���׸�Ʈ ũ�� ������ ���� ��ġ ����
        FVector RandomLocationXY = SegmentPosition + FVector(RandomX, RandomY, 0.0f);

        // ���� ���� ���ϱ�
        float RandomLocationZ = GetSurfaceZAtLocation(RandomLocationXY);
        FVector RandomLocation = FVector(RandomLocationXY.X, RandomLocationXY.Y, RandomLocationZ);

        // ���� ���� ����
        UStaticMesh* RandomTreeMesh = TreeMeshes[FMath::RandRange(0, TreeMeshes.Num() - 1)];
        TreeHISMComponent->SetStaticMesh(RandomTreeMesh);

        // ���� �ν��Ͻ� �߰�
        FTransform TreeTransform(FRotator::ZeroRotator, RandomLocation);
        TreeHISMComponent->AddInstance(TreeTransform, true);
    }
}

float AFieldMapGenerator::GetSurfaceZAtLocation(const FVector& Location)
{
    FHitResult HitResult;
    FVector Start = Location + FVector(0, 0, 1000.0f); // ������ �Ʒ��� Ʈ���̽�
    FVector End = Location - FVector(0, 0, 2000.0f); // �Ʒ��� �� �� �Ÿ��� Ʈ���̽�

    // ����� ���� �߰�
    DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 5.0f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility, // ECC_WorldStatic ä�� ��� ECC_Visibility ���
        CollisionParams
    );

    if (bHit)
    {
        // Ʈ���̽� ����: �Ķ��� ����
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f);
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit at location: %s with Z=%f"), *HitResult.Location.ToString(), HitResult.Location.Z);
        return HitResult.Location.Z;
    }
    else
    {
        // Ʈ���̽� ����: ������ ����
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f);
        UE_LOG(LogTemp, Warning, TEXT("Line trace failed at location: %s"), *Location.ToString());

        // �ֺ� ��ġ�� �ٽ� Ʈ���̽�
        const float SearchRadius = 1000.0f;
        const int32 NumTraces = 8;
        for (int32 i = 0; i < NumTraces; ++i)
        {
            float Angle = FMath::DegreesToRadians(360.0f / NumTraces * i);
            FVector Offset = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * SearchRadius;
            FVector NewStart = Start + Offset;
            FVector NewEnd = End + Offset;

            // ����� ���� �߰�
            DrawDebugLine(GetWorld(), NewStart, NewEnd, FColor::Yellow, false, 5.0f);

            bHit = GetWorld()->LineTraceSingleByChannel(
                HitResult,
                NewStart,
                NewEnd,
                ECC_Visibility, // ECC_WorldStatic ä�� ��� ECC_Visibility ���
                CollisionParams
            );

            if (bHit)
            {
                DrawDebugLine(GetWorld(), NewStart, NewEnd, FColor::Green, false, 1.0f);
                UE_LOG(LogTemp, Warning, TEXT("Fallback line trace hit at location: %s with Z=%f"), *HitResult.Location.ToString(), HitResult.Location.Z);
                return HitResult.Location.Z;
            }
        }

        // �ֺ� ��ġ������ ������ ��� �����ϰ� �ٴڿ� ��ġ
        float DefaultZ = 0.0f;
        return DefaultZ;
    }
}

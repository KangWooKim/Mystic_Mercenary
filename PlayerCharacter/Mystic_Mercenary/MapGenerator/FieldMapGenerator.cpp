#include "FieldMapGenerator.h"
#include "Engine/World.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h" // 디버그용 헤더

// 필요한 헤더 파일 추가
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"

AFieldMapGenerator::AFieldMapGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    // HISM 컴포넌트 초기화
    TreeHISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("TreeHISMComponent"));
    RootComponent = TreeHISMComponent;
}

void AFieldMapGenerator::BeginPlay()
{
    Super::BeginPlay();
    LoadRandomSegments();

    // 스카이박스 추가
    ASkyLight* SkyLight = GetWorld()->SpawnActor<ASkyLight>(ASkyLight::StaticClass());
    if (SkyLight)
    {
        SkyLight->GetLightComponent()->SetIntensity(0.2f); // 어두운 조명
        SkyLight->GetLightComponent()->SetLightColor(FLinearColor(0.1f, 0.1f, 0.3f)); // 푸른빛이 도는 어두운 색
    }

    // 포그 추가
    AExponentialHeightFog* HeightFog = GetWorld()->SpawnActor<AExponentialHeightFog>(AExponentialHeightFog::StaticClass());
    if (HeightFog)
    {
        HeightFog->GetComponent()->SetFogDensity(0.05f); // 짙은 안개
        HeightFog->GetComponent()->SetFogInscatteringColor(FLinearColor(0.1f, 0.1f, 0.2f)); // 어두운 회색/푸른색 안개
    }

    // Directional Light 추가
    ADirectionalLight* DirectionalLight = GetWorld()->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0.0f, 0.0f, 1000.0f), FRotator(-45.0f, 0.0f, 0.0f));
    if (DirectionalLight)
    {
        UDirectionalLightComponent* DirectionalLightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent());
        if (DirectionalLightComponent)
        {
            DirectionalLightComponent->SetIntensity(0.1f); // 낮은 강도
            DirectionalLightComponent->SetLightColor(FLinearColor(0.2f, 0.2f, 0.5f)); // 푸른빛이 도는 색
            DirectionalLightComponent->CastShadows = true; // 그림자 생성
        }
    }
}

void AFieldMapGenerator::LoadRandomSegments()
{
    // 세그먼트 이름 배열을 섞어줍니다.
    ShuffleArray(SegmentNames);

    // 세그먼트 크기 조정 (유닛으로 변환된 크기)
    int32 SegmentSizeX = 9200; // X 방향 크기
    int32 SegmentSizeY = 6100; // Y 방향 크기

    FVector CurrentPosition = FVector::ZeroVector;
    SegmentsToLoad = SegmentNames.Num(); // 로드해야 할 세그먼트 수 초기화

    for (int32 i = 0; i < SegmentNames.Num(); ++i)
    {
        FString SegmentName = SegmentNames[i];
        FVector SegmentPosition = FVector((i % 3) * SegmentSizeX, (i / 3) * SegmentSizeY, 0.0f);

        PositionSegment(SegmentName, SegmentPosition);
        SegmentPositions.Add(SegmentPosition); // 세그먼트 위치 저장
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

    // 디버그 로그로 전체 경로 확인
    UE_LOG(LogTemp, Warning, TEXT("Trying to load segment: %s"), *FullPath);

    ULevelStreamingDynamic* LoadedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        TSoftObjectPtr<UWorld>(FSoftObjectPath(FullPath)),
        SegmentTransform,
        bOutSuccess,
        SegmentName,  // 여기서 전체 경로 대신 이름만 전달
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
            OnSegmentLoaded(); // 이미 로드된 경우 수동으로 이벤트 호출
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
        // 모든 세그먼트가 로드된 후 나무를 생성
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
        // 세그먼트 중 하나를 랜덤으로 선택
        FVector SegmentPosition = SegmentPositions[FMath::RandRange(0, SegmentPositions.Num() - 1)];
        float RandomX = FMath::RandRange(-SegmentSizeX / 2, SegmentSizeX / 2); // 세그먼트 크기 내에서 랜덤 위치 생성
        float RandomY = FMath::RandRange(-SegmentSizeY / 2, SegmentSizeY / 2); // 세그먼트 크기 내에서 랜덤 위치 생성
        FVector RandomLocationXY = SegmentPosition + FVector(RandomX, RandomY, 0.0f);

        // 지형 높이 구하기
        float RandomLocationZ = GetSurfaceZAtLocation(RandomLocationXY);
        FVector RandomLocation = FVector(RandomLocationXY.X, RandomLocationXY.Y, RandomLocationZ);

        // 랜덤 나무 선택
        UStaticMesh* RandomTreeMesh = TreeMeshes[FMath::RandRange(0, TreeMeshes.Num() - 1)];
        TreeHISMComponent->SetStaticMesh(RandomTreeMesh);

        // 나무 인스턴스 추가
        FTransform TreeTransform(FRotator::ZeroRotator, RandomLocation);
        TreeHISMComponent->AddInstance(TreeTransform, true);
    }
}

float AFieldMapGenerator::GetSurfaceZAtLocation(const FVector& Location)
{
    FHitResult HitResult;
    FVector Start = Location + FVector(0, 0, 1000.0f); // 위에서 아래로 트레이스
    FVector End = Location - FVector(0, 0, 2000.0f); // 아래로 더 긴 거리의 트레이스

    // 디버그 라인 추가
    DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 5.0f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility, // ECC_WorldStatic 채널 대신 ECC_Visibility 사용
        CollisionParams
    );

    if (bHit)
    {
        // 트레이스 성공: 파란색 라인
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f);
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit at location: %s with Z=%f"), *HitResult.Location.ToString(), HitResult.Location.Z);
        return HitResult.Location.Z;
    }
    else
    {
        // 트레이스 실패: 빨간색 라인
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f);
        UE_LOG(LogTemp, Warning, TEXT("Line trace failed at location: %s"), *Location.ToString());

        // 주변 위치를 다시 트레이스
        const float SearchRadius = 1000.0f;
        const int32 NumTraces = 8;
        for (int32 i = 0; i < NumTraces; ++i)
        {
            float Angle = FMath::DegreesToRadians(360.0f / NumTraces * i);
            FVector Offset = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * SearchRadius;
            FVector NewStart = Start + Offset;
            FVector NewEnd = End + Offset;

            // 디버그 라인 추가
            DrawDebugLine(GetWorld(), NewStart, NewEnd, FColor::Yellow, false, 5.0f);

            bHit = GetWorld()->LineTraceSingleByChannel(
                HitResult,
                NewStart,
                NewEnd,
                ECC_Visibility, // ECC_WorldStatic 채널 대신 ECC_Visibility 사용
                CollisionParams
            );

            if (bHit)
            {
                DrawDebugLine(GetWorld(), NewStart, NewEnd, FColor::Green, false, 1.0f);
                UE_LOG(LogTemp, Warning, TEXT("Fallback line trace hit at location: %s with Z=%f"), *HitResult.Location.ToString(), HitResult.Location.Z);
                return HitResult.Location.Z;
            }
        }

        // 주변 위치에서도 실패할 경우 안전하게 바닥에 배치
        float DefaultZ = 0.0f;
        return DefaultZ;
    }
}

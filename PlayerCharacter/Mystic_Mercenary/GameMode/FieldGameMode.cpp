#include "FieldGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "TimerManager.h"

AFieldGameMode::AFieldGameMode()
{
    SegmentNames = {
        TEXT("Segment1"),
        TEXT("Segment2"),
        TEXT("Segment3"),
        TEXT("Segment4"),
        TEXT("Segment5"),
        TEXT("Segment6"),
        TEXT("Segment7")
    };

    SegmentSize = FVector(252.0f, 252.0f, 0.0f);
    SegmentsToLoad = 0;
    LoadedSegmentsCount = 0;
}

void AFieldGameMode::BeginPlay()
{
    Super::BeginPlay();
    GenerateFieldMap();
}

void AFieldGameMode::GenerateFieldMap()
{
    LoadRandomSegments();
}

void AFieldGameMode::LoadRandomSegments()
{
    FVector CurrentPosition = FVector::ZeroVector;

    // 세그먼트 위치를 랜덤하게 섞음
    SegmentNames.Sort([](const FName& A, const FName& B)
        {
            return FMath::RandRange(0, 1) == 0;
        });

    SegmentsToLoad = SegmentNames.Num();
    LoadedSegmentsCount = 0;

    // 세그먼트의 위치를 2차원 그리드 형태로 설정
    int32 GridSizeX = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(SegmentsToLoad)));
    int32 GridSizeY = GridSizeX;

    for (int32 i = 0; i < SegmentNames.Num(); ++i)
    {
        int32 X = i % GridSizeX;
        int32 Y = i / GridSizeX;

        FVector Position = FVector(X * SegmentSize.X, Y * SegmentSize.Y, 0.0f);

        const FName& SegmentName = SegmentNames[i];
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;
        LatentInfo.ExecutionFunction = FName("OnSegmentLoaded");
        LatentInfo.Linkage = i;
        LatentInfo.UUID = FMath::Rand(); // Unique ID

        UE_LOG(LogTemp, Warning, TEXT("Loading segment %s at position %s"), *SegmentName.ToString(), *Position.ToString());
        UGameplayStatics::LoadStreamLevel(this, SegmentName, true, true, LatentInfo);

        // Segment의 위치를 저장
        SegmentPositions.Add(SegmentName, Position);
    }
}

void AFieldGameMode::OnSegmentLoaded()
{
    UE_LOG(LogTemp, Warning, TEXT("A segment was loaded."));
    LoadedSegmentsCount++;

    if (LoadedSegmentsCount >= SegmentsToLoad)
    {
        for (const TPair<FName, FVector>& Segment : SegmentPositions)
        {
            CheckLevelLoaded(Segment.Key);
        }
    }
}

void AFieldGameMode::CheckLevelLoaded(FName SegmentName)
{
    ULevelStreaming* LoadedLevel = UGameplayStatics::GetStreamingLevel(this, SegmentName);
    if (LoadedLevel && LoadedLevel->IsLevelLoaded())
    {
        FVector* Position = SegmentPositions.Find(SegmentName);
        if (Position)
        {
            LoadedLevel->LevelTransform = FTransform(*Position);
            UE_LOG(LogTemp, Warning, TEXT("Segment %s positioned at %s"), *SegmentName.ToString(), *Position->ToString());
        }
    }
    else
    {
        FTimerDelegate RetryTimerDelegate;
        RetryTimerDelegate.BindUFunction(this, FName("CheckLevelLoaded"), SegmentName);
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, RetryTimerDelegate, 0.1f, false);
    }
}

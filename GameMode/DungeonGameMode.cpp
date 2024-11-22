#include "DungeonGameMode.h"
#include "Kismet/GameplayStatics.h"

void ADungeonGameMode::GenerateDungeonMap()
{
    // 방 리스트
    TArray<FString> DungeonRooms = { "DungeonRoom1", "DungeonRoom2", "DungeonRoom3" };

    // 초기 방 배치
    FVector StartPosition(0, 0, 0);
    FString StartRoom = DungeonRooms[FMath::RandRange(0, DungeonRooms.Num() - 1)];
    UGameplayStatics::LoadStreamLevel(this, FName(*StartRoom), true, true, FLatentActionInfo());

    // 다음 방 배치 로직
    FVector CurrentPosition = StartPosition;
    for (int32 i = 1; i < DungeonRooms.Num(); ++i)
    {
        FVector NextPosition = GetNextRoomPosition(CurrentPosition); // 다음 방 위치 계산
        FString RoomName = DungeonRooms[FMath::RandRange(0, DungeonRooms.Num() - 1)];

        UGameplayStatics::LoadStreamLevel(this, FName(*RoomName), true, true, FLatentActionInfo());
        // 방 위치 설정 로직 추가

        CurrentPosition = NextPosition;
    }
}

FVector ADungeonGameMode::GetNextRoomPosition(FVector CurrentPosition)
{
    // 현재 방 위치 기준으로 다음 방 위치를 결정하는 로직
    // 예: 동, 서, 남, 북 중 하나를 선택하여 위치 이동
    int32 Direction = FMath::RandRange(0, 3);
    switch (Direction)
    {
    case 0: return CurrentPosition + FVector(1000.0f, 0, 0); // 동쪽
    case 1: return CurrentPosition + FVector(-1000.0f, 0, 0); // 서쪽
    case 2: return CurrentPosition + FVector(0, 1000.0f, 0); // 남쪽
    case 3: return CurrentPosition + FVector(0, -1000.0f, 0); // 북쪽
    }
    return CurrentPosition;
}

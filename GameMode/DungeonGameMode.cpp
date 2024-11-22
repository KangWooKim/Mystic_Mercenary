#include "DungeonGameMode.h"
#include "Kismet/GameplayStatics.h"

void ADungeonGameMode::GenerateDungeonMap()
{
    // �� ����Ʈ
    TArray<FString> DungeonRooms = { "DungeonRoom1", "DungeonRoom2", "DungeonRoom3" };

    // �ʱ� �� ��ġ
    FVector StartPosition(0, 0, 0);
    FString StartRoom = DungeonRooms[FMath::RandRange(0, DungeonRooms.Num() - 1)];
    UGameplayStatics::LoadStreamLevel(this, FName(*StartRoom), true, true, FLatentActionInfo());

    // ���� �� ��ġ ����
    FVector CurrentPosition = StartPosition;
    for (int32 i = 1; i < DungeonRooms.Num(); ++i)
    {
        FVector NextPosition = GetNextRoomPosition(CurrentPosition); // ���� �� ��ġ ���
        FString RoomName = DungeonRooms[FMath::RandRange(0, DungeonRooms.Num() - 1)];

        UGameplayStatics::LoadStreamLevel(this, FName(*RoomName), true, true, FLatentActionInfo());
        // �� ��ġ ���� ���� �߰�

        CurrentPosition = NextPosition;
    }
}

FVector ADungeonGameMode::GetNextRoomPosition(FVector CurrentPosition)
{
    // ���� �� ��ġ �������� ���� �� ��ġ�� �����ϴ� ����
    // ��: ��, ��, ��, �� �� �ϳ��� �����Ͽ� ��ġ �̵�
    int32 Direction = FMath::RandRange(0, 3);
    switch (Direction)
    {
    case 0: return CurrentPosition + FVector(1000.0f, 0, 0); // ����
    case 1: return CurrentPosition + FVector(-1000.0f, 0, 0); // ����
    case 2: return CurrentPosition + FVector(0, 1000.0f, 0); // ����
    case 3: return CurrentPosition + FVector(0, -1000.0f, 0); // ����
    }
    return CurrentPosition;
}

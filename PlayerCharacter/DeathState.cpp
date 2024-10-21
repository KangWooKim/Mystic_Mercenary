#include "DeathState.h"
#include "GreatSwordCharacter.h"

void UDeathState::EnterState(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        Character->PlayAnimMontage(Character->DeathMontage); // 사망 애니메이션 재생
        Character->DisableInput(nullptr); // 캐릭터 조작 불가능하게 설정
    }
}

void UDeathState::UpdateState(AGreatSwordCharacter* Character, float DeltaTime)
{
    // 사망 상태에서는 특별한 업데이트 필요 없음
}
#include "UChallengeCommand.h"
#include "UTemporaryBuffManager.h"

void UChallengeCommand::Execute(AGreatSwordCharacter* Character)
{
    // 랜덤한 버프를 생성하고 적용
    if (Character)
    {
        UTemporaryBuffManager* BuffManager = Character->GetBuffManager();
        BuffManager->ApplyRandomBuff(Character);
    }
}
#include "UChallengeCommand.h"
#include "UTemporaryBuffManager.h"

void UChallengeCommand::Execute(AGreatSwordCharacter* Character)
{
    // ������ ������ �����ϰ� ����
    if (Character)
    {
        UTemporaryBuffManager* BuffManager = Character->GetBuffManager();
        BuffManager->ApplyRandomBuff(Character);
    }
}
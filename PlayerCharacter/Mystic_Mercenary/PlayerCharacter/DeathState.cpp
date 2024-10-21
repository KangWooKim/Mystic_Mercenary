#include "DeathState.h"
#include "GreatSwordCharacter.h"

void UDeathState::EnterState(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        Character->PlayAnimMontage(Character->DeathMontage); // ��� �ִϸ��̼� ���
        Character->DisableInput(nullptr); // ĳ���� ���� �Ұ����ϰ� ����
    }
}

void UDeathState::UpdateState(AGreatSwordCharacter* Character, float DeltaTime)
{
    // ��� ���¿����� Ư���� ������Ʈ �ʿ� ����
}
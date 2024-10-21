// ComboAttackStrategy.cpp

#include "ComboAttackStrategy.h"
#include "GreatSwordCharacter.h"
#include "Animation/AnimMontage.h"

ComboAttackStrategy::ComboAttackStrategy(EComboType InComboType)
    : ComboType(InComboType)
{
}

void ComboAttackStrategy::ExecuteAttack(AGreatSwordCharacter* Character)
{
    if (!Character)
    {
        return;
    }

    int32 ComboStep = Character->GetComboStep();
    UAnimMontage* MontageToPlay = Character->GetComboMontage(ComboType, ComboStep);

    if (MontageToPlay)
    {
        // ���� �ִϸ��̼� ��Ÿ�� ��� (bIsAttacking�� �ִϸ��̼� ��Ƽ���̿��� ����)
        Character->PlayAnimMontage(MontageToPlay);

        // �޺� �ܰ� ������Ʈ
        Character->IncrementComboStep();
    }
    else
    {
        // ��Ÿ�ְ� ������ ���� ����
        Character->ResetComboStep();
    }
}
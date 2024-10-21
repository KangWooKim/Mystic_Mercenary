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
        // 공격 애니메이션 몽타주 재생 (bIsAttacking은 애니메이션 노티파이에서 설정)
        Character->PlayAnimMontage(MontageToPlay);

        // 콤보 단계 업데이트
        Character->IncrementComboStep();
    }
    else
    {
        // 몽타주가 없으면 공격 종료
        Character->ResetComboStep();
    }
}
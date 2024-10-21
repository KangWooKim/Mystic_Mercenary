#include "HitState.h"
#include "GreatSwordCharacter.h"

void UHitState::EnterState(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        FVector HitDirection = Character->GetHitDirection(Character->GetVelocity().GetSafeNormal());
        float DamageAmount = 50.f; // ���÷� ����, ���� ������ �翡 ���� ���� �ʿ�

        if (DamageAmount > 50.f)
        {
            if (HitDirection.Equals(Character->GetActorForwardVector()))
            {
                Character->PlayAnimMontage(Character->StrongHitMontageFront);
            }
            else if (HitDirection.Equals(-Character->GetActorForwardVector()))
            {
                Character->PlayAnimMontage(Character->StrongHitMontageBack);
            }
            else if (HitDirection.Equals(Character->GetActorRightVector()))
            {
                Character->PlayAnimMontage(Character->StrongHitMontageRight);
            }
            else
            {
                Character->PlayAnimMontage(Character->StrongHitMontageLeft);
            }
        }
        else
        {
            if (HitDirection.Equals(Character->GetActorForwardVector()))
            {
                Character->PlayAnimMontage(Character->WeakHitMontageFront);
            }
            else if (HitDirection.Equals(-Character->GetActorForwardVector()))
            {
                Character->PlayAnimMontage(Character->WeakHitMontageBack);
            }
            else if (HitDirection.Equals(Character->GetActorRightVector()))
            {
                Character->PlayAnimMontage(Character->WeakHitMontageRight);
            }
            else
            {
                Character->PlayAnimMontage(Character->WeakHitMontageLeft);
            }
        }

        Character->SetIsHit(true);
    }
}

void UHitState::UpdateState(AGreatSwordCharacter* Character, float DeltaTime)
{
    // �ʿ信 ���� �߰� ������Ʈ ����
}

void UHitState::ExitState(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        Character->SetIsHit(false);
    }
}
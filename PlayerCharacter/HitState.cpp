#include "HitState.h"
#include "GreatSwordCharacter.h"

void UHitState::EnterState(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        FVector HitDirection = Character->GetHitDirection(Character->GetVelocity().GetSafeNormal());
        float DamageAmount = 50.f; // 예시로 설정, 실제 데미지 양에 따라 설정 필요

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
    // 필요에 따라 추가 업데이트 가능
}

void UHitState::ExitState(AGreatSwordCharacter* Character)
{
    if (Character)
    {
        Character->SetIsHit(false);
    }
}
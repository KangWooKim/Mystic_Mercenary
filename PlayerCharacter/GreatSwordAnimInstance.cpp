#include "GreatSwordAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GreatSwordCharacter.h" // 캐릭터 클래스 참조

void UGreatSwordAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    UpdateAnimationProperties();

    OwningPawn = Cast<AGreatSwordCharacter>(TryGetPawnOwner());
}

void UGreatSwordAnimInstance::UpdateAnimationProperties()
{
    if (!OwningPawn)
    {
        OwningPawn = Cast<AGreatSwordCharacter>(TryGetPawnOwner());
    }

    if (OwningPawn)
    {
        AGreatSwordCharacter* Character = Cast<AGreatSwordCharacter>(OwningPawn);
        if (Character)
        {
            // 이동 속도 업데이트
            FVector Velocity = Character->GetVelocity();
            Speed = Velocity.Size();

            // 이동 입력 벡터 가져오기
            MovementInput = Character->GetCurrentMovementInput();

            // 이동 방향 계산
            MovementDirection = CalculateDirection(Velocity, OwningPawn->GetActorRotation());

            // 공중 상태 확인
            bIsInAir = Character->GetMovementComponent()->IsFalling();

            // 점프 방향 계산 및 공중 상태 관리
            if (bIsInAir && !bWasInAir) // 점프가 시작될 때
            {
                if (MovementInput.SizeSquared() > KINDA_SMALL_NUMBER)
                {
                    // 이동 입력 벡터의 방향을 각도로 변환
                    JumpDirection = FMath::Atan2(MovementInput.Y, MovementInput.X) * (180.f / PI);
                }
                else
                {
                    // 이동 입력이 없으면 제자리 점프
                    JumpDirection = 0.f;
                }
            }
            else if (!bIsInAir)
            {
                JumpDirection = 0.f; // 지상에 있을 때는 점프 방향 초기화
            }

            // 이전 프레임의 공중 상태 업데이트
            bWasInAir = bIsInAir;

            // 캐릭터 상태 업데이트
            bIsSwordDrawn = Character->GetIsSwordDrawn();
            bIsDodging = Character->GetIsDodging();
            bIsAttacking = Character->GetIsAttacking();
            bIsBlocking = Character->GetIsBlocking();
            bIsRunning = Character->GetIsRunning();
            bIsHit = Character->GetIsHit(); // 피격 상태 업데이트
        }
    }
}

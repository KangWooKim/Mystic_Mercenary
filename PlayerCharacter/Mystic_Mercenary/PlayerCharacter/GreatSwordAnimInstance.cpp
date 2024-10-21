#include "GreatSwordAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GreatSwordCharacter.h" // ĳ���� Ŭ���� ����

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
            // �̵� �ӵ� ������Ʈ
            FVector Velocity = Character->GetVelocity();
            Speed = Velocity.Size();

            // �̵� �Է� ���� ��������
            MovementInput = Character->GetCurrentMovementInput();

            // �̵� ���� ���
            MovementDirection = CalculateDirection(Velocity, OwningPawn->GetActorRotation());

            // ���� ���� Ȯ��
            bIsInAir = Character->GetMovementComponent()->IsFalling();

            // ���� ���� ��� �� ���� ���� ����
            if (bIsInAir && !bWasInAir) // ������ ���۵� ��
            {
                if (MovementInput.SizeSquared() > KINDA_SMALL_NUMBER)
                {
                    // �̵� �Է� ������ ������ ������ ��ȯ
                    JumpDirection = FMath::Atan2(MovementInput.Y, MovementInput.X) * (180.f / PI);
                }
                else
                {
                    // �̵� �Է��� ������ ���ڸ� ����
                    JumpDirection = 0.f;
                }
            }
            else if (!bIsInAir)
            {
                JumpDirection = 0.f; // ���� ���� ���� ���� ���� �ʱ�ȭ
            }

            // ���� �������� ���� ���� ������Ʈ
            bWasInAir = bIsInAir;

            // ĳ���� ���� ������Ʈ
            bIsSwordDrawn = Character->GetIsSwordDrawn();
            bIsDodging = Character->GetIsDodging();
            bIsAttacking = Character->GetIsAttacking();
            bIsBlocking = Character->GetIsBlocking();
            bIsRunning = Character->GetIsRunning();
            bIsHit = Character->GetIsHit(); // �ǰ� ���� ������Ʈ
        }
    }
}

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GreatSwordAnimInstance.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API UGreatSwordAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // �ִϸ��̼� ������Ʈ
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // ĳ���� ���� ����
    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        bool bIsSwordDrawn;

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        bool bIsDodging;

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        bool bIsAttacking;

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        bool bIsBlocking;

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        bool bIsRunning;

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        bool bIsHit; // �ǰ� ���� ���� �߰�

    // �̵� ����
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        float MovementDirection; // �̵� ���� (-180 ~ 180)

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        bool bIsInAir;

    // ���� �������� ���� ����
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        bool bWasInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        FVector MovementInput;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        float JumpDirection;

    // �ִϸ��̼� �Ӽ� ������Ʈ
    UFUNCTION(BlueprintCallable, Category = "Update Animation Properties")
        void UpdateAnimationProperties();

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        class AGreatSwordCharacter* OwningPawn;

private:

    
};

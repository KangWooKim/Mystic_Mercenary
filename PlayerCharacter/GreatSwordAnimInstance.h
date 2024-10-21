#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GreatSwordAnimInstance.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API UGreatSwordAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // 애니메이션 업데이트
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // 캐릭터 상태 변수
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
        bool bIsHit; // 피격 상태 변수 추가

    // 이동 변수
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        float MovementDirection; // 이동 방향 (-180 ~ 180)

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        bool bIsInAir;

    // 이전 프레임의 공중 상태
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        bool bWasInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        FVector MovementInput;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
        float JumpDirection;

    // 애니메이션 속성 업데이트
    UFUNCTION(BlueprintCallable, Category = "Update Animation Properties")
        void UpdateAnimationProperties();

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
        class AGreatSwordCharacter* OwningPawn;

private:

    
};

// GreatSwordCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackStrategy.h" 
#include "Mystic_Mercenary/Weapon/Weapon.h" 
#include "GreatSwordCharacter.generated.h"

UENUM(BlueprintType)
enum class EComboType : uint8
{
    None,
    Combo1,
    Combo2,
    Combo3,
    Combo4
};

UCLASS()
class MYSTIC_MERCENARY_API AGreatSwordCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // 기본 생성자
    AGreatSwordCharacter();

protected:
    // 게임 시작 또는 스폰 시 호출
    virtual void BeginPlay() override;

    // 매 프레임마다 호출
    virtual void Tick(float DeltaTime) override;

    // 현재 장착된 무기
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
        TScriptInterface<IWeapon> CurrentWeapon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
        UStaticMeshComponent* WeaponMeshComponent;

public:
    // AnimNotify 함수들
    UFUNCTION(BlueprintCallable)
        void EnableCombo();

    UFUNCTION(BlueprintCallable)
        void DisableCombo();

    UFUNCTION(BlueprintCallable)
        void OnStartAttack();

    UFUNCTION(BlueprintCallable)
        void OnEndAttack();

    // 입력 구성
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 애니메이션 인스턴스에서 사용할 상태 가져오기
    bool GetIsSwordDrawn() const { return bIsSwordDrawn; }
    bool GetIsDodging() const { return bIsDodging; }
    bool GetIsAttacking() const { return bIsAttacking; }
    bool GetIsBlocking() const { return bIsBlocking; }
    bool GetIsRunning() const { return bIsRunning; }
    bool GetIsHit() const { return bIsHit; }

    // 현재 이동 입력 벡터 가져오기
    FVector GetCurrentMovementInput() const { return CurrentMovementInput; }

    // 점프 함수 오버라이드
    virtual void Jump() override;

    // 데미지 처리 함수
    void ReceiveDamage(float DamageAmount, FVector HitDirection, bool bIsStrongAttack);

    // 공격 전략 설정 메서드
    void SetAttackStrategy(TSharedPtr<IAttackStrategy> NewStrategy);

    // 공격 실행 메서드
    void PerformAttack();

    // 공격 상태 설정 메서드
    void SetIsAttacking(bool bAttacking) { bIsAttacking = bAttacking; }

    // 콤보 단계 접근자 메서드
    int32 GetComboStep() const { return ComboStep; }
    void IncrementComboStep();
    void ResetComboStep();

    // 콤보 몽타주 접근자 메서드
    UAnimMontage* GetComboMontage(EComboType ComboType, int32 Step) const;

    // 콤보1 몽타주 접근자 메서드
    UAnimMontage* GetCombo1Montage(int32 Step) const;

    // 콤보2 몽타주 접근자 메서드
    UAnimMontage* GetCombo2Montage(int32 Step) const;

    // 공격 입력 버퍼링 변수
    bool bAttackInputBuffered;

    UFUNCTION()
        void SetState(UCharacterState* NewState);

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* WeakHitMontageFront;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* WeakHitMontageBack;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* WeakHitMontageLeft;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* WeakHitMontageRight;

    void SetIsHit(bool bHit);

    UFUNCTION()
        FVector GetHitDirection(FVector HitDirection) const;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* StrongHitMontageFront;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* StrongHitMontageBack;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* StrongHitMontageLeft;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* StrongHitMontageRight;

    // 무기 장착
    void EquipWeapon(TScriptInterface<IWeapon> NewWeapon);

    // 현재 장착된 무기 반환
    TScriptInterface<IWeapon> GetCurrentWeapon() const { return CurrentWeapon; }

    // 버프 매니저 가져오기
    UTemporaryBuffManager* GetBuffManager() const;

    // 일시적 버프 추가
    void AddTemporaryBuff(float BuffValue);

    // 일시적 버프 제거
    void RemoveTemporaryBuff();

    // 특정 적 유형에 대한 데미지 증가
    void IncreaseDamageAgainstEnemyType(FName EnemyType, float DamageIncrease);

    // 공격 시 데미지 계산
    float CalculateDamage(AActor* EnemyActor) const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        class UHealthComponent* HealthComponent;

    // 플레이어 상태 변수
    bool bIsSwordDrawn;
    bool bIsDodging;
    bool bIsAttacking;
    bool bIsInvincible;
    bool bIsBlocking;
    bool bIsRunning;
    bool bIsHit;
    bool bCanCombo; // 콤보 입력 가능 여부

    // 체력 및 스태미나
    float Health;
    float MaxHealth;
    float Stamina;
    float MaxStamina;
    float StaminaRegenRate; // 초당 회복량

    UPROPERTY()
        class UCharacterState* CurrentState;

    UPROPERTY()
        class UCharacterState* HitState;

    UPROPERTY()
        class UCharacterState* DeathState;

    // 현재 이동 입력 벡터
    FVector CurrentMovementInput;

    // 콤보 공격 변수
    EComboType CurrentCombo;
    int32 ComboStep;
    static const int32 MaxComboSteps = 4; // 콤보 최대 단계

    // 콤보 리셋 타이머 핸들 추가
    FTimerHandle ComboResetTimerHandle;

    // 공격 전략 포인터
    TSharedPtr<IAttackStrategy> AttackStrategy;

    // 이동 메서드
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);

    // 액션 메서드
    void AttackLeft();
    void AttackRight();
    void Dodge();
    void StopDodge();
    void StartBlock();
    void StopBlock();
    void ToggleSword();
    void StartRun();
    void StopRun();

    // 스태미나 회복 메서드
    void RegenerateStamina(float DeltaTime);

    // 사망 처리
    void Die();

    // 애니메이션 몽타주
    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* Combo1Montages[MaxComboSteps];

    // 나머지 몽타주들도 동일하게 선언
    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* Combo2Montages[MaxComboSteps];

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* Combo3Montages[MaxComboSteps];

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* Combo4Montages[MaxComboSteps];

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* DodgeMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* JumpMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* StaminaEmptyHitMontage;

    // 특정 적 유형에 대한 데미지 증가량을 저장하는 맵
    TMap<FName, float> DamageModifiers;

    // 버프 매니저
    UPROPERTY()
        UTemporaryBuffManager* BuffManager;

    // 현재 적용된 일시적 버프 값
    float CurrentBuffValue;

    // 버프 지속 시간
    float BuffDuration;

    // 버프 타이머 핸들
    FTimerHandle BuffTimerHandle;

public:
    // 카메라 붐 컴포넌트 (카메라의 위치를 제어)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
        class USpringArmComponent* CameraBoom;

    // 따라다니는 카메라 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
        class UCameraComponent* FollowCamera;

    // 무기 드로우 및 언드로우 몽타주 (추가)
    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* DrawSwordMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* SheathSwordMontage;
};

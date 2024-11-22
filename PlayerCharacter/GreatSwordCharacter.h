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
    // �⺻ ������
    AGreatSwordCharacter();

protected:
    // ���� ���� �Ǵ� ���� �� ȣ��
    virtual void BeginPlay() override;

    // �� �����Ӹ��� ȣ��
    virtual void Tick(float DeltaTime) override;

    // ���� ������ ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
        TScriptInterface<IWeapon> CurrentWeapon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
        UStaticMeshComponent* WeaponMeshComponent;

public:
    // AnimNotify �Լ���
    UFUNCTION(BlueprintCallable)
        void EnableCombo();

    UFUNCTION(BlueprintCallable)
        void DisableCombo();

    UFUNCTION(BlueprintCallable)
        void OnStartAttack();

    UFUNCTION(BlueprintCallable)
        void OnEndAttack();

    // �Է� ����
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // �ִϸ��̼� �ν��Ͻ����� ����� ���� ��������
    bool GetIsSwordDrawn() const { return bIsSwordDrawn; }
    bool GetIsDodging() const { return bIsDodging; }
    bool GetIsAttacking() const { return bIsAttacking; }
    bool GetIsBlocking() const { return bIsBlocking; }
    bool GetIsRunning() const { return bIsRunning; }
    bool GetIsHit() const { return bIsHit; }

    // ���� �̵� �Է� ���� ��������
    FVector GetCurrentMovementInput() const { return CurrentMovementInput; }

    // ���� �Լ� �������̵�
    virtual void Jump() override;

    // ������ ó�� �Լ�
    void ReceiveDamage(float DamageAmount, FVector HitDirection, bool bIsStrongAttack);

    // ���� ���� ���� �޼���
    void SetAttackStrategy(TSharedPtr<IAttackStrategy> NewStrategy);

    // ���� ���� �޼���
    void PerformAttack();

    // ���� ���� ���� �޼���
    void SetIsAttacking(bool bAttacking) { bIsAttacking = bAttacking; }

    // �޺� �ܰ� ������ �޼���
    int32 GetComboStep() const { return ComboStep; }
    void IncrementComboStep();
    void ResetComboStep();

    // �޺� ��Ÿ�� ������ �޼���
    UAnimMontage* GetComboMontage(EComboType ComboType, int32 Step) const;

    // �޺�1 ��Ÿ�� ������ �޼���
    UAnimMontage* GetCombo1Montage(int32 Step) const;

    // �޺�2 ��Ÿ�� ������ �޼���
    UAnimMontage* GetCombo2Montage(int32 Step) const;

    // ���� �Է� ���۸� ����
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

    // ���� ����
    void EquipWeapon(TScriptInterface<IWeapon> NewWeapon);

    // ���� ������ ���� ��ȯ
    TScriptInterface<IWeapon> GetCurrentWeapon() const { return CurrentWeapon; }

    // ���� �Ŵ��� ��������
    UTemporaryBuffManager* GetBuffManager() const;

    // �Ͻ��� ���� �߰�
    void AddTemporaryBuff(float BuffValue);

    // �Ͻ��� ���� ����
    void RemoveTemporaryBuff();

    // Ư�� �� ������ ���� ������ ����
    void IncreaseDamageAgainstEnemyType(FName EnemyType, float DamageIncrease);

    // ���� �� ������ ���
    float CalculateDamage(AActor* EnemyActor) const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        class UHealthComponent* HealthComponent;

    // �÷��̾� ���� ����
    bool bIsSwordDrawn;
    bool bIsDodging;
    bool bIsAttacking;
    bool bIsInvincible;
    bool bIsBlocking;
    bool bIsRunning;
    bool bIsHit;
    bool bCanCombo; // �޺� �Է� ���� ����

    // ü�� �� ���¹̳�
    float Health;
    float MaxHealth;
    float Stamina;
    float MaxStamina;
    float StaminaRegenRate; // �ʴ� ȸ����

    UPROPERTY()
        class UCharacterState* CurrentState;

    UPROPERTY()
        class UCharacterState* HitState;

    UPROPERTY()
        class UCharacterState* DeathState;

    // ���� �̵� �Է� ����
    FVector CurrentMovementInput;

    // �޺� ���� ����
    EComboType CurrentCombo;
    int32 ComboStep;
    static const int32 MaxComboSteps = 4; // �޺� �ִ� �ܰ�

    // �޺� ���� Ÿ�̸� �ڵ� �߰�
    FTimerHandle ComboResetTimerHandle;

    // ���� ���� ������
    TSharedPtr<IAttackStrategy> AttackStrategy;

    // �̵� �޼���
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);

    // �׼� �޼���
    void AttackLeft();
    void AttackRight();
    void Dodge();
    void StopDodge();
    void StartBlock();
    void StopBlock();
    void ToggleSword();
    void StartRun();
    void StopRun();

    // ���¹̳� ȸ�� �޼���
    void RegenerateStamina(float DeltaTime);

    // ��� ó��
    void Die();

    // �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* Combo1Montages[MaxComboSteps];

    // ������ ��Ÿ�ֵ鵵 �����ϰ� ����
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

    // Ư�� �� ������ ���� ������ �������� �����ϴ� ��
    TMap<FName, float> DamageModifiers;

    // ���� �Ŵ���
    UPROPERTY()
        UTemporaryBuffManager* BuffManager;

    // ���� ����� �Ͻ��� ���� ��
    float CurrentBuffValue;

    // ���� ���� �ð�
    float BuffDuration;

    // ���� Ÿ�̸� �ڵ�
    FTimerHandle BuffTimerHandle;

public:
    // ī�޶� �� ������Ʈ (ī�޶��� ��ġ�� ����)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
        class USpringArmComponent* CameraBoom;

    // ����ٴϴ� ī�޶� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
        class UCameraComponent* FollowCamera;

    // ���� ��ο� �� ���ο� ��Ÿ�� (�߰�)
    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* DrawSwordMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
        UAnimMontage* SheathSwordMontage;
};

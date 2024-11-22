// GreatSwordCharacter.cpp

#include "GreatSwordCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "CharacterState.h"
#include "HitState.h"
#include "DeathState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ComboAttackStrategy.h"
#include "HealthComponent.h"
#include "Mystic_Mercenary/Weapon/WeaponFactory.h"
#include "Mystic_Mercenary/Weapon/Weapon.h"
#include "Mystic_Mercenary/GrowthSystem/UTemporaryBuffManager.h"
#include "Net/UnrealNetwork.h"

const int32 AGreatSwordCharacter::MaxComboSteps;

AGreatSwordCharacter::AGreatSwordCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // ���� �޽� ������Ʈ ���� �� ���� 
    WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMeshComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket")); // ���� �̸� ��ġ �ʿ�
    WeaponMeshComponent->bCastDynamicShadow = true;
    WeaponMeshComponent->CastShadow = true;
    WeaponMeshComponent->SetRelativeLocation(FVector::ZeroVector);
    WeaponMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
    WeaponMeshComponent->SetVisibility(false); // �⺻������ ����

    // ���� ���� �ʱ�ȭ
    bIsSwordDrawn = false;
    bIsDodging = false;
    bIsAttacking = false;
    bIsInvincible = false;
    bIsBlocking = false;
    bIsRunning = false;
    bIsHit = false;
    bAttackInputBuffered = false;
    bCanCombo = false;

    CurrentCombo = EComboType::None;
    ComboStep = 0;

    // ���� �ν��Ͻ� ����
    HitState = NewObject<UHitState>();
    DeathState = NewObject<UDeathState>();

    CurrentMovementInput = FVector::ZeroVector;

    // ü�� �� ���¹̳� �ʱ�ȭ
    MaxHealth = 300.f;
    Health = MaxHealth;
    MaxStamina = 300.f;
    Stamina = MaxStamina;
    StaminaRegenRate = 10.f;

    // ī�޶� �� ���� �� ����
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent); // ĳ������ ��Ʈ ������Ʈ�� ����
    CameraBoom->TargetArmLength = 300.0f;       // ī�޶�� ĳ���� ������ �Ÿ�
    CameraBoom->bUsePawnControlRotation = true; // ��Ʈ�ѷ��� ȸ���� ���� ���� ȸ��

    // ī�޶� ������Ʈ ���� �� ����
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ī�޶� ���� ���Ͽ� ����
    FollowCamera->bUsePawnControlRotation = false; // ī�޶� ��ü�� ��Ʈ�ѷ��� ȸ���� ���� ȸ������ ����

    // ĳ���Ͱ� ��Ʈ�ѷ��� ȸ���� ���� ȸ������ �ʵ��� ����
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // ĳ���� ������ ����
    GetCharacterMovement()->bOrientRotationToMovement = true; // �����̴� �������� ĳ���� ȸ��
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ȸ�� �ӵ� ����
    GetCharacterMovement()->JumpZVelocity = 600.f; // ���� �ӵ� ����
    GetCharacterMovement()->AirControl = 0.2f;    // ���� ����� ����

    // �ʱ� �ȱ� �ӵ� ����
    GetCharacterMovement()->MaxWalkSpeed = 200.f;

    // ���ø����̼� ����
    bReplicates = true;

    CurrentBuffValue = 0.0f;
    BuffDuration = 10.0f; // ���� ���� �ð� ����

    // ���� �Ŵ��� ����
    BuffManager = CreateDefaultSubobject<UTemporaryBuffManager>(TEXT("BuffManager"));
}

void AGreatSwordCharacter::BeginPlay()
{
    Super::BeginPlay();

    CurrentState = nullptr; // �⺻ ���´� �������� ����

    // HealthComponent�� OnDeath ��������Ʈ�� Die �Լ� ���ε�
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AGreatSwordCharacter::Die);
    }

    // ���� ���� �� ����
    TScriptInterface<IWeapon> GreatSword = UWeaponFactory::CreateWeapon(TEXT("GreatSword"));
    EquipWeapon(GreatSword);

    // ���� �Ŵ��� �ʱ�ȭ
    if (BuffManager)
    {
        BuffManager->Initialize(this);
    }
}

void AGreatSwordCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���¹̳� ȸ��
    RegenerateStamina(DeltaTime);

    if (CurrentState)
    {
        CurrentState->UpdateState(this, DeltaTime);
    }
}

void AGreatSwordCharacter::RegenerateStamina(float DeltaTime)
{
    if (Stamina < MaxStamina)
    {
        Stamina += StaminaRegenRate * DeltaTime;
        if (Stamina > MaxStamina)
        {
            Stamina = MaxStamina;
        }
    }
}

void AGreatSwordCharacter::ReceiveDamage(float DamageAmount, FVector HitDirection, bool bIsStrongAttack)
{
    if (bIsBlocking)
    {
        // ���� ���¿��� ���� �ǰ� ó��
        FVector ForwardVector = GetActorForwardVector();
        float DotProduct = FVector::DotProduct(ForwardVector, HitDirection);
        if (DotProduct > 0.7f) // ����
        {
            Stamina -= DamageAmount; // ���¹̳� �Ҹ�
            if (Stamina <= 0.f)
            {
                StopBlock(); // ���¹̳��� �ٴڳ��� ���� ����
                PlayAnimMontage(StaminaEmptyHitMontage);
            }
            else
            {
                // ���¹̳��� �����ִٸ� ���� ����
                return;
            }
        }
        else
        {
            // �ĸ� ���� �� ���� �Ұ�
            StopBlock();
        }
    }

    if (HealthComponent)
    {
        HealthComponent->TakeDamage(DamageAmount);
    }

    if (HealthComponent->GetHealth() <= 0.f)
    {
        SetState(DeathState);
    }
    else
    {
        SetState(HitState);
    }
}

void AGreatSwordCharacter::Die()
{
    Destroy();
}

void AGreatSwordCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // �Է� ���ε�
    PlayerInputComponent->BindAxis("MoveForward", this, &AGreatSwordCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AGreatSwordCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AGreatSwordCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &AGreatSwordCharacter::LookUp);
    PlayerInputComponent->BindAction("ToggleSword", IE_Pressed, this, &AGreatSwordCharacter::ToggleSword);
    PlayerInputComponent->BindAction("AttackLeft", IE_Pressed, this, &AGreatSwordCharacter::AttackLeft);
    PlayerInputComponent->BindAction("AttackRight", IE_Pressed, this, &AGreatSwordCharacter::AttackRight);
    PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AGreatSwordCharacter::Dodge);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGreatSwordCharacter::Jump);
    PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AGreatSwordCharacter::StartBlock);
    PlayerInputComponent->BindAction("Block", IE_Released, this, &AGreatSwordCharacter::StopBlock);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AGreatSwordCharacter::StartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AGreatSwordCharacter::StopRun);
}

void AGreatSwordCharacter::MoveForward(float Value)
{
    if (bIsAttacking)
    {
        CurrentMovementInput.X = 0.f;
        return;
    }

    if (Value != 0.0f)
    {
        // ��Ʈ�ѷ��� ������ �������� ���� ���� ���
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // ���� ���� ����
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);

        CurrentMovementInput.X = Value;
    }
    else
    {
        CurrentMovementInput.X = 0.f;
    }
}

void AGreatSwordCharacter::MoveRight(float Value)
{
    if (bIsAttacking)
    {
        CurrentMovementInput.Y = 0.f;
        return;
    }

    if (Value != 0.0f)
    {
        // ��Ʈ�ѷ��� ������ �������� ������ ���� ���
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // ������ ���� ����
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);

        CurrentMovementInput.Y = Value;
    }
    else
    {
        CurrentMovementInput.Y = 0.f;
    }
}

void AGreatSwordCharacter::Turn(float Value)
{
    if (bIsAttacking)
    {
        return;
    }
    AddControllerYawInput(Value);
}

void AGreatSwordCharacter::LookUp(float Value)
{
    if (bIsAttacking)
    {
        return;
    }
    AddControllerPitchInput(Value);
}

void AGreatSwordCharacter::ToggleSword()
{
    if (bIsAttacking)
    {
        return;
    }
    bIsSwordDrawn = !bIsSwordDrawn;

    if (bIsSwordDrawn)
    {
        // �ҵ� ��ο� �ִϸ��̼� ��� (�ʿ� ��)
        PlayAnimMontage(DrawSwordMontage);
    }
    else
    {
        // �ҵ� ���ο� �ִϸ��̼� ��� (�ʿ� ��)
        PlayAnimMontage(SheathSwordMontage);
    }
}

void AGreatSwordCharacter::AttackLeft()
{
    if (bIsSwordDrawn)
    {
        if (bIsAttacking)
        {
            if (bCanCombo)
            {
                bAttackInputBuffered = true;
            }
        }
        else
        {
            if (CurrentWeapon)
            {
                // ������ ���� ���� ����
                CurrentWeapon->SetAttackStrategy(MakeShareable(new ComboAttackStrategy(EComboType::Combo1)));
            }
            // PerformAttack ȣ��
            PerformAttack();
        }
    }
}

void AGreatSwordCharacter::AttackRight()
{
    if (bIsSwordDrawn)
    {
        if (bIsAttacking)
        {
            if (bCanCombo)
            {
                bAttackInputBuffered = true;
            }
        }
        else
        {
            if (CurrentWeapon)
            {
                // ������ ���� ���� ����
                CurrentWeapon->SetAttackStrategy(MakeShareable(new ComboAttackStrategy(EComboType::Combo2)));
            }
            // PerformAttack ȣ��
            PerformAttack();
        }
    }
}

void AGreatSwordCharacter::SetAttackStrategy(TSharedPtr<IAttackStrategy> NewStrategy)
{
    AttackStrategy = NewStrategy;
}

void AGreatSwordCharacter::PerformAttack()
{
    if (CurrentWeapon && CurrentWeapon.GetInterface())
    {
        CurrentWeapon->Attack(this);
    }
    else
    {
        // �⺻ ���� ó�� (�ʿ� ��)
        UE_LOG(LogTemp, Warning, TEXT("No weapon equipped or weapon interface invalid."));
    }
}

void AGreatSwordCharacter::IncrementComboStep()
{
    ComboStep++;
    if (ComboStep >= MaxComboSteps)
    {
        ComboStep = 0;
    }
}

void AGreatSwordCharacter::ResetComboStep()
{
    ComboStep = 0;
    bIsAttacking = false;
    bAttackInputBuffered = false;
    bCanCombo = false;
}

UAnimMontage* AGreatSwordCharacter::GetComboMontage(EComboType ComboType, int32 Step) const
{
    switch (ComboType)
    {
    case EComboType::Combo1:
        return GetCombo1Montage(Step);
    case EComboType::Combo2:
        return GetCombo2Montage(Step);
    case EComboType::Combo3:
        // GetCombo3Montage �Լ� ���� �ʿ�
        return nullptr;
    case EComboType::Combo4:
        // GetCombo4Montage �Լ� ���� �ʿ�
        return nullptr;
    default:
        return nullptr;
    }
}

UAnimMontage* AGreatSwordCharacter::GetCombo1Montage(int32 Step) const
{
    if (Step >= 0 && Step < MaxComboSteps)
    {
        return Combo1Montages[Step];
    }
    return nullptr;
}

UAnimMontage* AGreatSwordCharacter::GetCombo2Montage(int32 Step) const
{
    if (Step >= 0 && Step < MaxComboSteps)
    {
        return Combo2Montages[Step];
    }
    return nullptr;
}

void AGreatSwordCharacter::EnableCombo()
{
    bCanCombo = true;
}

void AGreatSwordCharacter::DisableCombo()
{
    bCanCombo = false;
}

void AGreatSwordCharacter::OnStartAttack()
{
    bIsAttacking = true;
}

void AGreatSwordCharacter::OnEndAttack()
{
    bIsAttacking = false;

    if (bAttackInputBuffered)
    {
        bAttackInputBuffered = false;
        PerformAttack();
    }
    else
    {
        ResetComboStep();
    }
}

void AGreatSwordCharacter::Dodge()
{
    if (!bIsDodging && !bIsAttacking && Stamina >= 50.f)
    {
        bIsDodging = true;
        bIsInvincible = true; // ȸ�� �� ����
        PlayAnimMontage(DodgeMontage);

        // ���¹̳� �Ҹ�
        Stamina -= 50.f;
        if (Stamina < 0.f)
        {
            Stamina = 0.f;
        }

        // ���� ���� �ð� ����
        GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &AGreatSwordCharacter::StopDodge, DodgeMontage->GetPlayLength(), false);
    }
}

void AGreatSwordCharacter::StopDodge()
{
    bIsDodging = false;
    bIsInvincible = false;
}

void AGreatSwordCharacter::Jump()
{
    if (!bIsDodging && !bIsAttacking)
    {
        Super::Jump();
    }
}

void AGreatSwordCharacter::StartRun()
{
    if (bIsAttacking)
    {
        return;
    }

    bIsRunning = true;
    GetCharacterMovement()->MaxWalkSpeed = 600.f; // �޸��� �ӵ��� ����
}

void AGreatSwordCharacter::StopRun()
{
    bIsRunning = false;
    GetCharacterMovement()->MaxWalkSpeed = 200.f; // �ȱ� �ӵ��� ����
}

void AGreatSwordCharacter::StartBlock()
{
    if (!bIsBlocking && !bIsAttacking && !bIsDodging && !bIsRunning && !bIsHit && !GetCharacterMovement()->IsFalling() && Stamina > 0.f)
    {
        bIsBlocking = true;
        bIsInvincible = true;
    }
}

void AGreatSwordCharacter::StopBlock()
{
    if (bIsBlocking)
    {
        bIsBlocking = false;
        bIsInvincible = false;
    }
}

void AGreatSwordCharacter::SetState(UCharacterState* NewState)
{
    if (CurrentState)
    {
        CurrentState->ExitState(this);
    }
    CurrentState = NewState;
    if (CurrentState)
    {
        CurrentState->EnterState(this);
    }
}

void AGreatSwordCharacter::SetIsHit(bool bHit)
{
    bIsHit = bHit;
}

FVector AGreatSwordCharacter::GetHitDirection(FVector HitDirection) const
{
    FVector ForwardVector = GetActorForwardVector();
    FVector RightVector = GetActorRightVector();

    float ForwardDot = FVector::DotProduct(ForwardVector, HitDirection);
    float RightDot = FVector::DotProduct(RightVector, HitDirection);

    if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
    {
        return (ForwardDot > 0) ? ForwardVector : -ForwardVector;
    }
    else
    {
        return (RightDot > 0) ? RightVector : -RightVector;
    }
}

void AGreatSwordCharacter::EquipWeapon(TScriptInterface<IWeapon> NewWeapon)
{
    if (NewWeapon)
    {
        CurrentWeapon = NewWeapon;

        // ���丮���� ������ ���� �޽� ��������
        UStaticMesh* WeaponMesh = CurrentWeapon->GetWeaponMesh();

        if (WeaponMeshComponent && WeaponMesh)
        {
            WeaponMeshComponent->SetStaticMesh(WeaponMesh);
            WeaponMeshComponent->SetVisibility(true);
            WeaponMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
        }

        // ���� ���濡 ���� �ִϸ��̼� �Ǵ� ���� ������Ʈ ����
        // ��: ���⸦ �ֵθ��� �ִϸ��̼�, �̵� �ӵ� ���� ��
    }
    else
    {
        // ���� ���� �� ó��
        if (WeaponMeshComponent)
        {
            WeaponMeshComponent->SetVisibility(false);
            WeaponMeshComponent->SetStaticMesh(nullptr);
        }

        CurrentWeapon = nullptr;
    }
}

// ���� �Ŵ��� ��������
UTemporaryBuffManager* AGreatSwordCharacter::GetBuffManager() const
{
    return BuffManager;
}

// �Ͻ��� ���� �߰�
void AGreatSwordCharacter::AddTemporaryBuff(float BuffValue)
{
    CurrentBuffValue += BuffValue;

    // ���� Ÿ�̸� ����
    GetWorldTimerManager().SetTimer(BuffTimerHandle, this, &AGreatSwordCharacter::RemoveTemporaryBuff, BuffDuration, false);
}

// �Ͻ��� ���� ����
void AGreatSwordCharacter::RemoveTemporaryBuff()
{
    // ���� ȿ�� ����
    CurrentBuffValue = 0.0f;
    // Ÿ�̸� �ʱ�ȭ
    GetWorldTimerManager().ClearTimer(BuffTimerHandle);
}

// Ư�� �� ������ ���� ������ ����
void AGreatSwordCharacter::IncreaseDamageAgainstEnemyType(FName EnemyType, float DamageIncrease)
{
    if (DamageModifiers.Contains(EnemyType))
    {
        DamageModifiers[EnemyType] += DamageIncrease;
    }
    else
    {
        DamageModifiers.Add(EnemyType, DamageIncrease);
    }
}

// ���� �� ������ ���
float AGreatSwordCharacter::CalculateDamage(AActor* EnemyActor) const
{
    float BaseDamage = CurrentWeapon ? CurrentWeapon->GetBaseDamage() : 10.0f;
    float TotalDamage = BaseDamage + CurrentBuffValue;

    // �� ���� ��������
    IEnemyInterface* Enemy = Cast<IEnemyInterface>(EnemyActor);
    if (Enemy)
    {
        FName EnemyType = Enemy->GetEnemyType();

        // �ش� �� ������ ���� ������ ���ʽ� ����
        if (DamageModifiers.Contains(EnemyType))
        {
            TotalDamage += DamageModifiers[EnemyType];
        }
    }

    return TotalDamage;
}
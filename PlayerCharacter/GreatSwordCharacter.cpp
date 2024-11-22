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

    // 무기 메쉬 컴포넌트 생성 및 설정 
    WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMeshComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket")); // 소켓 이름 일치 필요
    WeaponMeshComponent->bCastDynamicShadow = true;
    WeaponMeshComponent->CastShadow = true;
    WeaponMeshComponent->SetRelativeLocation(FVector::ZeroVector);
    WeaponMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
    WeaponMeshComponent->SetVisibility(false); // 기본적으로 숨김

    // 기존 변수 초기화
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

    // 상태 인스턴스 생성
    HitState = NewObject<UHitState>();
    DeathState = NewObject<UDeathState>();

    CurrentMovementInput = FVector::ZeroVector;

    // 체력 및 스태미나 초기화
    MaxHealth = 300.f;
    Health = MaxHealth;
    MaxStamina = 300.f;
    Stamina = MaxStamina;
    StaminaRegenRate = 10.f;

    // 카메라 붐 생성 및 설정
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent); // 캐릭터의 루트 컴포넌트에 부착
    CameraBoom->TargetArmLength = 300.0f;       // 카메라와 캐릭터 사이의 거리
    CameraBoom->bUsePawnControlRotation = true; // 컨트롤러의 회전에 따라 붐이 회전

    // 카메라 컴포넌트 생성 및 설정
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 카메라 붐의 소켓에 부착
    FollowCamera->bUsePawnControlRotation = false; // 카메라 자체는 컨트롤러의 회전에 따라 회전하지 않음

    // 캐릭터가 컨트롤러의 회전에 따라 회전하지 않도록 설정
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // 캐릭터 움직임 설정
    GetCharacterMovement()->bOrientRotationToMovement = true; // 움직이는 방향으로 캐릭터 회전
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 회전 속도 설정
    GetCharacterMovement()->JumpZVelocity = 600.f; // 점프 속도 설정
    GetCharacterMovement()->AirControl = 0.2f;    // 공중 제어력 설정

    // 초기 걷기 속도 설정
    GetCharacterMovement()->MaxWalkSpeed = 200.f;

    // 레플리케이션 설정
    bReplicates = true;

    CurrentBuffValue = 0.0f;
    BuffDuration = 10.0f; // 버프 지속 시간 설정

    // 버프 매니저 생성
    BuffManager = CreateDefaultSubobject<UTemporaryBuffManager>(TEXT("BuffManager"));
}

void AGreatSwordCharacter::BeginPlay()
{
    Super::BeginPlay();

    CurrentState = nullptr; // 기본 상태는 지정하지 않음

    // HealthComponent의 OnDeath 델리게이트에 Die 함수 바인딩
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AGreatSwordCharacter::Die);
    }

    // 무기 생성 및 장착
    TScriptInterface<IWeapon> GreatSword = UWeaponFactory::CreateWeapon(TEXT("GreatSword"));
    EquipWeapon(GreatSword);

    // 버프 매니저 초기화
    if (BuffManager)
    {
        BuffManager->Initialize(this);
    }
}

void AGreatSwordCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 스태미나 회복
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
        // 가드 상태에서 정면 피격 처리
        FVector ForwardVector = GetActorForwardVector();
        float DotProduct = FVector::DotProduct(ForwardVector, HitDirection);
        if (DotProduct > 0.7f) // 정면
        {
            Stamina -= DamageAmount; // 스태미나 소모
            if (Stamina <= 0.f)
            {
                StopBlock(); // 스태미나가 바닥나면 가드 해제
                PlayAnimMontage(StaminaEmptyHitMontage);
            }
            else
            {
                // 스태미나가 남아있다면 가드 유지
                return;
            }
        }
        else
        {
            // 후면 공격 시 가드 불가
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

    // 입력 바인딩
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
        // 컨트롤러의 방향을 기준으로 전진 벡터 계산
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 전진 방향 벡터
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
        // 컨트롤러의 방향을 기준으로 오른쪽 벡터 계산
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 오른쪽 방향 벡터
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
        // 소드 드로우 애니메이션 재생 (필요 시)
        PlayAnimMontage(DrawSwordMontage);
    }
    else
    {
        // 소드 언드로우 애니메이션 재생 (필요 시)
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
                // 무기의 공격 전략 설정
                CurrentWeapon->SetAttackStrategy(MakeShareable(new ComboAttackStrategy(EComboType::Combo1)));
            }
            // PerformAttack 호출
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
                // 무기의 공격 전략 설정
                CurrentWeapon->SetAttackStrategy(MakeShareable(new ComboAttackStrategy(EComboType::Combo2)));
            }
            // PerformAttack 호출
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
        // 기본 공격 처리 (필요 시)
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
        // GetCombo3Montage 함수 구현 필요
        return nullptr;
    case EComboType::Combo4:
        // GetCombo4Montage 함수 구현 필요
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
        bIsInvincible = true; // 회피 중 무적
        PlayAnimMontage(DodgeMontage);

        // 스태미나 소모
        Stamina -= 50.f;
        if (Stamina < 0.f)
        {
            Stamina = 0.f;
        }

        // 무적 지속 시간 설정
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
    GetCharacterMovement()->MaxWalkSpeed = 600.f; // 달리기 속도로 변경
}

void AGreatSwordCharacter::StopRun()
{
    bIsRunning = false;
    GetCharacterMovement()->MaxWalkSpeed = 200.f; // 걷기 속도로 변경
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

        // 팩토리에서 생성된 무기 메쉬 가져오기
        UStaticMesh* WeaponMesh = CurrentWeapon->GetWeaponMesh();

        if (WeaponMeshComponent && WeaponMesh)
        {
            WeaponMeshComponent->SetStaticMesh(WeaponMesh);
            WeaponMeshComponent->SetVisibility(true);
            WeaponMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
        }

        // 무기 변경에 따른 애니메이션 또는 상태 업데이트 가능
        // 예: 무기를 휘두르는 애니메이션, 이동 속도 변경 등
    }
    else
    {
        // 무기 해제 시 처리
        if (WeaponMeshComponent)
        {
            WeaponMeshComponent->SetVisibility(false);
            WeaponMeshComponent->SetStaticMesh(nullptr);
        }

        CurrentWeapon = nullptr;
    }
}

// 버프 매니저 가져오기
UTemporaryBuffManager* AGreatSwordCharacter::GetBuffManager() const
{
    return BuffManager;
}

// 일시적 버프 추가
void AGreatSwordCharacter::AddTemporaryBuff(float BuffValue)
{
    CurrentBuffValue += BuffValue;

    // 버프 타이머 시작
    GetWorldTimerManager().SetTimer(BuffTimerHandle, this, &AGreatSwordCharacter::RemoveTemporaryBuff, BuffDuration, false);
}

// 일시적 버프 제거
void AGreatSwordCharacter::RemoveTemporaryBuff()
{
    // 버프 효과 제거
    CurrentBuffValue = 0.0f;
    // 타이머 초기화
    GetWorldTimerManager().ClearTimer(BuffTimerHandle);
}

// 특정 적 유형에 대한 데미지 증가
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

// 공격 시 데미지 계산
float AGreatSwordCharacter::CalculateDamage(AActor* EnemyActor) const
{
    float BaseDamage = CurrentWeapon ? CurrentWeapon->GetBaseDamage() : 10.0f;
    float TotalDamage = BaseDamage + CurrentBuffValue;

    // 적 유형 가져오기
    IEnemyInterface* Enemy = Cast<IEnemyInterface>(EnemyActor);
    if (Enemy)
    {
        FName EnemyType = Enemy->GetEnemyType();

        // 해당 적 유형에 대한 데미지 보너스 적용
        if (DamageModifiers.Contains(EnemyType))
        {
            TotalDamage += DamageModifiers[EnemyType];
        }
    }

    return TotalDamage;
}
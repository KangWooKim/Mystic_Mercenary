// Weapon.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Weapon.generated.h"

UINTERFACE(MinimalAPI)
class UWeapon : public UInterface
{
    GENERATED_BODY()
};

/**
 * IWeapon 인터페이스 정의
 */
class MYSTIC_MERCENARY_API IWeapon : public IInterface
{
    GENERATED_BODY()

public:
    // 공격 실행
    virtual void Attack(class AGreatSwordCharacter* Character) = 0;

    // 무기 이름 반환
    virtual FString GetWeaponName() const = 0;

    // 무기 데미지 반환
    virtual float GetDamage() const = 0;

    // 공격 전략 설정
    virtual void SetAttackStrategy(TSharedPtr<class IAttackStrategy> NewStrategy) = 0;

    // 무기 메쉬 반환
    virtual UStaticMesh* GetWeaponMesh() const = 0;
};

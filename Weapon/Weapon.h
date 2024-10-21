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
 * IWeapon �������̽� ����
 */
class MYSTIC_MERCENARY_API IWeapon : public IInterface
{
    GENERATED_BODY()

public:
    // ���� ����
    virtual void Attack(class AGreatSwordCharacter* Character) = 0;

    // ���� �̸� ��ȯ
    virtual FString GetWeaponName() const = 0;

    // ���� ������ ��ȯ
    virtual float GetDamage() const = 0;

    // ���� ���� ����
    virtual void SetAttackStrategy(TSharedPtr<class IAttackStrategy> NewStrategy) = 0;

    // ���� �޽� ��ȯ
    virtual UStaticMesh* GetWeaponMesh() const = 0;
};

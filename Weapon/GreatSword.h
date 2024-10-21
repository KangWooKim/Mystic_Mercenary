// GreatSword.h
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GreatSword.generated.h"

UCLASS(Blueprintable)
class MYSTIC_MERCENARY_API UGreatSword : public UObject, public IWeapon
{
    GENERATED_BODY()

public:
    UGreatSword();

    // IWeapon �������̽� ����
    virtual void Attack(AGreatSwordCharacter* Character) override;
    virtual FString GetWeaponName() const override { return TEXT("Great Sword"); }
    virtual float GetDamage() const override { return Damage; }

    virtual void SetAttackStrategy(TSharedPtr<IAttackStrategy> NewStrategy) override
    {
        AttackStrategy = NewStrategy;
    }

    // IWeapon �������̽��� GetWeaponMesh ����
    virtual UStaticMesh* GetWeaponMesh() const override { return WeaponMesh; }

    // �߰�: Damage �� WeaponMesh ���� �޼���
    void SetDamage(float NewDamage) { Damage = NewDamage; }
    void SetWeaponMesh(UStaticMesh* NewMesh) { WeaponMesh = NewMesh; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
        float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
        UStaticMesh* WeaponMesh;

private:
    TSharedPtr<IAttackStrategy> AttackStrategy;
};

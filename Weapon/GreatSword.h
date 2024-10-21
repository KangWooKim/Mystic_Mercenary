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

    // IWeapon 인터페이스 구현
    virtual void Attack(AGreatSwordCharacter* Character) override;
    virtual FString GetWeaponName() const override { return TEXT("Great Sword"); }
    virtual float GetDamage() const override { return Damage; }

    virtual void SetAttackStrategy(TSharedPtr<IAttackStrategy> NewStrategy) override
    {
        AttackStrategy = NewStrategy;
    }

    // IWeapon 인터페이스의 GetWeaponMesh 구현
    virtual UStaticMesh* GetWeaponMesh() const override { return WeaponMesh; }

    // 추가: Damage 및 WeaponMesh 설정 메서드
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

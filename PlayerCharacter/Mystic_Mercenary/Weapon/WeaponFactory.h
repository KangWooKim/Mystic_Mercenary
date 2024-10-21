// WeaponFactory.h
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GreatSword.h"
#include "WeaponFactory.generated.h"

UCLASS()
class MYSTIC_MERCENARY_API UWeaponFactory : public UObject
{
    GENERATED_BODY()

public:
    // ���� Ÿ���� ������� ���� ����
    UFUNCTION(BlueprintCallable, Category = "Weapon")
        static TScriptInterface<IWeapon> CreateWeapon(FName WeaponType);
};

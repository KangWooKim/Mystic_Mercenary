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
    // 무기 타입을 기반으로 무기 생성
    UFUNCTION(BlueprintCallable, Category = "Weapon")
        static TScriptInterface<IWeapon> CreateWeapon(FName WeaponType);
};

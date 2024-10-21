// WeaponFactory.cpp
#include "WeaponFactory.h"
#include "GreatSword.h"
// #include "Dagger.h" // 다른 무기 클래스 포함 (예시)

TScriptInterface<IWeapon> UWeaponFactory::CreateWeapon(FName WeaponType)
{
    if (WeaponType == TEXT("GreatSword"))
    {
        UGreatSword* NewWeapon = NewObject<UGreatSword>();
        if (NewWeapon)
        {
            // 무기 메쉬는 에디터를 통해 설정됩니다.
            return TScriptInterface<IWeapon>(NewWeapon);
        }
    }
    // else if (WeaponType == TEXT("Dagger"))
    // {
    //     UDagger* NewWeapon = NewObject<UDagger>();
    //     if (NewWeapon)
    //     {
    //         return TScriptInterface<IWeapon>(NewWeapon);
    //     }
    // }
    // 추가 무기 타입 처리
    return nullptr;
}

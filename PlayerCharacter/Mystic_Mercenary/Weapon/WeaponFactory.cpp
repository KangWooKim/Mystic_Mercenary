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
            // 무기 메쉬 설정 (예시)
            UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Demo/Mannequin_UE4/Character/Mesh/Great_Sword"));
            if (StaticMesh)
            {
                NewWeapon->SetWeaponMesh(StaticMesh);
            }

            return TScriptInterface<IWeapon>(NewWeapon);
        }
    }

    return nullptr;
}
// WeaponFactory.cpp
#include "WeaponFactory.h"
#include "GreatSword.h"
// #include "Dagger.h" // �ٸ� ���� Ŭ���� ���� (����)

TScriptInterface<IWeapon> UWeaponFactory::CreateWeapon(FName WeaponType)
{
    if (WeaponType == TEXT("GreatSword"))
    {
        UGreatSword* NewWeapon = NewObject<UGreatSword>();
        if (NewWeapon)
        {
            // ���� �޽� ���� (����)
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
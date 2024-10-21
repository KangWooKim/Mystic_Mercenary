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
            // ���� �޽��� �����͸� ���� �����˴ϴ�.
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
    // �߰� ���� Ÿ�� ó��
    return nullptr;
}

// GreatSword.cpp
#include "GreatSword.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"
#include "Mystic_Mercenary/PlayerCharacter/AttackStrategy.h"
#include "Mystic_Mercenary/PlayerCharacter/ComboAttackStrategy.h"

UGreatSword::UGreatSword()
{
    Damage = 50.f; // �⺻ ������ ����
    WeaponMesh = nullptr; // WeaponMesh�� �����͸� ���� ����
}

void UGreatSword::Attack(AGreatSwordCharacter* Character)
{
    if (Character && AttackStrategy.IsValid())
    {
        // ���� ������ ���� ���� ����
        AttackStrategy->ExecuteAttack(Character);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackStrategy is not valid or Character is null!"));
    }
}

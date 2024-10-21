// GreatSword.cpp
#include "GreatSword.h"
#include "Mystic_Mercenary/PlayerCharacter/GreatSwordCharacter.h"
#include "Mystic_Mercenary/PlayerCharacter/AttackStrategy.h"
#include "Mystic_Mercenary/PlayerCharacter/ComboAttackStrategy.h"

UGreatSword::UGreatSword()
{
    Damage = 50.f; // 기본 데미지 설정
    WeaponMesh = nullptr; // WeaponMesh는 에디터를 통해 설정
}

void UGreatSword::Attack(AGreatSwordCharacter* Character)
{
    if (Character && AttackStrategy.IsValid())
    {
        // 공격 전략을 통해 공격 실행
        AttackStrategy->ExecuteAttack(Character);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackStrategy is not valid or Character is null!"));
    }
}

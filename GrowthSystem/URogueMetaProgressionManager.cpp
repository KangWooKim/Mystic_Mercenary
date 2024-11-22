#include "URogueMetaProgressionManager.h"
#include "UEnemyTypeDamageUpgradeStrategy.h"

URogueMetaProgressionManager* URogueMetaProgressionManager::Instance = nullptr;

URogueMetaProgressionManager* URogueMetaProgressionManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<URogueMetaProgressionManager>();
    }
    return Instance;
}

void URogueMetaProgressionManager::OnEnemyDefeated(FName EnemyType, AGreatSwordCharacter* Character)
{
    UEnemyTypeDamageUpgradeStrategy* UpgradeStrategy = NewObject<UEnemyTypeDamageUpgradeStrategy>();
    UpgradeStrategy->SetEnemyType(EnemyType);
    UpgradeStrategy->ApplyUpgrade(Character);
}
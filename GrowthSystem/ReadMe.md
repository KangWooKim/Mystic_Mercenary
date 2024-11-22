# Growth System

## 1. 특정 타입에 대한 영구적 데미지 증가
URogueMetaProgressionManager Class는 싱글톤 패턴을 홗용하여 캐릭터의 성장을 적용하는 기능을 가지고 있습니다. IUpgradeStrategy Class를 상속 받는
UEnemyTypeDamageUpgradeStrategy Class에서는 특정 타입 적을 처치할 경우 해당 타입의 적과 다시 젂투를 치를 경우 영구적 데미지 증가 버프를 부여합
니다. IUpgradeStrategy Class를 통해 다른 성장 시스템을 구현 가능하도록 설계했습니다. 

## 2. 랜덤한 버프 부여
UChallengeCommand Class의 Execute 메서드를 통해 버프를 부여합니다. 버프를 요청하는 캐릭터와 부여하는 실행자를 분리하여 구현해 다양한 버프 요소
를 쉽게 추가하거나 변경할 수 있도록 했습니다. 
버프는 UTemporaryBuffManager Class를 통해 캐릭터에게 적용되며 시갂 제한이 있는 버프는 이 Class를 통해 관리됩니다.

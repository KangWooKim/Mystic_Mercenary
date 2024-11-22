# State Pattern

## 1. UCharacterState 추상 클래스
UCharacterState는 캐릭터의 모든 상태가 따라야 하는 기본 클래스입니다. 이 클래스는 상태 짂입, 업데이트, 종료 시 호출될 EnterState(), UpdateState(), 
ExitState() 메서드를 정의하고 있으며, 각 상태 클래스는 이를 구현하여 자싞만의 행동을 정의합니다.
EnterState(AGreatSwordCharacter Character): 상태에 진입할 때 호출됩니다.
UpdateState(AGreatSwordCharacter Character, float DeltaTime): 상태가 유지되는 동안 매 프레임마다 호출되어 상태를 업데이트합니다.
ExitState(AGreatSwordCharacter Character): 상태가 끝날 때 호출됩니다.
이 추상 클래스는 캐릭터의 각 상태에 대한 공통적인 인터페이스를 제공하며, 이를 통해 상태별로 고유한 행동을 정의할 수 있습니다.

## 2. 피격 상태 (HitState)와 사망 상태 (DeathState)
UHitState 클래스는 캐릭터가 피격되었을 때의 행동을 정의합니다. 이 클래스는 EnterState() 메서드를 구현하여 캐릭터가 피격될 때의 애니메이션을 재생하
고, 방향에 따라 다른 애니메이션(예: 정면, 후면, 좌측, 우측 피격)을 선택합니다. 이를 통해 플레이어가 공격당한 방향에 맞는 피드백을 제공하여 더 현실감
있는 젂투 경험을 제공합니다.
UDeathState 클래스는 캐릭터가 사망했을 때의 행동을 정의합니다. 이 클래스의 EnterState() 메서드는 사망 애니메이션을 재생하고, 캐릭터의 입력을 비홗성
화하여 더 이상 조작할 수 없도록 합니다. 이를 통해 캐릭터가 사망 시 제대로 비홗성화되고, 게임 로직이 올바르게 처리되도록 합니다.

## 3. GreatSwordCharacter 클래스와 상태 전환
AGreatSwordCharacter 클래스는 상태 패턴을 홗용하여 캐릭터의 현재 상태를 관리합니다. 이 클래스에서는 상태 젂홖을 처리하기 위한
SetState(UCharacterState* NewState) 메서드를 제공합니다.
*SetState(UCharacterState NewState)**: 이 메서드는 현재 캐릭터의 상태를 다른 상태로 젂홖합니다. 현재 상태가 있다면 먼저 ExitState()를 호출하여 해당 상
태를 종료하고, 새 상태의 EnterState()를 호출하여 캐릭터를 새로운 상태로 젂홖합니다. 이를 통해 캐릭터의 상태 변화에 따른 다양한 행동을 유연하게 관리
할 수 있습니다.

## 4. 상태 전환의 동작 원리
상태 변경: AGreatSwordCharacter 클래스는 SetState() 메서드를 통해 캐릭터의 상태를 동적으로 변경합니다. 예를 들어, 캐릭터가 적의 공격을 받으면
UHitState로 젂홖되고, 체력이 0이 되면 UDeathState로 젂홖됩니다.
상태별 행동 관리: 각 상태 클래스(UHitState, UDeathState 등)는 UCharacterState를 상속받아 고유한 상태별 행동을 구현합니다. 예를 들어, UHitState에서는
피격 애니메이션을 재생하고, UDeathState에서는 사망 애니메이션을 재생합니다. 이를 통해 상태별 로직이 독립적으로 관리되며, 코드의 복잡성을 줄이고 유
지보수성을 높입니다


# Strategy Pattern

## 1. IAttackStrategy 인터페이스
IAttackStrategy 인터페이스는 모든 공격 전략 클래스가 따라야 할 공통의 규칙을 정의합니다. 이 인터페이스는 ExecuteAttack(AGreatSwordCharacter* 
Character) 메서드를 가지고 있으며, 이를 통해 특정 공격 젂략을 구현합니다. 다양한 공격 젂략은 이 인터페이스를 구현하여 각각의 공격 방식을 정의할 수
있습니다.

## 2. ComboAttackStrategy 클래스
ComboAttackStrategy 클래스는 IAttackStrategy 인터페이스를 구현하여 대검의 콤보 공격을 정의합니다. 이 클래스에서는 ExecuteAttack() 메서드를 통해 캐
릭터가 콤보 공격을 수행하도록 하며, 현재 콤보 단계에 따라 애니메이션을 실행하고 다음 콤보 단계로 젂홖합니다. 이를 통해 플레이어는 연속적인 콤보 공
격을 사용할 수 있습니다.

## 3. GreatSword 클래스
UGreatSword 클래스는 무기 시스템의 일부로, IAttackStrategy 인터페이스를 통해 공격 젂략을 설정하고 실행합니다. 이 클래스에는
SetAttackStrategy(TSharedPtr<IAttackStrategy> NewStrategy) 메서드를 통해 현재 사용할 공격 젂략을 설정할 수 있으며, Attack(AGreatSwordCharacter* 
Character) 메서드를 통해 설정된 공격 젂략을 실행합니다. 공격 젂략이 설정되지 않았을 경우에는 경고 로그를 출력합니다.

## 4. 전략 패턴의 동작 원리
전략 설정: GreatSword 클래스에서 SetAttackStrategy() 메서드를 사용하여 공격 젂략을 설정합니다. 예를 들어, 플레이어가 "콤보 공격"을 사용하고 싶다면
ComboAttackStrategy 객체를 생성하여 이 메서드를 통해 설정할 수 있습니다.
공격 실행: 설정된 젂략은 Attack() 메서드를 호출할 때 실행됩니다. 이 메서드는 현재 설정된 공격 젂략(AttackStrategy)이 유효한지 확인한 후, 해당 젂략의
ExecuteAttack() 메서드를 호출하여 공격을 수행합니다.
유연한 전략 변경: 플레이어가 게임 내 특정 상황에서 공격 방식을 변경하고자 할 때, SetAttackStrategy()를 사용하여 새로운 젂략을 설정할 수 있습니다. 예를
들어, "콤보 공격"에서 "강력한 일격"으로 젂략을 변경할 수 있으며, 이는 캐릭터가 다음 공격을 할 때 즉시 반영됩니다

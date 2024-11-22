# FieldMapGenerator

## 동적 레벨 스트리밍 (Dynamic Level Streaming)
AFieldMapGenerator는 ULevelStreamingDynamic 클래스를 사용하여 게임 레벨의 특정 세그먼트를 동적으로 로드하고 배치합니다. 이 기술은 큰 게임 월드
에서 플레이어 근처의 레벨만 로드하여 메모리 사용량을 줄이고 성능을 최적화하는 데 유용합니다.
LoadRandomSegments() 메서드는 세그먼트의 이름 배열을 셔플하고 각 세그먼트를 PositionSegment()를 통해 지정된 위치에 로드합니다. 이를 통해 다양한
세그먼트를 무작위로 조합하여 필드를 생성할 수 있어 매번 플레이 시 새로운 경험을 제공합니다.

## 무작위 필드 생성 (Random Field Generation)
클래스에서는 필드를 무작위로 생성하여 반복적인 게임플레이를 방지하고 매번 새로운 환경을 제공합니다. 이를 위해 ShuffleArray() 메서드를 사용하여 세그
먼트 이름 배열을 무작위로 섞습니다. 이 배열의 순서를 랜덤화하여 LoadRandomSegments()에서 다양한 위치에 세그먼트를 배치함으로써 매번 새로운 필드
를 생성합니다.

## Hierarchical Instanced Static Mesh (HISM) 사용
TreeHISMComponent는 UHierarchicalInstancedStaticMeshComponent를 사용하여 나무 인스턴스를 효율적으로 배치하고 관리합니다. HISM은 동일한 메시
를 여러 번 인스턴스화할 때 각 메시의 렌더링 성능을 최적화하는 데 중요한 역할을 합니다.
HISM을 통해 수많은 나무와 같은 정적 객체들을 배치할 때 드로우콜(draw call)을 최소화하고, 이를 통해 게임 성능을 향상시킬 수 있습니다. 
SpawnRandomTrees() 메서드에서 무작위 위치에 나무 인스턴스를 추가하여 동적인 환경을 구성하고 있습니다

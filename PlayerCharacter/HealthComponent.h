#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    void TakeDamage(float DamageAmount);

    float GetHealth() const { return CurrentHealth; }

    FOnDeath OnDeath;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Health")
        float MaxHealth;

    float CurrentHealth;
};
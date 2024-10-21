#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MaxHealth = 300.f;
    CurrentHealth = MaxHealth;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
    CurrentHealth -= DamageAmount;
    if (CurrentHealth <= 0.f)
    {
        CurrentHealth = 0.f;
        OnDeath.Broadcast();
    }
}
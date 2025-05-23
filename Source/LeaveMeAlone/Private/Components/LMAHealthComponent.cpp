// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "Components/LMAHealthComponent.h"

// Sets default values
ULMAHealthComponent::ULMAHealthComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

}

bool ULMAHealthComponent::IsDead() const
{
	return Health <= 0.0f;
}

// Called when the game starts or when spawned
void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	OnHealthChanged.Broadcast(Health);

	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}
	
}

void ULMAHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//Health -= Damage;

	if (IsDead())
		return;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(Health);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

bool ULMAHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool ULMAHealthComponent::AddHealth(float NewHealth)
{
	if (IsDead() || IsHealthFull())
		return false;
	Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	return true;
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "MarblesSpawner.h"
#include "Marble.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
AMarblesSpawner::AMarblesSpawner()
	:SphereComponent(CreateDefaultSubobject<USphereComponent>(FName(TEXT("SphereComponent"))))
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	// ...
	SphereRadius = 20.0f;
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(SphereRadius);
	NumberOfActors = 15;
	MarbleClass = nullptr;
}


// Called when the game starts
void AMarblesSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (MarbleClass)
	{
		const UWorld* World = this->GetWorld();
		if (World != nullptr)
		{
			for (uint32 i = 0; i < NumberOfActors; i++)
			{
				// spawn the projectile
				const FVector SpawnerLocation = this->GetActorLocation();
				const float SpawnLocation_X = SpawnerLocation.X - FMath::FRandRange(5.0f, SpawnerLocation.X + SphereRadius);
				const float SpawnLocation_Y = SpawnerLocation.Y - FMath::FRandRange(5.0f, SpawnerLocation.Y + SphereRadius);
				const FVector SpawnLocation = FVector(SpawnLocation_X, SpawnLocation_Y, SpawnerLocation.Z);
				AMarble* const Marble = this->GetWorld()->SpawnActor<AMarble>(SpawnLocation, FRotator(0.0f));
				Marble->SetLifeSpan(0);
				Marble->SetOwner(this);
			}
		}
	
	}
}


// Called every frame
void AMarblesSpawner::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);
}


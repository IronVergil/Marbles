// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MarblesSpawner.generated.h"

class USphereComponent;
class AMarble;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARBLES_API AMarblesSpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMarblesSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* const SphereComponent{nullptr};

	UPROPERTY(EditAnywhere, Category = "Spawns")
	TSubclassOf<AMarble> MarbleClass;
	
	UPROPERTY(Category = Gameplay, EditAnywhere)
	uint32 NumberOfActors;
	UPROPERTY(Category = Gameplay, EditAnywhere)
	float SphereRadius;

		
};

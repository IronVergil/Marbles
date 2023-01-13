// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Marble.generated.h"

UCLASS()
class MARBLES_API AMarble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly)
	UStaticMeshComponent* MarbleMesh{nullptr};

public:
	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetMarbleMesh() const { return MarbleMesh; }

};

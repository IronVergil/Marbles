// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SliderStaticMesh.generated.h"

class UStaticMeshComponent;

UCLASS()
class MARBLES_API ASliderStaticMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASliderStaticMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly)
	UStaticMeshComponent* SliderMesh{nullptr};


};

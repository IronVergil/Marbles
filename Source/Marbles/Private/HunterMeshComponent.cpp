// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterMeshComponent.h"

// Sets default values for this component's properties
UHunterMeshComponent::UHunterMeshComponent()
					 :MarbleMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh"))))
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MarbleMeshAsset(TEXT("/Game/Meshes/Marble.Marble"));
	MarbleMesh->SetStaticMesh(MarbleMeshAsset.Object);
}


// Called when the game starts
void UHunterMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHunterMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


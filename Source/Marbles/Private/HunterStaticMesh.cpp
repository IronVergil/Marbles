// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterStaticMesh.h"

// Sets default values
AHunterStaticMesh::AHunterStaticMesh()
	:HunterMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh"))))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = HunterMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MarbleMeshAsset(TEXT("/Game/Meshes/Hunter.Hunter"));
	HunterMesh->SetStaticMesh(MarbleMeshAsset.Object);
	HunterMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AHunterStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHunterStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


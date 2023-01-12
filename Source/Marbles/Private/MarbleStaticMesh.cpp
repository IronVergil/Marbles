// Fill out your copyright notice in the Description page of Project Settings.


#include "MarbleStaticMesh.h"

// Sets default values
AMarbleStaticMesh::AMarbleStaticMesh()
	:MarbleMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh"))))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = MarbleMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MarbleMeshAsset(TEXT("/Game/Meshes/Marble.Marble"));
	MarbleMesh->SetStaticMesh(MarbleMeshAsset.Object);
	MarbleMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AMarbleStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMarbleStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


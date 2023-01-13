// Fill out your copyright notice in the Description page of Project Settings.


#include "Marble.h"

// Sets default values
AMarble::AMarble()
		:MarbleMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Projectile"))))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/Meshes/Marble.Marble"));
	MarbleMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	RootComponent = MarbleMesh;
	MarbleMesh->SetNotifyRigidBodyCollision(true);

}
// Called when the game starts or when spawned
void AMarble::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMarble::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MarbleMesh->SetSimulatePhysics(true);
	MarbleMesh->SetMassOverrideInKg(NAME_None, 0.06);
	MarbleMesh->SetLinearDamping(0.01);
	MarbleMesh->SetAngularDamping(0.5);
}

// Called every frame
void AMarble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


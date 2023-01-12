// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

// Sets default values
AArrow::AArrow()
		:ArrowMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh"))))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = ArrowMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MarbleMeshAsset(TEXT("/Game/Meshes/Arrow.Arrow"));
	ArrowMesh->SetStaticMesh(MarbleMeshAsset.Object);
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


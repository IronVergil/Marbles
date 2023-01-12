// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderStaticMesh.h"

// Sets default values
ASliderStaticMesh::ASliderStaticMesh()
				  :SliderMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh"))))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = SliderMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SliderMeshAsset(TEXT("/Game/Meshes/Slider.Slider"));
	SliderMesh->SetStaticMesh(SliderMeshAsset.Object);
	SliderMesh->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	SliderMesh->SetWorldScale3D(FVector(1.0f, 4.0f, 1.0f));
}

// Called when the game starts or when spawned
void ASliderStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASliderStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


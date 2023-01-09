// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"


#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

const FName AHunter::MoveRightBinding("MoveRight");

// Sets default values
AHunter::AHunter()
		:HunterMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh")))),SphereComponent(CreateDefaultSubobject<USphereComponent>(FName(TEXT("SphereComponent")))),SpringArmComponent(CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("SpringArmComponent")))),
		CameraComponent(CreateDefaultSubobject<UCameraComponent>(FName(TEXT("CameraComponent"))))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereComponent; 

	HunterMesh->SetupAttachment(SphereComponent);
	HunterMesh->SetRelativeLocation(FVector(-46.0f,0.0f,0.0f));
	
	SpringArmComponent->SetupAttachment(SphereComponent);
	SpringArmComponent->TargetArmLength = 250.f;
	SpringArmComponent->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArmComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;

	
	
	//CameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 80.0f;

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveRightBinding);

}

void AHunter::LockOnMouse()
{
}

void AHunter::LockOnTouch()
{
}

void AHunter::FireShot(FVector FireDirection)
{
}

void AHunter::ShotTimerExpired()
{
}



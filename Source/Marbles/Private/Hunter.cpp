// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"


#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HunterProjectile.h"

// Sets default values
AHunter::AHunter()
		:HunterMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh")))),SphereComponent(CreateDefaultSubobject<USphereComponent>(FName(TEXT("SphereComponent")))),SpringArmComponent(CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("SpringArmComponent")))),
		CameraComponent(CreateDefaultSubobject<UCameraComponent>(FName(TEXT("CameraComponent")))),PlayerControllerRef(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereComponent; 

	HunterMeshRelativeLocation_X = -52.0f;
	HunterMesh->SetupAttachment(SphereComponent);
	HunterMesh->SetRelativeLocation(FVector(HunterMeshRelativeLocation_X,0.0f,0.0f));
	
	SpringArmComponent->SetupAttachment(SphereComponent);
	SpringArmComponent->TargetArmLength = 250.f;
	SpringArmComponent->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArmComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;

	
	
	//CameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 62.0f;

	//this->SetActorEnableCollision(false);
	//HunterMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	
	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	bCalculateProjectilePath = false;
	SpeedMultiplier = 1.0f;
	TouchLocationOnTick = FVector(0.0f);
	LastTouchLocation = FVector(0.0f);
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();
	PlayerControllerRef = this->GetNetOwningPlayer()->GetPlayerController(this->GetWorld());
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateProjectilePath();
}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHunter::LockOnTouch);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHunter::TouchReleased);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHunter::MoveRight);
}


void AHunter::LockOnTouch(ETouchIndex::Type FingerIndex, FVector Location)
{
	UE_LOG(LogTemp, Warning, TEXT("LockOnTouch Called"));
	bCalculateProjectilePath = true;
	PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouch);
	this->HunterMesh->SetRelativeLocation(FVector(HunterMeshRelativeLocation_X, HitResultOnTouch.Location.Y, 0.0f));
	LastTouchLocation = this->HunterMesh->GetComponentLocation();
}

void AHunter::TouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	bCalculateProjectilePath = false;
}


void AHunter::MoveRight(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MoveRight Called"));
	if ((Controller != NULL) && (Value != 0.0f))
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveRight Called"));
		HunterMesh->MoveComponent(HunterMesh->GetRightVector() * Value,FRotator(0.0f),false);
	}
}

void AHunter::FireShot(FVector FireDirection)
{
	// If we are pressing fire stick in a direction
	if (FireDirection.SizeSquared() > 0.0f)
	{
		const FRotator FireRotation = FireDirection.Rotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			// spawn the projectile
			World->SpawnActor<AHunterProjectile>(SpawnLocation, FireRotation);
		}

		// bCanFire = false;
		// World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AHunter::ShotTimerExpired, FireRate);

		// try and play the sound if specified
		// if (FireSound != nullptr)
		// {
		// 	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		// }

		bCanFire = false;
	}
}

void AHunter::CalculateProjectilePath()
{
	if (bCalculateProjectilePath)
	{
		FHitResult HitResultOnTouchTick;
		PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouchTick);
		TouchLocationOnTick = HitResultOnTouchTick.Location;
		float LauchVelocity = (TouchLocationOnTick - LastTouchLocation).X * SpeedMultiplier;
		FPredictProjectilePathParams Params;
		Params.StartLocation = this->HunterMesh->GetComponentLocation();
		Params.LaunchVelocity = this->HunterMesh->GetComponentRotation().Vector() * LauchVelocity;
		Params.ProjectileRadius = 2.0f;
		FPredictProjectilePathResult PathResult;
		bool bHit = UGameplayStatics::PredictProjectilePath(this->GetWorld(),Params,PathResult);
		
	}
}

	


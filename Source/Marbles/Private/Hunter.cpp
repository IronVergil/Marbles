// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"

// Sets default values
AHunter::AHunter()
		:HunterMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("ShipMesh")))),SphereComponent(CreateDefaultSubobject<USphereComponent>(FName(TEXT("SphereComponent")))),SpringArmComponent(CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("SpringArmComponent")))),
		CameraComponent(CreateDefaultSubobject<UCameraComponent>(FName(TEXT("CameraComponent")))),ProjectileClass(nullptr), PlayerControllerRef(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereComponent; 

	HunterMeshRelativeLocation_X = -41.0f;
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
	SpeedMultiplier = 4.0f;
	TouchLocationOnTick = FVector(0.0f);
	LastTouchLocation = FVector(0.0f);
	RotationSpeed = 1.0f;
	LauchVelocity = FVector(0.0f);
	
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
	//PlayerInputComponent->BindTouch(IE_Repeat, this, &AHunter::AimAt);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHunter::MoveRight);
}


void AHunter::LockOnTouch(ETouchIndex::Type FingerIndex, FVector Location)
{

	bCalculateProjectilePath = true;
	bCanFire = true;
	PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouch);
	this->HunterMesh->SetRelativeLocation(FVector(HunterMeshRelativeLocation_X, HitResultOnTouch.Location.Y, 0.0f));
	LastTouchLocation = this->HunterMesh->GetComponentLocation();
}

void AHunter::TouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	bCalculateProjectilePath = false;
	FireShot();
	bCanFire = false;
}

void AHunter::AimAt(ETouchIndex::Type FingerIndex, FVector Location)
{
	PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, TouchLocationRotate);
	
	float DeltaRotationY_Pitch = (HitResultOnTouch.Location - TouchLocationRotate.Location).Y * this->GetWorld()->GetDeltaSeconds() * RotationSpeed;
	float DeltaRotationZ_Yaw = (HitResultOnTouch.Location - TouchLocationRotate.Location).X * this->GetWorld()->GetDeltaSeconds() * RotationSpeed * -1.0f;
	
	this->HunterMesh->SetWorldRotation(FRotator(DeltaRotationY_Pitch, DeltaRotationZ_Yaw, 0.0f));

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



void AHunter::CalculateProjectilePath()
{
	if (bCalculateProjectilePath)
	{
		FHitResult HitResultOnTouchTick;
		PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouchTick);
		TouchLocationOnTick = HitResultOnTouchTick.Location;
		FVector HunterLocationOnTick = this->HunterMesh->GetComponentLocation();
		// float LauchVelocityX = (TouchLocationOnTick - LastTouchLocation).X * SpeedMultiplier;
		// float LauchVelocityY = (TouchLocationOnTick - LastTouchLocation).Y * SpeedMultiplier;
		// float LauchVelocityZ = (TouchLocationOnTick - LastTouchLocation).Z * SpeedMultiplier;

		LauchVelocity = (HunterLocationOnTick - TouchLocationOnTick ) * SpeedMultiplier;
		
		FPredictProjectilePathParams Params;
		Params.StartLocation = this->HunterMesh->GetComponentLocation();
		Params.LaunchVelocity = LauchVelocity;
		// Params.LaunchVelocity.X = LauchVelocityX;
		// Params.LaunchVelocity.Y = LauchVelocityY;
		// Params.LaunchVelocity.Z = LauchVelocityZ;
		 Params.LaunchVelocity.Z = 0.0f;
		Params.ProjectileRadius = 2.0f;
		FPredictProjectilePathResult PathResult;
		bool bHit = UGameplayStatics::PredictProjectilePath(this->GetWorld(),Params,PathResult);
		

		TArray<FVector> PointLocation;
		for(auto PathPoint : PathResult.PathData)
		{
			PointLocation.Add(PathPoint.Location);
			//DrawDebugPoint(this->GetWorld(), PathPoint.Location, 20.0f, FColor::Green, true );
		}
		//SplinePath->SetSplinePoints(PointLocation, ESplineCoordinateSpace::World);
		DrawDebugPoint(this->GetWorld(), HitResultOnTouch.ImpactPoint, 20.0f, FColor::Green, true );
	}
}


void AHunter::FireShot()
{
	if (bCanFire == true)
	{
		UWorld* const World = this->GetWorld();
		if (World != nullptr)
		{
			// spawn the projectile
			const FVector SpawnLocation = this->HunterMesh->GetComponentLocation();

			AProjectile* const Projectile = this->GetWorld()->SpawnActor<AProjectile>(SpawnLocation, FRotator(0.0f));
			Projectile->SetOwner(this);
			
			//const AProjectile* Projectile = World->SpawnActor<AProjectile>(FVector(0.0f,0.0f,10.0f), FRotator(0.0f));
			//UGameplayStatics::SpawnEmitterAtLocation(this->GetWorld(), ExplosionFX, SpawnLocation);
			Projectile->GetProjectileMesh()->AddImpulseAtLocation(LauchVelocity * 1.0f, Projectile->GetActorLocation());
			const FVector ProjectileLocation  = Projectile->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Spawned"));
			UE_LOG(LogTemp, Warning, TEXT("Spawn Location is: %s"), *SpawnLocation.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Projectile Location is: %s"), *ProjectileLocation.ToString());
		}

		// bCanFire = false;
		// World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AHunter::ShotTimerExpired, FireRate);

		// try and play the sound if specified
		// if (FireSound != nullptr)
		// {
		// 	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		// 
	}
}
	


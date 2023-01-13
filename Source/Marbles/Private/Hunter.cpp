// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"

#include "DrawDebugHelpers.h"
#include "HunterStaticMesh.h"
#include "SliderStaticMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"

// Sets default values
AHunter::AHunter()
		:SphereComponent(CreateDefaultSubobject<USphereComponent>(FName(TEXT("SphereComponent")))),SpringArmComponent(CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("SpringArmComponent")))),
		CameraComponent(CreateDefaultSubobject<UCameraComponent>(FName(TEXT("CameraComponent")))),ProjectileClass(nullptr),SpawnedHunterMesh{nullptr},SpawnedSliderMesh{nullptr}, PlayerControllerRef(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereComponent;
	HunterMeshRelativeLocation_X = -41.0f;
	HunterMeshRelativeLocation_Y = 41.0f;
	SliderMeshRelativeLocation_X = -51.0f;
	SliderMeshRelativeLocation_Y = 51.0f;
	
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
	bIsTouchingHunter = false;
	bIsTouchingSlider = false;

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
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetShowMouseCursor(true);
		const FVector HunterSpawnLocation = FVector(this->GetActorLocation().X + HunterMeshRelativeLocation_X, 0.0f, this->GetActorLocation().Z);
		const FVector SliderSpawnLocation = FVector(this->GetActorLocation().X + SliderMeshRelativeLocation_X, 0.0f, this->GetActorLocation().Z);
		SpawnedHunterMesh = this->GetWorld()->SpawnActor<AHunterStaticMesh>(HunterSpawnLocation, FRotator(0.0f));
		SpawnedSliderMesh = this->GetWorld()->SpawnActor<ASliderStaticMesh>(SliderSpawnLocation, FRotator(0.0f));
	}
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateProjectilePath();
	UpdateHunterLocation();
}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHunter::TouchPressed);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHunter::TouchReleased);
	//PlayerInputComponent->BindTouch(IE_Repeat, this, &AHunter::UpdateHunterLocation);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHunter::MoveRight);
}


void AHunter::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{

	PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouch);
	
	if ((HitResultOnTouch.GetActor()) == SpawnedSliderMesh )
	{
		bIsTouchingSlider = true;
		bCanFire = false;
		UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *HitResultOnTouch.GetActor()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Touching Slider"));
	}
	if ((HitResultOnTouch.GetActor()) == SpawnedHunterMesh )
	{
		bIsTouchingHunter = true;
		bCanFire = true;
		bCalculateProjectilePath = true;
		UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *HitResultOnTouch.GetActor()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Touching Hunter"));
	}
}

void AHunter::TouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	bCalculateProjectilePath = false;
	FireShot();
	bIsTouchingHunter = false;
	bIsTouchingSlider = false;
}

void AHunter::AimAt(ETouchIndex::Type FingerIndex, FVector Location)
{
	PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, TouchLocationRotate);
	
	float DeltaRotationY_Pitch = (HitResultOnTouch.Location - TouchLocationRotate.Location).Y * this->GetWorld()->GetDeltaSeconds() * RotationSpeed;
	float DeltaRotationZ_Yaw = (HitResultOnTouch.Location - TouchLocationRotate.Location).X * this->GetWorld()->GetDeltaSeconds() * RotationSpeed * -1.0f;
	
	//this->HunterMesh->SetWorldRotation(FRotator(DeltaRotationY_Pitch, DeltaRotationZ_Yaw, 0.0f));
	

}


void AHunter::MoveRight(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MoveRight Called"));
	if ((Controller != NULL) && (Value != 0.0f))
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveRight Called"));
		//HunterMesh->MoveComponent(HunterMesh->GetRightVector() * Value,FRotator(0.0f),false);
	}
}



void AHunter::CalculateProjectilePath()
{
	if (bCalculateProjectilePath)
	{
		FHitResult HitResultOnTouchTick;
		PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouchTick);
		TouchLocationOnTick = HitResultOnTouchTick.Location;
		FVector HunterLocationOnTick = this->SpawnedHunterMesh->GetActorLocation();
		// float LauchVelocityX = (TouchLocationOnTick - LastTouchLocation).X * SpeedMultiplier;
		// float LauchVelocityY = (TouchLocationOnTick - LastTouchLocation).Y * SpeedMultiplier;
		// float LauchVelocityZ = (TouchLocationOnTick - LastTouchLocation).Z * SpeedMultiplier;

		LauchVelocity = (HunterLocationOnTick - TouchLocationOnTick ) * SpeedMultiplier;
		
		FPredictProjectilePathParams Params;
		Params.StartLocation = this->SpawnedHunterMesh->GetActorLocation();
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

void AHunter::UpdateHunterLocation()
{
	if (PlayerControllerRef)
	{
		PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility,false, HitResultOnTouch);
	}
	if (HitResultOnTouch.GetActor() == SpawnedSliderMesh && !bIsTouchingHunter)
	{
		const float NewLocation_Y = HitResultOnTouch.Location.Y;
		SpawnedSliderMesh->SetActorRelativeLocation(FVector(SliderMeshRelativeLocation_X,NewLocation_Y, this->GetActorLocation().Z));
		SpawnedHunterMesh->SetActorRelativeLocation(FVector(HunterMeshRelativeLocation_X,NewLocation_Y, this->GetActorLocation().Z));
	}
}


void AHunter::FireShot()
{
	if (bCanFire == true)
	{
		const UWorld* World = this->GetWorld();
		if (World != nullptr)
		{
			// spawn the projectile
			const FVector SpawnLocation = this->SpawnedHunterMesh->GetActorLocation();

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
		bCanFire = false;
		// bCanFire = false;
		// World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AHunter::ShotTimerExpired, FireRate);

		// try and play the sound if specified
		// if (FireSound != nullptr)
		// {
		// 	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		// 
	}
}
	


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hunter.generated.h"


// Forward Declarations
class AHunterStaticMesh;
class ASliderStaticMesh;
class USpringArmComponent;	
class UCameraComponent;
class USphereComponent;
class AProjectile;

UCLASS()
class MARBLES_API AHunter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHunter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* const SphereComponent{nullptr};
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* const SpringArmComponent{nullptr};
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* const CameraComponent{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* const ExplosionFX{nullptr};

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AHunterStaticMesh> HunterMeshActor;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<ASliderStaticMesh> SliderMeshActor;

	UPROPERTY()
	AHunterStaticMesh* SpawnedHunterMesh;
	UPROPERTY() 
	ASliderStaticMesh* SpawnedSliderMesh;
	
	
	// UPROPERTY(VisibleAnywhere, Category = "Components")
	// USplineComponent* const SplinePath{nullptr};
	// UPROPERTY(VisibleAnywhere, Category = "Components")
	// UNiagaraComponent* const PathVisualEffect{nullptr};
	
	UPROPERTY()
	APlayerController* PlayerControllerRef;

	UPROPERTY()
	FHitResult HitResultOnMouse;
	UPROPERTY()
	FHitResult HitResultOnTouch;
	UPROPERTY()
	FHitResult TouchLocationRotate;
	UPROPERTY()
	bool bCalculateProjectilePath;
	UPROPERTY(EditAnywhere, Category="Gameplay")
	float SpeedMultiplier;
	UPROPERTY()
	FVector TouchLocationOnTick;
	UPROPERTY()
	FVector LastTouchLocation;
	UPROPERTY()
	float RotationSpeed;
	UPROPERTY()
	FVector LauchVelocity;
	UPROPERTY()
	bool bIsTouchingHunter;
	UPROPERTY()
	bool bIsTouchingSlider;
	
	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere)
	FVector GunOffset;

	UPROPERTY(Category = Gameplay, EditAnywhere)
	float HunterMeshRelativeLocation_X;
	UPROPERTY(Category = Gameplay, EditAnywhere)
	float HunterMeshRelativeLocation_Y;
	
	UPROPERTY(Category = Gameplay, EditAnywhere)
	float SliderMeshRelativeLocation_X;
	UPROPERTY(Category = Gameplay, EditAnywhere)
	float SliderMeshRelativeLocation_Y;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere)
	float MoveSpeed;

	/* Flag to control firing  */
	bool bCanFire;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

public:
	// Static names for axis bindings
	static const FName MoveRightBinding;

private:
	/** Called for side to side input */
	void MoveRight(float Value);
	
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void TouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void TouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void AimAt(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	/* Fire a shot in the specified direction */
	void FireShot();
	
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void CalculateProjectilePath();
	UFUNCTION(BlueprintCallable, Category = "Targeting")	
	void UpdateHunterLocation();

};

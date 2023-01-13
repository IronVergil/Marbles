// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Marble.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
			:ProjectileMesh(CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Projectile"))))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/Meshes/Projectile.Projectile"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetMassOverrideInKg(NAME_None, 0.06);
	ProjectileMesh->SetLinearDamping(0.01);
	ProjectileMesh->SetAngularDamping(0.5);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const AMarble* Marble = Cast<AMarble>(OtherActor);

	if (Marble)
	{
		UStaticMeshComponent* Mesh = Marble->GetMarbleMesh();
		Mesh->AddImpulseAtLocation(this->GetActorForwardVector() * 3.0f, OtherActor->GetActorLocation());
	}
}

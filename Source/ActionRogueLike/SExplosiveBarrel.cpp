// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	//Unlike blueprints, collision presets is not automatically changed to physics body if you enable simulate physics in cpp
	MeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	//by default (true), the force component adds a small force to everything around it in a small radius
	ForceComp->SetAutoActivate(false);
	
	ForceComp->Radius = 700.f;
	ForceComp->ImpulseStrength = 2500.f;

	// Ignores current 'Velocity' and 'Mass' of other objects (if false, the impulse strength will be much higher to push most objects depending on Mass and velocity)
	ForceComp->bImpulseVelChange = true;

	// Default constructor of component already adds 4 object types to affect, excluding WorldDynamic (Check RadialForceComponent.cpp at line 27-31). Right now we add world dynamic as well
	// Or in BP Radial Force, Check into Radial Force Component>Object Types To Affect
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//can be placed in constructor or begin play. But generally advised placed in PostInitializeComponents() function because it runs in between the constructor and BeginPlay()
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
}


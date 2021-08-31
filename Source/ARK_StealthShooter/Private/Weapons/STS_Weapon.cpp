// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/STS_Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "../ARK_StealthShooter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASTS_Weapon::ASTS_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = WeaponMesh;

	ShotDistance = 10000.f;
	ShotDamage = 20.f;

	bIsDebug = false;

	RoundsPerMinute = 600.0f;

	MuzzleSocketName = "none";

}

//===============================================================================================================
// Called when the game starts or when spawned
void ASTS_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	CadencyShoot = 60 / RoundsPerMinute;

}

//===============================================================================================================
void ASTS_Weapon::Fire()
{

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{

		FVector EyeLocation;
		FRotator EyeRotation;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShotDirection * ShotDistance);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, WEAPON_COLLISION, QueryParams))
		{

			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, ShotDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
			
		}
		
		if (bIsDebug)
		{
			DrawDebugLine(GetWorld(), EyeLocation, Hit.ImpactPoint, FColor::White, false, 1.0f, 0, 1.0f);
		}

		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, MuzzleSocketName);
		}

	}

	LastFireTime = GetWorld()->TimeSeconds;

}

//===============================================================================================================
void ASTS_Weapon::StartFire()
{

	float FirstDelay = FMath::Max(0.0f, LastFireTime + CadencyShoot - GetWorld()->TimeSeconds);

	GetWorldTimerManager().SetTimer(TimerAutoFire, this, &ASTS_Weapon::Fire, CadencyShoot, true, FirstDelay);

}

//===============================================================================================================
void ASTS_Weapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerAutoFire);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STS_Weapon.generated.h"

class UStaticMeshComponent;
class UParticleSystem;

UCLASS()
class ARK_STEALTHSHOOTER_API ASTS_Weapon : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;

protected:

	UPROPERTY(EditDefaultsOnly)
	float ShotDistance;
	
	UPROPERTY(EditDefaultsOnly)
	float ShotDamage;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f))
	float RoundsPerMinute;

	float CadencyShoot;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly)
	bool bIsDebug;

	FTimerHandle TimerAutoFire;

	float LastFireTime;
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleSocketName;

public:	
	// Sets default values for this actor's properties
	ASTS_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Fire();
		
public:	

	void StartFire();

	void StopFire();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STS_Weapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class ARK_STEALTHSHOOTER_API ASTS_Weapon : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;
	
public:	
	// Sets default values for this actor's properties
	ASTS_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};

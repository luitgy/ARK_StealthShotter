// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STS_BaseCharacter.generated.h"

class ASTS_Weapon;
class USTS_HealthComponent;
class UAnimInstance;

UCLASS()
class ARK_STEALTHSHOOTER_API ASTS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASTS_Weapon> StartingWeaponClass;

	UPROPERTY(BlueprintReadOnly)
	ASTS_Weapon* CurrentWeapon;
		
	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USTS_HealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category  = "Melee")
	UAnimMontage* MeleeAttackMontage; 

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsMeleeAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeAttackBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float MeleeAttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	TArray<TEnumAsByte<EObjectTypeQuery> >  MeleeObjectTypes;

	UAnimInstance* AnimInstance;

public:
	// Sets default values for this character's properties
	ASTS_BaseCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void StartMelee();

	UFUNCTION(BlueprintCallable)
	void FinishMelee(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void DoMeleeAttack();

	UFUNCTION()
	void OnHealthChanged(USTS_HealthComponent * HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnDeath(USTS_HealthComponent * HealthComp, class AController* InstigatedBy, AActor* Killer);

};

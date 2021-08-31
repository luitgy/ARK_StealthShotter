// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STS_BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Weapons/STS_Weapon.h"
#include "Component/STS_HealthComponent.h"
#include <Animation/AnimInstance.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ASTS_BaseCharacter::ASTS_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	HealthComponent = CreateDefaultSubobject<USTS_HealthComponent>(TEXT("HealthComponent"));

	bIsMeleeAttacking = false;


}

//===============================================================================================================
// Called when the game starts or when spawned
void ASTS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(StartingWeaponClass))
	{
		
		CurrentWeapon = GetWorld()->SpawnActor<ASTS_Weapon>(StartingWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
		}

	}

	if (IsValid(HealthComponent))
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ASTS_BaseCharacter::OnHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &ASTS_BaseCharacter::OnDeath);
	}
	
	AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ASTS_BaseCharacter::FinishMelee);
	}

}

//===============================================================================================================
void ASTS_BaseCharacter::StartFire()
{

	if (!IsValid(HealthComponent))
	{
		return;
	}

	if (HealthComponent->GetIsDied())
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}

}

//===============================================================================================================
void ASTS_BaseCharacter::StopFire()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}

}

//===============================================================================================================
void ASTS_BaseCharacter::StartMelee()
{

	if (bIsMeleeAttacking)
	{
		return;
	}

	AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance != nullptr)
	{
		bIsMeleeAttacking = true;
		GetMovementComponent()->StopMovementImmediately();		
		AnimInstance->Montage_Play(MeleeAttackMontage);
	}

}

//===============================================================================================================
void ASTS_BaseCharacter::FinishMelee(UAnimMontage* AnimMontage, bool bInterrupted)
{

	if (AnimMontage == MeleeAttackMontage)
	{
		bIsMeleeAttacking = false;
	}
		
}

//===============================================================================================================
void ASTS_BaseCharacter::DoMeleeAttack()
{

	const FVector MeleeAttackLocation = GetMesh()->GetSocketLocation(MeleeAttackBoneName);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<AActor*> OutActors;

	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), MeleeAttackLocation, MeleeAttackRange, MeleeObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors))
	{

		AActor* HitActor = OutActors[0];

		if (IsValid(HitActor))
		{
			
			USTS_HealthComponent* HitActorHealthComp = Cast<USTS_HealthComponent>(HitActor->GetComponentByClass(USTS_HealthComponent::StaticClass()));

			if (IsValid(HitActorHealthComp))
			{
				HitActorHealthComp->Kill(GetController(), this);
			}

		}

	}

}

//===============================================================================================================
void ASTS_BaseCharacter::OnHealthChanged(USTS_HealthComponent * HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

}

//===============================================================================================================
void ASTS_BaseCharacter::OnDeath(USTS_HealthComponent * HealthComp, class AController* InstigatedBy, AActor* Killer)
{

	StopFire();
	GetMovementComponent()->StopMovementImmediately();
	SetActorEnableCollision(false);

}


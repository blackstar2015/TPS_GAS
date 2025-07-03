// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "TPSCharacterBase.generated.h"

UCLASS()
class TPSGAS_API ATPSCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ATPSCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
#pragma region Combat Interface
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual  UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual  FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;\
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
#pragma endregion

	FOnASCRegistered OnASCRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;

	UPROPERTY(EditAnywhere, Category = "Combat|Montages")
	TArray<FTaggedMontage> AttackMontages;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(ReplicatedUsing=OnRep_BeingShocked, BlueprintReadOnly)
	bool bIsBeingShocked = false;
	
	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	UFUNCTION()
	virtual void OnRep_BeingShocked();

	void SetCharacterClass(ECharacterClass InCharacterClass) {CharacterClass = InCharacterClass;}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual void InitializeDefaultAttributes() const;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;
	void AddCharacterAbilities();	
	void Dissolve();
	void DissolveWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Summons")
	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat|Properties")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;
	//
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat|Weapon Sockets")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat|Weapon Sockets")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Weapon Sockets")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Weapon Sockets")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Weapon Sockets")
	FName TailSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat|Attributes")
	TSubclassOf<UGameplayEffect>  DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat|Attributes")
	TSubclassOf<UGameplayEffect>  DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat|Attributes")
	TSubclassOf<UGameplayEffect>  DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Effects")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Effects")
	USoundBase* DeathSound;
	

	//Dissolve Effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> WeaponDissolveMaterialInstance;
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicInstance);
	
private:
	UPROPERTY(EditAnywhere, Category = "Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	//TODO: Handle Passive Abilities
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;
	//
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;
	//
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};

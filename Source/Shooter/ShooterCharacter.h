// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoType.h"
#include "ShooterCharacter.generated.h"


UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// called for forwards and backwards input
	void MoveForward(float Value);

	// called for side to side input
	void MoveRight(float Value);

	//called via input to turn at a given rate
	//@param rate this is a normalized rate, i.e 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	//called via input to look up.down at a given rate
	// @ param rate this is a normalized rate i.e 1.0 means 100% of desired rate
	void LookUpAtRate(float Rate);

	//rotate controller based on mouse X movement
	//@param value   The input value from mouse movement
	void Turn(float Value);

	//rotate controller based on mouse Y movement
	//@param value   The input value from mouse movement
	void LookUp(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1200.f;

	/** Getter for Character */
	FORCEINLINE ACharacter* GetCharacter() const { return Character; }

	void FireWeapon();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	// set bAiming to true or false with button press
	void AimingButtonPressed();
	void AimingButtonReleased();


	void CameraInterpZoom(float DeltaTime);

	// set base turn rate and base look up rate based on aiming
	void SetLookRates();

	void CalculateCrosshairSpread(float DeltaTime);

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	// line trace for items under the crosshairs
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	// trace for items if OverlappedItemCount > 0
	void TraceForItems();

	// spawns default weapon and equips it 
	class AWeapon* SpawnDefaultWeapon();

	// Takes a weapon and attaches it to the mesh
	void EquipWeapon(AWeapon* WeaponToEquip);

	//Detach Weapon and let it fall to the ground
	void DropWeapon();

	void SelectButtonPressed();
	void SelectButtonReleased();

	//drops currently equipped weapon and equips TraceHitItem
	void SwapWeapon(AWeapon* WeaponToSwap);

	//initialize the ammo map with ammo values
	void InitializeAmmoMap();

	// check to make sure our weapon has ammo
	bool WeaponHasAmmo();
	//FireWeapon functions
	void PlayFireSound();
	void SendBullet();
	void PlayGunfireMontage();
	//Bound to the R key and gamepad face button left
	void ReloadButtonPressed();
	//handle reloading of the weapon
	void ReloadWeapon();

	// Checks to see if we have ammo of the EquippedWeapon's ammo type
	bool CarryingAmmo();

	// called from animation blueprint with the grab clip notify
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	//called from animation blueprint with the release clip notify
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	void CrouchButtonPressed();

	virtual void Jump() override;

	// interps capsule half height when crouching/standing
	void InterpCapsuleHalfHeight(float DeltaTime);

	UPROPERTY()
		ACharacter* Character;

	void RequestSprintStart();

	void RequestSprintEnd();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Camera Boom positioning the camera begind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* CameraBoom;

	// camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* FollowCamera;
	
	// base turn rate, in deg/sec other scaling may affect final turn rate 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	float BaseTurnRate;

	// base look up/down rate in deg/sec other scaling may affect final turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	float BaseLookUpRate;

	// turn rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	float HipTurnRate;

	// look up rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	float HipLookUpRate;

	//turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	float AimingTurnRate;

	//look up rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	float AimingLookUpRate;

	// scale factor for mouse look sensitivity. turn rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	
	//scale factor for mouse look sensitivity look up rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;

	// scale  factor  for mouse look sensitivity   turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;

	// scale factor for mouse look sensitivity     look up rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;

	// Randomized Gunshot sound cue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "True"))
	class USoundCue* FireSound;

	// Flash spawned at barrelsocket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "True"))
	class UParticleSystem* MuzzleFlash;

	// montage for firing the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "True"))
	class UAnimMontage* HipFireMontage;

	// particles spawn upon impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "True"))
	UParticleSystem* ImpactParticles;

	// smoke trail for bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "True"))
	UParticleSystem* BeamParticles;

	// true when aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "True"))
	bool bAiming;

	//default camera feild of view value
	float CameraDefaultFOV;

	// feild of view value for when zoomed in 
	float CameraZoomedFOV;

	// current field of view this frame
	float CameraCurrentFOV;

	// interp speed for zooming when aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "True"))
	float ZoomInterpSpeed;

	// Determines the spread of the crosshairs
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crossahairs, meta = (AllowPrivateAccess = "True"))
	float CrosshairSpreadMultiplier;

	// velocity component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crossahairs, meta = (AllowPrivateAccess = "True"))
	float CrosshairVelocityFactor;

	// in air component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crossahairs, meta = (AllowPrivateAccess = "True"))
	float CrosshairInAirFactor;

	//aim component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crossahairs, meta = (AllowPrivateAccess = "True"))
	float CrosshairAimFactor;
	
	//shooting component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crossahairs, meta = (AllowPrivateAccess = "True"))
	float CrosshairShootingFactor;


	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	


	// Rate of Automatic Gunfire
	float AutomaticFireRate;

	// True when we can fire. False when waiting for the timer
	bool bShouldFire;

	// left mouse button or right console trigger  pressed
	bool bFireButtonPressed;

	

	//Sets a timer between gunshots
	FTimerHandle AutoFireTimer;

	//True if we should trace every frame for items
	bool bShouldTraceForItems;

	//number of overlapped AItems
	int8 OverlappedItemCount;

	// The AItem we hit last frame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLastFrame;

	// currently equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AWeapon> DefaultWeaponClass;

	// the item currently hit by our trace in TraceForItems (Could be null)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	// Distance outward from the camera for the interp destination
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "True"))
	float CameraInterpDistance;

	//Distance upward from the camera for the interp destination
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "True"))
	float CameraInterpElevation;

	// map to keep track of ammo of the different ammo types
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TMap < EAmmoType, int32> AmmoMap;

	// starting amount of 9mm ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo;

	// starting amount of AR ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;

	// Combat state, can only fire or reload if unoccupied 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	// montage for reload animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	//transform of the clip when we first grab the clip during reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;

	// scene components to attach to the charachters hand while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

	//true when crouching
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	// regular movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	// regular crouching speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	// current half height of the capsule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CurrentCapsuleHalfHeight;

	// half height of the capsule when not crouching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;

	// half height of the capsule when crouching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;

	// ground friction while not crouching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseGroundFriction;

	// ground friction while crouching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingGroundFriction;



public:
	// Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	// adds/subtracts to/from OverLappedItemCount and updates bShouldTracefor items
	void IncrementOverlappedItemCount(int8 Amount);

	FVector GetCameraInterpLocation();

	void GetPickupItem(AItem* Item);

	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE bool GetCrouching() const { return bCrouching; }

};

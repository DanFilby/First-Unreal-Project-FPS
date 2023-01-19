// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	//attach the camera component to the 
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	//allows the pawn to control the camera?
	FPSCameraComponent->bUsePawnControlRotation = true;

	//add first person mesh to the object
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	//only owner can see 'second' set of arms 
	FPSMesh->SetOnlyOwnerSee(true);

	//attach to first person camera
	FPSMesh->SetupAttachment(FPSCameraComponent);

	//Disable shadows so when in third person dont see two arm shadows
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//didnt work in constructor so put here
	GetMesh()->SetOwnerNoSee(true);
	
	UE_LOG(LogTemp, Warning, TEXT("This is major Tom from the character controller"));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

void AFPSCharacter::MoveForward(float axisValue)
{
	FVector forwardDir = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(forwardDir, axisValue);
}

void AFPSCharacter::MoveRight(float axisValue)
{
	FVector rightDir = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(rightDir, axisValue);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	if (ProjectileClass) {
		//get cam pos and rot
		FVector CameraLoaction;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLoaction, CameraRotation);

		//set slight the muzzle offset from the camera
		muzzleOffset.Set(100.0f, 0.0f, 0.0f);

		//final projectile spawn point
		FVector muzzleLocation = CameraLoaction + FTransform(CameraRotation).TransformVector(muzzleOffset);

		//final projectile rotation 
		FRotator muzzleRotation = CameraRotation;
		muzzleRotation.Pitch += 10.0f;

		//get world
		UWorld* World = GetWorld();

		if (World) {

			//spawn parameters 
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			//make this object responsible for damage done by the spawned projectile
			spawnParams.Instigator = GetInstigator();

			//spawn the projectile
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, muzzleLocation, muzzleRotation, spawnParams);

			if (Projectile) {
				//launch the projectile
				FVector launchDirection = muzzleRotation.Vector();
				Projectile->FireInDirection(launchDirection);
			}
		}
	}


}


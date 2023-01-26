// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

#include "BasicEnemy.h"


// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//make sure theres is root component
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	//Setup collisions
	if (!CollisionComponent) {

		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;

		CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	}

	//setup projectile movement behaviour
	if (!ProjectileMovementComponent) {
		//creates the component
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		
		//assigns the projectile movement to the collision component, giving the collison component this behaviour
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

		//setup porjectile
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	//setup bullet mesh
	if (!ProjectileMesh) {
		//adds mesh to bullet
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Models/Bullet/Sphere.Sphere'"));
		if (Mesh.Succeeded()) {
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}
	}

	//Setup material 
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Models/Bullet/BulletMat.BulletMat'"));
	if (Material.Succeeded()) {
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMesh);

		ProjectileMesh->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMesh->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		ProjectileMesh->SetupAttachment(RootComponent);
	}
	
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("This is Ziggy, we hit something boss"));

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("This is Ziggy, we hit something boss: %s"), *Hit.GetActor()->GetName()));

	//hit an enemy
	if (ABasicEnemy* enemy = Cast<ABasicEnemy>(Hit.GetActor())) {
		GEngine->AddOnScreenDebugMessage(-1, 21.0f, FColor::Red, "Dealing with an ENEMY here");
		enemy->EnemyTest();


	}
	

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity, Hit.ImpactPoint);

		UE_LOG(LogTemp, Warning, TEXT("This is Alien 16721, we hit a physics body"));


	}
	Destroy();

}

void AFPSProjectile::FireInDirection(const FVector& shootDirection)
{
	ProjectileMovementComponent->Velocity = shootDirection * ProjectileMovementComponent->InitialSpeed;
}


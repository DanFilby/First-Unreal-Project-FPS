// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"


// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//mesh
	EnemyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMeshComponent"));

	//EnemyMeshComponent->SetupAttachment(RootComponent);
}

void ABasicEnemy::EnemyGameplaySetup(void(* DeathCallback)() )
{
	DeathCallback();

}


// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicEnemy::EnemyTest()
{
	UE_LOG(LogTemp, Warning, TEXT("This is Enemy we've been hit ahhh "));
}



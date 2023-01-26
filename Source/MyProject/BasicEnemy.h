// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicEnemy.generated.h"

UCLASS()
class MYPROJECT_API ABasicEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EnemyTest();

	void EnemyGameplaySetup(void(*DeathCallback)());

	UPROPERTY(EditAnywhere, Category = Projectile)
		UStaticMeshComponent* EnemyMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UMaterialInstanceDynamic* EnemyMaterialInstance;

};

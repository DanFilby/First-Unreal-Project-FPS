// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectGameModeBase.h"

#include "BasicEnemy.h"
#include <Kismet/GameplayStatics.h>


void AMyProjectGameModeBase::StartPlay() {
	Super::StartPlay();

    check(GEngine != nullptr);
    
    UE_LOG(LogTemp, Warning, TEXT("This is ground control typing to the console"));


    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicEnemy::StaticClass(), FoundActors);

    GEngine->AddOnScreenDebugMessage(-1, 21.0f, FColor::Green, FString::Printf(TEXT("Ground Control Has Found %i Enemies"), FoundActors.Num()));

    for (size_t i = 0; i < FoundActors.Num(); i++)
    {
        ABasicEnemy* enemy = Cast<ABasicEnemy>(FoundActors[i]);
        enemy->EnemyGameplaySetup(&AMyProjectGameModeBase::EnemyDied);

    }

}

void AMyProjectGameModeBase::EnemyDied()
{
    GEngine->AddOnScreenDebugMessage(-1, 21.0f, FColor::Green, TEXT("EnemyHasPassed"));
}

void AMyProjectGameModeBase::UpdateEnemyCount()
{
    GEngine->AddOnScreenDebugMessage(-1, 21.0f, FColor::Blue, TEXT("Ground Control has called a member function"));


}

// Copyright 2020 Jesse Roffel

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrototypeMaze.generated.h"

class UPrototypeGrid;
class UArrowComponent;

UCLASS()
class MAZETRAVERSAL_API APrototypeMaze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrototypeMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    UPROPERTY(EditAnywhere, Category = "Generation")
		UPrototypeGrid* Grid = nullptr;

    UPROPERTY(EditAnywhere, Category = "Generation")
        TArray<int32> CellStack;

    UPROPERTY(EditAnywhere, Category = "Generation")
        int32 currentGenerationIndex = 0;

    UPROPERTY(EditAnywhere, Category = "Generation")
        bool bIsGenerationComplete = false;

    UPROPERTY(EditAnywhere, Category = "Generation")
        bool bCurrentlyBackTracking = false;

    UPROPERTY(EditAnywhere, Category = "Update")
        float updateSpeed = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Update")
        float currentTimer = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Visuals")
        UMaterial* visitedMaterial;

    UPROPERTY(EditAnywhere, Category = "Visuals")
        UMaterial* deadEndMaterial;

    UPROPERTY(EditAnywhere, Category = "Visuals")
        UArrowComponent* DirectionArrow = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Generation")
        void UpdateMazeGeneration();

};

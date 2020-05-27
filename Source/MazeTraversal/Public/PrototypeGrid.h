// Copyright 2020 Jesse Roffel

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PrototypeGrid.generated.h"

class APrototypeCell;
class UMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAZETRAVERSAL_API UPrototypeGrid : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPrototypeGrid();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

    UPROPERTY(EditAnywhere, Category = "Generation")
        float DistanceBetweenVertices = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Generation")
		int32 Width = 10;

    UPROPERTY(EditAnywhere, Category = "Generation")
        int32 Height = 10;

	UPROPERTY(EditAnywhere, Category = "Generation")
		TArray<APrototypeCell*> Cells;

    UPROPERTY(EditAnywhere, Category = "Generation")
        TSubclassOf<APrototypeCell> CellTypeToUse;


	UFUNCTION(BlueprintCallable, Category = "Generation")
		void GenerateCellComponents();

    UFUNCTION(BlueprintCallable, Category = "Cells")
        void SetCellMaterial(APrototypeCell* cellToSet, UMaterial* newMaterial);

    UFUNCTION(CallInEditor, Category = "Cells")
        int32 GetRandomAvailableNeightbour(int32 currentIndex);

    UFUNCTION(BlueprintCallable, Category = "Cells")
        APrototypeCell* GetCellElement(int32 index);

    UFUNCTION(BlueprintCallable, Category = "Cells")
        void RemoveWallsBetweenCells(APrototypeCell* startingCell, APrototypeCell* endingCell);

private:
    UFUNCTION(BlueprintCallable, Category = "Cells")
        void GetValidNeighboursIndices(TArray<int32>& OutValidNeighborCells, int32 currentIndex);
};

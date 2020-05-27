// Copyright 2020 Jesse Roffel


#include "PrototypeGrid.h"
#include "PrototypeCell.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPrototypeGrid::UPrototypeGrid()
{
}


// Called when the game starts
void UPrototypeGrid::BeginPlay()
{
	Super::BeginPlay();

	GenerateCellComponents();
}


void UPrototypeGrid::GenerateCellComponents()
{
	if (Width == 0 || Height == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to generate cell components, invalid parameters."));
		return;
	}

	if (CellTypeToUse == nullptr)
	{
        UE_LOG(LogTemp, Error, TEXT("Unable to generate cell components, cell to use is not set."));
        return;
	}

	UWorld* world = GetWorld();
    FActorSpawnParameters SpawnInfo;
	FVector cellLoc(0.0f);
	FRotator cellRot(0.0f);
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (uint8 h = 0; h < Height; h++)
	{
		for(uint8 w = 0; w < Width; w++)
		{
			FVector newPos = FVector(cellLoc.X + (DistanceBetweenVertices * w), cellLoc.Y + (DistanceBetweenVertices * h), cellLoc.Z);
			APrototypeCell* cell = world->SpawnActor<APrototypeCell>(CellTypeToUse, newPos, cellRot, SpawnInfo);
			cell->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			cell->SetActorRelativeLocation(newPos);
			Cells.Add(cell);
		}
	}
}

void UPrototypeGrid::SetCellMaterial(APrototypeCell* cellToSet, UMaterial* newMaterial)
{
	if (cellToSet == nullptr || newMaterial == nullptr)
	{
        UE_LOG(LogTemp, Error, TEXT("Unable to set cell material, invalid parameters."));
        return;
	}

	if (cellToSet->CenterMesh == nullptr)
	{
        UE_LOG(LogTemp, Error, TEXT("Unable to set cell material, cell static mesh is nullptr."));
        return;
	}

	cellToSet->CenterMesh->SetMaterial(0, newMaterial);
}

int32 UPrototypeGrid::GetRandomAvailableNeightbour(int32 currentIndex)
{
	//Get available neighbors
	TArray<int32> validNeighborsElements;
	GetValidNeighboursIndices(validNeighborsElements, currentIndex);

	int32 count = validNeighborsElements.Num();
	if (count == 0)
    {
		// We have researched the end of the current line. Time to backtrack.
        return -1;
	}

	// We found our candidate
	if (count == 1)
	{
		return validNeighborsElements[0];
	}

	// Return random neighbor based on the available indices
	return validNeighborsElements[FMath::RandRange(0, count-1)];
}

APrototypeCell* UPrototypeGrid::GetCellElement(int32 index)
{
	if (!Cells.IsValidIndex(index))
	{
        UE_LOG(LogTemp, Error, TEXT("Unable to get cell, index out of bounds."));
        return nullptr;
	}
	return Cells[index];
}

void UPrototypeGrid::RemoveWallsBetweenCells(APrototypeCell* startingCell, APrototypeCell* endingCell)
{
    FVector boundingBox;

	// Currently, get the bounding box of both Cell actors and use the center to cast two rays
	FVector startLocation = startingCell->GetActorLocation();
    startingCell->GetActorBounds(true, startLocation, boundingBox);
	startLocation.Z += (boundingBox.Z / 2);

	FVector endLocation = endingCell->GetActorLocation();
	endingCell->GetActorBounds(true, endLocation, boundingBox);
	endLocation.Z += (boundingBox.Z / 2);

	UWorld* world = GetWorld();
	TArray<FHitResult> hits;
	FCollisionQueryParams collisionParams;
	world->LineTraceMultiByChannel(hits, startLocation, endLocation, ECC_Visibility, collisionParams);
	UKismetSystemLibrary::DrawDebugLine(world, startLocation, endLocation, FColor::Red, 1.0f, 3.0f);


	for(int32 i = 0; i < hits.Num(); i++)
	{
		APrototypeCell* cellHit = Cast<APrototypeCell>(hits[i].Actor);
		if (cellHit == nullptr)
		{
			return;
		}

		UInstancedStaticMeshComponent* castAttempt = Cast<UInstancedStaticMeshComponent>(hits[i].Component);
		if (castAttempt != nullptr)
		{
			cellHit->RemoveInstanceStaticMeshIndex(castAttempt, hits[i].Item);
		}
	}

	world->LineTraceMultiByChannel(hits, startLocation, endLocation, ECC_Visibility, collisionParams);
    for (int32 i = 0; i < hits.Num(); i++)
    {
        APrototypeCell* cellHit = Cast<APrototypeCell>(hits[i].Actor);
        if (cellHit == nullptr)
        {
            return;
        }

        UInstancedStaticMeshComponent* castAttempt = Cast<UInstancedStaticMeshComponent>(hits[i].Component);
        if (castAttempt != nullptr)
        {
            cellHit->RemoveInstanceStaticMeshIndex(castAttempt, hits[i].Item);
        }
    }
}

void UPrototypeGrid::GetValidNeighboursIndices(TArray<int32>& OutValidNeighborCells, int32 currentIndex)
{
	int32 size = Width * Height;
	if (currentIndex >= size)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get valid neighbors, index out of bounds."));
		return;
	}

	int32 neighborIndex = 0;
	// Check above
	if (currentIndex - Width >= 0)
	{
		neighborIndex = currentIndex - Width;
		APrototypeCell* matchingCell = GetCellElement(neighborIndex);
		if (!matchingCell->bIsVisited)
		{
			OutValidNeighborCells.Add(neighborIndex);
		}
	}

	// Check below
    if ((currentIndex + Width) < size)
    {
        neighborIndex = currentIndex + Width;
        APrototypeCell* matchingCell = GetCellElement(neighborIndex);
        if (!matchingCell->bIsVisited)
        {
            OutValidNeighborCells.Add(neighborIndex);
        }
    }

	// Check left
    if (currentIndex % Width != 0)
    {
        neighborIndex = currentIndex - 1;
        APrototypeCell* matchingCell = GetCellElement(neighborIndex);
        if (!matchingCell->bIsVisited)
        {
            OutValidNeighborCells.Add(neighborIndex);
        }
    }
	// Check Right
    if ((currentIndex + 1) % Width != 0)
    {
        neighborIndex = currentIndex + 1;
        APrototypeCell* matchingCell = GetCellElement(neighborIndex);
        if (!matchingCell->bIsVisited)
        {
            OutValidNeighborCells.Add(neighborIndex);
        }
    }
}


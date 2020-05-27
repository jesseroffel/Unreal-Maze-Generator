// Copyright 2020 Jesse Roffel


#include "PrototypeMaze.h"
#include "Components/ArrowComponent.h"
#include "PrototypeGrid.h"
#include "PrototypeCell.h"

// Sets default values
APrototypeMaze::APrototypeMaze()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a root if not already found and set
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MazeRoot"));
	}

	// Create the grid first time
	if (!Grid)
	{
		Grid = CreateDefaultSubobject<UPrototypeGrid>("Grid");
	}
	Grid->SetupAttachment(RootComponent);

	// Construct Arrow for generation direction
    DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
    DirectionArrow->SetupAttachment(RootComponent);


	// Materials for visualing visited/deadend states when creating the maze.
    visitedMaterial = CreateDefaultSubobject<UMaterial>(TEXT("VisitedMaterial"));
    visitedMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/MazeGeneration/Visited.Visited'")).Object;

    deadEndMaterial = CreateDefaultSubobject<UMaterial>(TEXT("DeadEndMaterial"));
    deadEndMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/MazeGeneration/DeadEnd.DeadEnd'")).Object;
}


void APrototypeMaze::BeginPlay()
{
	Super::BeginPlay();

	// Get a random element from the grid
	int32 gridMaxIndex = (Grid->Width * Grid->Height) - 1;
	int32 initialIndex = FMath::RandRange(0, gridMaxIndex);

	APrototypeCell* initialCell = Grid->GetCellElement(initialIndex);
	if (initialCell == nullptr)
	{
        UE_LOG(LogTemp, Error, TEXT("Unable to get initial cell, nullptr returned."));
        return;
	}

	initialCell->bIsVisited = true;
	Grid->SetCellMaterial(initialCell, visitedMaterial);

	// Push initial element to stack to still process its neighbours. 
	CellStack.Push(initialIndex);
}

// Called every frame
void APrototypeMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGenerationComplete)
	{
		return;
	}

	// Update the visualisation and generation process based on a timer to showcase the visualisation.
	currentTimer += DeltaTime;
	if (currentTimer > updateSpeed)
	{
		currentTimer = 0.0f;
		UpdateMazeGeneration();
	}
}

void APrototypeMaze::UpdateMazeGeneration()
{
	if (CellStack.Num() == 0)
	{
		// Stack is empty, we're done.
		bIsGenerationComplete = true;
		return;
	}

	int32 currentCellIndex = CellStack.Pop();
	// If the current cell has any neighbors which have not been visited 	
	// Choose one of the unvisited neighbors
	int32 newNeighbor = Grid->GetRandomAvailableNeightbour(currentCellIndex);
	if (newNeighbor >= 0)
	{
		CellStack.Push(currentCellIndex);
		APrototypeCell* neighborCell = Grid->GetCellElement(newNeighbor);
		if (neighborCell == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Unable get neighbor cell, nullptr returned."));
			return;
		}

		// Remove the wall between the current cell and the chosen cell
		APrototypeCell* currentCell = Grid->GetCellElement(currentCellIndex);
		Grid->RemoveWallsBetweenCells(currentCell, neighborCell);

		// Mark the chosen cell as visited and push it to the stack
		neighborCell->bIsVisited = true;
		Grid->SetCellMaterial(neighborCell, visitedMaterial);

		// push neighbour to the stack to process neighbors.
		CellStack.Push(newNeighbor);
	}
	else
	{
		APrototypeCell* currentCell = Grid->GetCellElement(currentCellIndex);

		// Cell is a dead end, set backtracking flag. 
		currentCell->bIsDeadEnd = true;
		Grid->SetCellMaterial(currentCell, deadEndMaterial);
		if (!bCurrentlyBackTracking)
		{
			bCurrentlyBackTracking = true;
		}
	}
}


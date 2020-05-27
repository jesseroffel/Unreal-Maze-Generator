// Copyright 2020 Jesse Roffel

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrototypeCell.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class MAZETRAVERSAL_API APrototypeCell : public AActor
{
	GENERATED_BODY()
	
public:	
	APrototypeCell();

public:	
    
    UPROPERTY(EditAnywhere, Category = "Visuals")
        UStaticMeshComponent* CenterMesh;

    UPROPERTY(EditAnywhere, Category = "Generation Flags")
        bool bIsVisited = false;

    UPROPERTY(EditAnywhere, Category = "Generation Flags")
        bool bIsDeadEnd = false;

    UFUNCTION(BlueprintCallable, Category = "Cells")
        void RemoveInstanceStaticMeshIndex(UInstancedStaticMeshComponent* staticMeshComponent, int32 index);
};

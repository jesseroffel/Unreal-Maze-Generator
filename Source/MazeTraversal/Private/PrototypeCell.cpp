// Copyright 2020 Jesse Roffel


#include "PrototypeCell.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
APrototypeCell::APrototypeCell()
{
    CenterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell Shape"));

    // Find and use the startcontent sphere as default.
    UStaticMesh* Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'")).Object;
    if (Mesh != nullptr)
    {
        CenterMesh->SetStaticMesh(Mesh);
        CenterMesh->SetRelativeScale3D(FVector(0.5f)); 
    }
}


void APrototypeCell::RemoveInstanceStaticMeshIndex(UInstancedStaticMeshComponent* staticMeshComponent, int32 index)
{

    int32 wallCount = staticMeshComponent->GetInstanceCount();
    if (wallCount == 0 || index > wallCount)
    {
        return;
    }
    staticMeshComponent->RemoveInstance(index);
}


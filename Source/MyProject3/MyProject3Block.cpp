// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject3.h"
#include "MyProject3Block.h"
#include "MyProject3BlockGrid.h"

AMyProject3Block::AMyProject3Block()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> PinkMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, PinkMaterial(TEXT("/Game/Puzzle/Meshes/PinkMaterial.PinkMaterial"))
			, GreenMaterial(TEXT("/Game/Puzzle/Meshes/GreenMaterial.GreenMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.PinkMaterial.Get());
	BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMyProject3Block::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMyProject3Block::OnFingerPressedBlock);

	// Save a pointer to the orange material
	GreenMaterial = ConstructorStatics.GreenMaterial.Get();
    PinkMaterial = ConstructorStatics.PinkMaterial.Get();
}

void AMyProject3Block::BlockClicked(UPrimitiveComponent* ClickedComp)
{
    OwningGrid->toggleRecalScore(blockIDNumber);
}


void AMyProject3Block::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	BlockClicked(TouchedComponent);
}

void AMyProject3Block::toggleMesh(){
    // Check we are not already active
    if(!bIsActive)
    {
        bIsActive = true;
        
        // Change material
        BlockMesh->SetMaterial(0, GreenMaterial);
        OwningGrid->AddScore();
    }
    else
    {
        bIsActive = false;
        
        //change material
        BlockMesh->SetMaterial(0, PinkMaterial);
        OwningGrid->SubtractScore();
    }
}

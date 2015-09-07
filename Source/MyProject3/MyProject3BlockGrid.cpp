// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject3.h"
#include "MyProject3BlockGrid.h"
#include "MyProject3Block.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

// !!GLOBAL LIST OF BLOCKS STATES!! Initiallized in
//    AMyProject3BlocksGrid::BeginPlay()
struct blocksAll{
    bool state;
    AMyProject3Block *theBlock;
};
blocksAll *states;

AMyProject3BlockGrid::AMyProject3BlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
    ScoreText->XScale=5.0;
    ScoreText->YScale=5.0;
	ScoreText->SetRelativeLocation(FVector(0.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->AttachTo(DummyRoot);

	// Set defaults
	Size = 5;
	BlockSpacing = 300.f;
}


void AMyProject3BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;
    
    states = new blocksAll[Size*Size];

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AMyProject3Block* NewBlock = GetWorld()->SpawnActor<AMyProject3Block>(BlockLocation, FRotator(0,0,0));
        
        NewBlock->blockIDNumber=BlockIndex;

		// Tell the block about its owner
		if(NewBlock != NULL)
		{
			NewBlock->OwningGrid = this;
            
            states[BlockIndex].theBlock=NewBlock;
            states[BlockIndex].state=false;
		}
	}
}
void AMyProject3BlockGrid::toggleRecalScore(short int ID)
{
    states[ID].theBlock->toggleMesh();
    if(ID>4)
        states[ID-5].theBlock->toggleMesh();
    if(ID<20)
        states[ID+5].theBlock->toggleMesh();
    if(ID%5>0)
        states[ID-1].theBlock->toggleMesh();
    if(ID%5<4)
        states[ID+1].theBlock->toggleMesh();
    AMyProject3BlockGrid::checkWin();
}
void AMyProject3BlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AMyProject3BlockGrid::SubtractScore()
{
    // Decrement Score
    Score--;
    
    // Update text
    ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}
void AMyProject3BlockGrid::checkWin()
{
    if(Score>=25)
        ScoreText->SetText("WINNER");
}

#undef LOCTEXT_NAMESPACE

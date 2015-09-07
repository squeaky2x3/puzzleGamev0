// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject3.h"
#include "MyProject3GameMode.h"
#include "MyProject3PlayerController.h"

AMyProject3GameMode::AMyProject3GameMode()
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = AMyProject3PlayerController::StaticClass();
}

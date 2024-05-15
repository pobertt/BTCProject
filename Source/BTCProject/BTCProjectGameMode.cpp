// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTCProjectGameMode.h"
#include "BTCProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABTCProjectGameMode::ABTCProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

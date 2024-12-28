// Copyright Epic Games, Inc. All Rights Reserved.

#include "UTHUB_PercepcionGameMode.h"
#include "UTHUB_PercepcionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUTHUB_PercepcionGameMode::AUTHUB_PercepcionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

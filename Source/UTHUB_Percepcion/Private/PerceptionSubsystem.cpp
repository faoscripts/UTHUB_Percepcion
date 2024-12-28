#include "PerceptionSubsystem.h"
#include "PerceptionComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"

void UPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UPerceptionSubsystem::OnWorldInitialized);
}

void UPerceptionSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
}

void UPerceptionSubsystem::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (!World) return;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->FindComponentByClass<UPerceptionComponent>())
		{
			RegisterActor(Actor);
		}
	}
}

void UPerceptionSubsystem::RegisterActor(AActor* Actor)
{
	if (Actor && !RegisteredActors.Contains(Actor))
	{
		RegisteredActors.Add(Actor);

		Actor->OnDestroyed.RemoveDynamic(this, &UPerceptionSubsystem::HandleActorDestroyed);

		Actor->OnDestroyed.AddDynamic(this, &UPerceptionSubsystem::HandleActorDestroyed);

		UE_LOG(LogTemp, Warning, TEXT("Actor Registered: %s"), *Actor->GetName());
	}
}

void UPerceptionSubsystem::UnregisterActor(AActor* Actor)
{
	RegisteredActors.Remove(Actor);
}

void UPerceptionSubsystem::HandleActorDestroyed(AActor* Actor)
{
	UnregisterActor(Actor);
	UE_LOG(LogTemp, Warning, TEXT("Actor Destroyed: %s"), *Actor->GetName());
}

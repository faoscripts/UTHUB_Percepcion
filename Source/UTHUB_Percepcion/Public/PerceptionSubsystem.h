#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PerceptionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGlobalActorDetected, AActor*, Detector, AActor*, Detected);

UCLASS()
class UTHUB_PERCEPCION_API UPerceptionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	TArray<AActor*> RegisteredActors;

	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnGlobalActorDetected OnGlobalActorDetected;

	void RegisterActor(AActor* Actor);

	void UnregisterActor(AActor* Actor);

private:
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	UFUNCTION()
	void HandleActorDestroyed(AActor* Actor);
};

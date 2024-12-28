#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDetected, AActor*, DetectedActor);

USTRUCT(BlueprintType)
struct FPerceptionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DetectionRadius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DetectionInterval = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive = true;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTHUB_PERCEPCION_API UPerceptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPerceptionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Debug")
    bool bShowLogs = true;
    
    UPROPERTY(BlueprintAssignable, Category = "Perception")
    FOnActorDetected OnActorDetected;

    UFUNCTION(BlueprintCallable, Category = "Perception")
    void InitPerceptionInfo(float NewDetectionRadius, float NewDetectionInterval, bool bNewIsActive);

    UFUNCTION(BlueprintCallable, Category = "Perception")
    void SetPerceptionActive(bool bActive);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Perception")
    FPerceptionInfo PerceptionInfo;

    FTimerHandle DetectionTimerHandle;

    TArray<AActor*> DetectedActors;

    void PerformDetection();
};
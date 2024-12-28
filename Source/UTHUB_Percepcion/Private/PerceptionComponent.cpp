#include "PerceptionComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

UPerceptionComponent::UPerceptionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (PerceptionInfo.bIsActive)
    {
        GetWorld()->GetTimerManager().SetTimer(
            DetectionTimerHandle,
            this,
            &UPerceptionComponent::PerformDetection,
            PerceptionInfo.DetectionInterval,
            true
        );
    }
}

void UPerceptionComponent::InitPerceptionInfo(float NewDetectionRadius, float NewDetectionInterval, bool bNewIsActive)
{
    PerceptionInfo.DetectionRadius = NewDetectionRadius;
    PerceptionInfo.DetectionInterval = NewDetectionInterval;
    PerceptionInfo.bIsActive = bNewIsActive;

    if (bNewIsActive)
    {
        GetWorld()->GetTimerManager().ClearTimer(DetectionTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(
            DetectionTimerHandle,
            this,
            &UPerceptionComponent::PerformDetection,
            PerceptionInfo.DetectionInterval,
            true
        );
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(DetectionTimerHandle);
    }
}

void UPerceptionComponent::SetPerceptionActive(bool bActive)
{
    InitPerceptionInfo(PerceptionInfo.DetectionRadius, PerceptionInfo.DetectionInterval, bActive);
}

void UPerceptionComponent::PerformDetection()
{
    FVector OwnerLocation = GetOwner()->GetActorLocation();
    DrawDebugSphere(GetWorld(), OwnerLocation, PerceptionInfo.DetectionRadius, 12, FColor::Green, false, 0.5f);

    TArray<FHitResult> HitResults;
    FCollisionShape DetectionSphere = FCollisionShape::MakeSphere(PerceptionInfo.DetectionRadius);

    TArray<AActor*> CurrentlyDetectedActors;

    if (GetWorld()->SweepMultiByObjectType(
        HitResults,
        OwnerLocation,
        OwnerLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_WorldDynamic),
        DetectionSphere))
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* DetectedActor = Hit.GetActor();
            if (DetectedActor && DetectedActor != GetOwner() && DetectedActor->FindComponentByClass<UPerceptionComponent>())
            {
                CurrentlyDetectedActors.Add(DetectedActor);

                if (!DetectedActors.Contains(DetectedActor))
                {
                    DetectedActors.Add(DetectedActor);
                    OnActorDetected.Broadcast(DetectedActor);
                    if (bShowLogs)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Detected Actor: %s"), *DetectedActor->GetName());
                    }
                }
            }
        }
    }

    for (int32 i = DetectedActors.Num() - 1; i >= 0; --i)
    {
        AActor* PreviouslyDetectedActor = DetectedActors[i];
        if (!CurrentlyDetectedActors.Contains(PreviouslyDetectedActor))
        {
            DetectedActors.RemoveAt(i);
            if (bShowLogs)
            {
                UE_LOG(LogTemp, Warning, TEXT("Lost Detection: %s"), *PreviouslyDetectedActor->GetName());
            }
        }
    }

    if (CurrentlyDetectedActors.Num() == 0 && bShowLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("No actors detected in radius."));
    }
}

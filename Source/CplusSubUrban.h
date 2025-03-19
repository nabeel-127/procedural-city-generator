#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralSuburbanGenerator.generated.h"

UCLASS()
class YOURPROJECT_API AProceduralSuburbanGenerator : public AActor
{
    GENERATED_BODY()

public:
    AProceduralSuburbanGenerator();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Suburban-specific functions
    FVector GetLandscapeData();
    FVector SurfaceSampling();
    float AttributingNoise(const FVector& Point);
    bool DensityFilter(const FVector& Point, float Density);
    FTransform TransformPoints(const FVector& Point);

    // Custom helper functions
    ALandscape* FindLandscape();
    bool IsFlatEnough(const FVector& Point);
    float GetDensity(const FVector& Point);
    void PlaceBuilding(const FTransform& BuildingTransform);
};

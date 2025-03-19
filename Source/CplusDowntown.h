#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralCityGenerator.generated.h"

UCLASS()
class CplusDowntown AProceduralCityGenerator : public AActor
{
    GENERATED_BODY()

public:
    AProceduralCityGenerator();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Function declarations
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

    UPROPERTY(EditAnywhere, Category = "Buildings")
    TSubclassOf<AActor> BuildingClass;  // To spawn buildings
};

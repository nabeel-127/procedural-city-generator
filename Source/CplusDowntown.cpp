#include "ProceduralCityGenerator.h"
#include "Landscape.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"

AProceduralCityGenerator::AProceduralCityGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AProceduralCityGenerator::BeginPlay()
{
    Super::BeginPlay();

    FVector LandscapeData = GetLandscapeData();

    for (int i = 0; i < 1000; ++i)
    {
        FVector SampledPoint = SurfaceSampling();
        if (IsFlatEnough(SampledPoint))
        {
            float NoiseValue = AttributingNoise(SampledPoint);
            float Density = GetDensity(SampledPoint);

            if (DensityFilter(SampledPoint, Density))
            {
                FTransform BuildingTransform = TransformPoints(SampledPoint);
                PlaceBuilding(BuildingTransform);
            }
        }
    }
}

void AProceduralCityGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

FVector AProceduralCityGenerator::GetLandscapeData()
{
    ALandscape* Landscape = FindLandscape();
    if (Landscape)
    {
        return Landscape->GetActorLocation();
    }
    return FVector::ZeroVector;
}

FVector AProceduralCityGenerator::SurfaceSampling()
{
    return FVector(FMath::RandRange(-1000.0f, 1000.0f), FMath::RandRange(-1000.0f, 1000.0f), 0);
}

float AProceduralCityGenerator::AttributingNoise(const FVector& Point)
{
    return FMath::PerlinNoise2D(FVector2D(Point.X, Point.Y));
}

bool AProceduralCityGenerator::DensityFilter(const FVector& Point, float Density)
{
    return Density > 0.5f;
}

FTransform AProceduralCityGenerator::TransformPoints(const FVector& Point)
{
    FTransform Transform;
    Transform.SetLocation(Point);
    Transform.SetRotation(FQuat::Identity);
    Transform.SetScale3D(FVector(1.0f));
    return Transform;
}

ALandscape* AProceduralCityGenerator::FindLandscape()
{
    for (TActorIterator<ALandscape> It(GetWorld()); It; ++It)
    {
        return *It;
    }
    return nullptr;
}

bool AProceduralCityGenerator::IsFlatEnough(const FVector& Point)
{
    return true;
}

float AProceduralCityGenerator::GetDensity(const FVector& Point)
{
    return FMath::PerlinNoise2D(FVector2D(Point.X, Point.Y));
}

void AProceduralCityGenerator::PlaceBuilding(const FTransform& BuildingTransform)
{
    if (BuildingClass)
    {
        GetWorld()->SpawnActor<AActor>(BuildingClass, BuildingTransform);
    }
}

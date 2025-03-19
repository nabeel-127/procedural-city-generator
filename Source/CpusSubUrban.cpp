#include "ProceduralSuburbanGenerator.h"
#include "Landscape.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Constructor
AProceduralSuburbanGenerator::AProceduralSuburbanGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
}

// BeginPlay
void AProceduralSuburbanGenerator::BeginPlay()
{
    Super::BeginPlay();

    // Get landscape data to inform the suburban layout
    FVector LandscapeData = GetLandscapeData();

    // Sample surface points and generate suburban buildings
    for (int i = 0; i < 500; ++i) // Fewer iterations for suburban area (less dense)
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

// Tick
void AProceduralSuburbanGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// GetLandscapeData
FVector AProceduralSuburbanGenerator::GetLandscapeData()
{
    ALandscape* Landscape = FindLandscape();
    if (Landscape)
    {
        return Landscape->GetActorLocation();
    }
    return FVector::ZeroVector;
}

// SurfaceSampling
FVector AProceduralSuburbanGenerator::SurfaceSampling()
{
    return FVector(FMath::RandRange(-2000.0f, 2000.0f), FMath::RandRange(-2000.0f, 2000.0f), 0);
}

// AttributingNoise
float AProceduralSuburbanGenerator::AttributingNoise(const FVector& Point)
{
    return FMath::PerlinNoise2D(FVector2D(Point.X * 0.5f, Point.Y * 0.5f)); // Adjust noise scale for suburban area
}

// DensityFilter
bool AProceduralSuburbanGenerator::DensityFilter(const FVector& Point, float Density)
{
    // Suburban areas are less dense, so we use a different threshold
    return Density > 0.3f; // Lower threshold for suburban placement
}

// TransformPoints
FTransform AProceduralSuburbanGenerator::TransformPoints(const FVector& Point)
{
    FTransform Transform;
    Transform.SetLocation(Point);
    Transform.SetRotation(FQuat::Identity);
    Transform.SetScale3D(FVector(0.8f)); // Slightly smaller buildings for suburban area
    return Transform;
}

// Helper Functions

// Find the landscape in the scene
ALandscape* AProceduralSuburbanGenerator::FindLandscape()
{
    for (TActorIterator<ALandscape> It(GetWorld()); It; ++It)
    {
        return *It;
    }
    return nullptr;
}

// Check if a sampled point is flat enough for building placement
bool AProceduralSuburbanGenerator::IsFlatEnough(const FVector& Point)
{
    return true;
}

// Get density based on suburban characteristics
float AProceduralSuburbanGenerator::GetDensity(const FVector& Point)
{
    // Suburban areas are less dense, so the noise value is scaled
    return FMath::PerlinNoise2D(FVector2D(Point.X * 0.5f, Point.Y * 0.5f));
}

// Place a building at the given transform
void AProceduralSuburbanGenerator::PlaceBuilding(const FTransform& BuildingTransform)
{
    GetWorld()->SpawnActor<AActor>(BuildingClass, BuildingTransform);
}

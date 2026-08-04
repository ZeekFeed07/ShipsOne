#pragma once

#include "CoreMinimal.h"

enum class EShipDirection : uint8
{
    TOP = 0,
    RIGHT = 1,
    BOTTOM = 2,
    LEFT = 3
};

enum class EShotOutcome : uint8
{
    Miss,
    Hit,
    Kill,
    Invalid
};

struct FRulesConfig
{
    int32 FieldSizeX = 10;
    int32 FieldSizeY = 10;
    TMap<int32 /*длина в клетках*/, int32 /*количество*/> Fleet;
};

struct FShipModel
{
    int32 X, Y;
    EShipDirection Dir;
    int32 Length;
};

class FGridModel
{
public:
    FGridModel();
public:
    bool CanPlaceShip(int32 X, int32 Y, EShipDirection Dir, int32 Length) const;
//    bool PlaceShip(int32 X, int32 Y, EShipDirection Dir, int32 Length);
//    bool ValidateFullLayout(const TArray<FShipModel>& Layout) const; // состав флота + пересечения + касания
//    EShotOutcome ApplyShot(int32 X, int32 Y, FShipModel*& OutKilledShip);
//    bool AllShipsSunk() const;
//    bool PlaceFleetRandom(FRandomStream& Rng);

public:
    FRulesConfig Rules;
    TArray<int32> Occupancy;
    TArray<bool>   ShotMask;
    TArray<FShipModel> Ships;
};
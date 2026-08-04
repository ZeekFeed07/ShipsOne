#include "GridModel.h"

FGridModel::FGridModel()
{
	if (Rules.FieldSizeX <= 0 || Rules.FieldSizeY <= 0) return;

	Occupancy.SetNum(Rules.FieldSizeX * Rules.FieldSizeY);
}

bool FGridModel::CanPlaceShip(int32 X, int32 Y, EShipDirection Dir, int32 Length) const
{
	if (X >= Rules.FieldSizeX || X < 0 || Y >= Rules.FieldSizeY || Y < 0) return false;

	return false;
}

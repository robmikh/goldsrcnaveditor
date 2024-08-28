#pragma once

#ifndef NAV_CONSTANTS_H
#define NAV_CONSTANTS_H

// Possible movement types. Defines the actions the bot needs to take to traverse this node
enum NavMovementFlag
{
	NAV_FLAG_DISABLED = 2147483647,		// Disabled
	NAV_FLAG_WALK = 1,		// Walk
	NAV_FLAG_CROUCH = 2,		// Crouch
	NAV_FLAG_JUMP = 4,		// Jump
	NAV_FLAG_LADDER = 8,		// Ladder
	NAV_FLAG_FALL = 16,		// Fall
	NAV_FLAG_PLATFORM = 32,		// Platform
	NAV_FLAG_DAMAGEJUMP = 64,		// Damaging Jump
	NAV_FLAG_DAMAGEFALL = 128,		// Damaging Fall
	NAV_FLAG_WALLCLIMB = 256,		// Wall Climb
	NAV_FLAG_WELD = 512,		// Weld
	NAV_FLAG_PHASEGATE1 = 1024,		// Team 1 Phase Gate
	NAV_FLAG_PHASEGATE2 = 2048,		// Team 2 Phase Gate
	NAV_FLAG_FLY = 4096,		// Fly
	NAV_FLAG_STRUCTURE1 = 8192,		// Team 1 Structure
	NAV_FLAG_STRUCTURE2 = 16384,		// Team 2 Structure
	NAV_FLAG_DUCKJUMP = 32768,		// Duck Jump
	NAV_FLAG_ALL = -1		// All flags
}

// Area types. Defines the cost of movement through an area and which flag to use
enum NavArea
{
	NAV_AREA_UNWALKABLE = 60,		// Unwalkable
	NAV_AREA_WALK = 1,		// Walk
	NAV_AREA_CROUCH = 2,		// Crouch
	NAV_AREA_HAZARD = 3,		// Hazard
	NAV_AREA_FALLDAMAGE = 4,		// Fall Damage
	NAV_AREA_BLOCKEDSTRUCTURE1 = 5,		// Blocked By Team 1 Structure
	NAV_AREA_BLOCKEDSTRUCTURE2 = 6,		// Blocked By Team 2 Structure
}

// Retrieve appropriate flag for area (See process() in the MeshProcess struct)
inline NavMovementFlag GetFlagForArea(NavArea Area)
{
	switch(Area)
	{
		case NAV_AREA_UNWALKABLE:
			return NAV_FLAG_DISABLED;
		case NAV_AREA_WALK:
			return NAV_FLAG_WALK;
		case NAV_AREA_CROUCH:
			return NAV_FLAG_CROUCH;
		case NAV_AREA_HAZARD:
			return NAV_FLAG_WALK;
		case NAV_AREA_FALLDAMAGE:
			return NAV_FLAG_FALL;
		case NAV_AREA_BLOCKEDSTRUCTURE1:
			return NAV_FLAG_STRUCTURE1;
		case NAV_AREA_BLOCKEDSTRUCTURE2:
			return NAV_FLAG_STRUCTURE2;
		default:
			return NAV_FLAG_DISABLED;
	}
}

#endif // NAV_CONSTANTS_H
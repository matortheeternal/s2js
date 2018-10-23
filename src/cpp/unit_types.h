#ifndef S2JS_UNIT_TYPES
#define S2JS_UNIT_TYPES

#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"

namespace Util {
	enum UNIT_TYPE : uint8_t {
		MINERAL,
		GEYSER,
		WORKER,
		STRUCTURE,
		AIR,
		GROUND,
		TOWN_HALL,
		PROVIDES_FOOD,
		GAS_STRUCTURE,
		ATTACKS_GROUND,
		ATTACKS_AIR,
		TRAINS_UNITS,
		UPGRADES_UNITS,
		TRANSPORTS_UNITS,
		SUPPORTS_UNITS,
		SPAWNING,
		CLOAKING,
		DETECTOR,
		BOMB
	};
  
  uint8_t GetResourceType(const sc2::Unit* unit);
	std::vector<uint8_t> GetUnitTypes(const sc2::Unit* unit);
}

#endif
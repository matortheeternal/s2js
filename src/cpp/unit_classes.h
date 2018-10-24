#ifndef S2JS_UNIT_CLASSES
#define S2JS_UNIT_CLASSES

#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"

namespace Util {
	enum UNIT_CLASS : uint8_t {
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
  
    uint8_t GetResourceClass(const sc2::Unit* unit);
    std::vector<uint8_t> GetUnitClasses(const sc2::Unit* unit);
}

#endif
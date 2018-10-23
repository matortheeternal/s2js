#ifndef S2JS_API
#define S2JS_API

#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"

#include <nan.h>

class Bot : public sc2::Agent {};

// NAN METHODS
NAN_METHOD(StartGame);
NAN_METHOD(Update);
NAN_METHOD(GetStats);
NAN_METHOD(GetUnits);
NAN_METHOD(GetUnitTypes);
NAN_METHOD(CommandUnit);

#endif
#include "api.h"

using v8::FunctionTemplate;

// This represents the top level of the module.
// C++ constructs that are exposed to javascript are exported here

NAN_MODULE_INIT(InitAll) {
    NAN_EXPORT(target, StartGame);
    NAN_EXPORT(target, Update);
    NAN_EXPORT(target, GetStats);
    NAN_EXPORT(target, GetUnits);
    NAN_EXPORT(target, GetUnitTypes);
    NAN_EXPORT(target, CommandUnit);
}

NODE_MODULE(s2js, InitAll)
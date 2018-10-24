#ifndef S2JS_API
#define S2JS_API

#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"

#include <nan.h>

class Bot : public sc2::Agent {
public:
	void OnGameStart();
	void OnError(const std::vector<sc2::ClientError>& client_errors, const std::vector<std::string>& protocol_errors = {});
};

// NAN METHODS
NAN_METHOD(AddBot);
NAN_METHOD(AddComputer);
NAN_METHOD(StartGame);
NAN_METHOD(GetGameStatus);
NAN_METHOD(Update);
NAN_METHOD(GetStats);
NAN_METHOD(GetUnits);
NAN_METHOD(GetUnitTypes);
NAN_METHOD(GetUnitInfo);
NAN_METHOD(GetResourceInfo);
NAN_METHOD(CommandUnit);

#endif
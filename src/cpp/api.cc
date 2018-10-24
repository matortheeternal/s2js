#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"
#include <stdlib.h>
#include "api.h"
#include "unit_types.h"
#include <nan.h>

using namespace sc2;

Coordinator coordinator;
std::vector<PlayerSetup> participants;
//std::map<std::string, Bot> bots;
Bot bot;
std::string status = "Initializing";

void Bot::OnGameStart() {
	status = "Game started";
}

std::string ClientErrorStr(ClientError err) {
	switch (err) {
		case ClientError::ConnectionClosed:		return "ConnectionClosed";
		case ClientError::ErrorSC2:				return "ErrorSC2";
		case ClientError::InvalidAbilityRemap:	return "ErrorSC2";
		case ClientError::InvalidResponse:		return "InvalidResponse";
		case ClientError::NoAbilitiesForTag:	return "NoAbilitiesForTag";
		case ClientError::ResponseMismatch: 	return "ResponseMismatch";
		case ClientError::ResponseNotConsumed:	return "ResponseNotConsumed";
		case ClientError::SC2AppFailure:		return "SC2AppFailure";
		case ClientError::SC2ProtocolError:		return "SC2ProtocolError";
	}
}

void Bot::OnError(const std::vector<ClientError>& client_errors, const std::vector<std::string>& protocol_errors) {
	status = "";
	for (int i = 0; i < client_errors.size(); i++) {
		ClientError err = client_errors.at(i);
		status += "Client Error: " + ClientErrorStr(err) + "\n";
	}
	for (int i = 0; i < protocol_errors.size(); i++) {
		std::string err = protocol_errors.at(i);
		status += "Protocol Error: " + err + "\n";
	}
}

v8::Local<v8::String> nstr(const char* str) {
	return Nan::New(str).ToLocalChecked();
}

const char* cstr(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

NAN_METHOD(AddBot) {
	//v8::String::Utf8Value botKey(info[0]->ToString());
	//bots[(std::string)(botKey)] = bot;
	Race race = static_cast<Race>(info[0]->Uint32Value());
	participants.push_back(CreateParticipant(race, &bot));
}

NAN_METHOD(AddComputer) {
	Race race = static_cast<Race>(info[0]->Uint32Value());
	participants.push_back(CreateComputer(race));
}

NAN_METHOD(StartGame) {
	try {
		// coordinate game
		char* argv[1]{ "" };
		coordinator.LoadSettings(1, argv);
		coordinator.SetParticipants(participants);

		// start game
		v8::String::Utf8Value mapPath(info[0]);
		coordinator.LaunchStarcraft();
		coordinator.StartGame(cstr(mapPath));
	} catch (const std::exception& x) {
		status = x.what();
	}
}

NAN_METHOD(GetGameStatus) {
	info.GetReturnValue().Set(nstr(status.c_str()));
}

NAN_METHOD(Update) {
	bool updated = coordinator.Update();
	info.GetReturnValue().Set(Nan::New(updated));
}

NAN_METHOD(GetStats) {
	const ObservationInterface* o = bot.Observation();
	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	Nan::Set(obj, nstr("minerals"), Nan::New(o->GetMinerals()));
	Nan::Set(obj, nstr("vespene"), Nan::New(o->GetVespene()));
	Nan::Set(obj, nstr("food_cap"), Nan::New(o->GetFoodCap()));
	Nan::Set(obj, nstr("food_used"), Nan::New(o->GetFoodUsed()));
	Nan::Set(obj, nstr("food_army"), Nan::New(o->GetFoodArmy()));
	Nan::Set(obj, nstr("food_workers"), Nan::New(o->GetFoodWorkers()));
	Nan::Set(obj, nstr("idle_workers"), Nan::New(o->GetIdleWorkerCount()));
	Nan::Set(obj, nstr("army_count"), Nan::New(o->GetArmyCount()));
	Nan::Set(obj, nstr("warp_gate_count"), Nan::New(o->GetWarpGateCount()));
	info.GetReturnValue().Set(obj);
}

std::string TagToStr(Tag tag) {
	char buffer[_MAX_U64TOSTR_BASE16_COUNT];
	_ui64toa(tag, buffer, 16);
	std::string str(buffer);
	return str;
}

NAN_METHOD(GetUnits) {
	const ObservationInterface* o = bot.Observation();
	Units units = o->GetUnits();
	v8::Local<v8::Array> a = Nan::New<v8::Array>(units.size());
	for (int i = 0; i < units.size(); i++) {
		std::string tstr = TagToStr(units.at(i)->tag);
		Nan::Set(a, i, nstr(tstr.c_str()));
	}
	info.GetReturnValue().Set(a);
}

Tag StrToTag(char* str) {
	return strtoull(str, NULL, 16);
}

NAN_METHOD(GetUnitTypes) {
	v8::String::Utf8Value str(info[0]);
	Tag tag = StrToTag((char*)(*str));
	const ObservationInterface* o = bot.Observation();
	const Unit* unit = o->GetUnit(tag);
	std::vector<uint8_t> unitTypes = Util::GetUnitTypes(unit);
	v8::Local<v8::Array> a = Nan::New<v8::Array>(unitTypes.size());
	for (int i = 0; i < unitTypes.size(); i++) {
		Nan::Set(a, i, Nan::New(unitTypes.at(i)));
	}
	info.GetReturnValue().Set(a);
}

v8::Local<v8::Object> GetPosition(const Unit* unit) {
	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	Nan::Set(obj, nstr("x"), Nan::New(unit->pos.x));
	Nan::Set(obj, nstr("y"), Nan::New(unit->pos.y));
	Nan::Set(obj, nstr("z"), Nan::New(unit->pos.z));
	return obj;
}

NAN_METHOD(GetUnitInfo) {
	v8::String::Utf8Value str(info[0]);
	Tag tag = StrToTag((char*)(*str));
	const ObservationInterface* o = bot.Observation();
	const Unit* unit = o->GetUnit(tag);
	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	Nan::Set(obj, nstr("health"), Nan::New(unit->health));
	Nan::Set(obj, nstr("health_max"), Nan::New(unit->health_max));
	Nan::Set(obj, nstr("type"), Nan::New(unit->unit_type));
	Nan::Set(obj, nstr("pos"), GetPosition(unit));
	Nan::Set(obj, nstr("alliance"), Nan::New(unit->alliance));
	if (unit->shield_max > 0) {
		Nan::Set(obj, nstr("shield"), Nan::New(unit->shield));
		Nan::Set(obj, nstr("shield_max"), Nan::New(unit->shield_max));
	}
	if (unit->energy_max > 0) {
		Nan::Set(obj, nstr("energy"), Nan::New(unit->energy));
		Nan::Set(obj, nstr("energy_max"), Nan::New(unit->energy_max));
	}
	if (unit->alliance == Unit::Alliance::Enemy) {
		Nan::Set(obj, nstr("display"), Nan::New(unit->display_type));
		Nan::Set(obj, nstr("cloak"), Nan::New(unit->cloak));
	}
	info.GetReturnValue().Set(obj);
}

NAN_METHOD(GetResourceInfo) {
	v8::String::Utf8Value str(info[0]);
	Tag tag = StrToTag((char*)(*str));
	const ObservationInterface* o = bot.Observation();
	const Unit* unit = o->GetUnit(tag);
	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	Nan::Set(obj, nstr("pos"), GetPosition(unit));
	uint8_t resourceType = Util::GetResourceType(unit);
	if (resourceType == Util::MINERAL) {
		Nan::Set(obj, nstr("contents"), Nan::New(unit->mineral_contents));
	} else if (resourceType == Util::GEYSER) {
		Nan::Set(obj, nstr("contents"), Nan::New(unit->vespene_contents));
	}
	info.GetReturnValue().Set(obj);
}

NAN_METHOD(CommandUnit) {
	v8::String::Utf8Value str(info[0]);
	Tag tag = StrToTag((char*)(*str));
	const ObservationInterface* o = bot.Observation();
	const Unit* unit = o->GetUnit(tag);
	ABILITY_ID ability_id = static_cast<ABILITY_ID>(info[1]->Uint32Value());
	if (info.Length() == 3) {
		if (info[2]->IsString()) {
			v8::String::Utf8Value targetStr(info[2]);
			Tag targetTag = StrToTag((char*)(*targetStr));
			const Unit* target = o->GetUnit(targetTag);
			bot.Actions()->UnitCommand(unit, ability_id, target);
		} else {
			v8::Local<v8::Object> pos(info[2]->ToObject());
			float x = pos->Get(nstr("x"))->NumberValue();
			float y = pos->Get(nstr("y"))->NumberValue();
			bot.Actions()->UnitCommand(unit, ability_id, Point2D{ x, y });
		}
	} else {
		bot.Actions()->UnitCommand(unit, ability_id);
	}
}
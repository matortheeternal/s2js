let s2js = require('../index');
let {UNIT_TYPES, ABILITIES} = s2js;

s2js.StartGame({
    participants: [{
        type: 'bot',
        race: s2js.RACES.Terran
    }, {
        type: 'computer',
        race: s2js.RACES.Zerg
    }]
});

let updateCount = 0;
let allUnits = {};
let stats = {};

let units = {
    MINERAL: [],
    WORKER: [],
    TOWN_HALL: []
};

let updateStats = function() {
    stats = s2js.GetStats();
};

let isResourceUnit = function(unit) {
    return unit.types.includes(UNIT_TYPES.MINERAL) ||
        unit.types.includes(UNIT_TYPES.GEYSER);
};

let getUnitInfo = function(unit) {
    return isResourceUnit(unit) ?
        s2js.GetResourceInfo(unit.tag) :
        s2js.GetUnitInfo(unit.tag);
};

let buildUnit = function(tag) {
    let types = s2js.GetUnitTypes(tag),
        unit = { tag, types };
    return Object.assign(unit, getUnitInfo(unit));
};

let updateUnit = function(tag) {
    let unit = allUnits[tag];
    return Object.assign(unit, getUnitInfo(unit));
};

let updateUnits = function() {
    allUnits = s2js.GetUnits().reduce((newUnits, tag) => {
        let unitCreated = !allUnits.hasOwnProperty(tag);
        newUnits[tag] = unitCreated ? buildUnit(tag) : updateUnit(tag);
        return newUnits;
    }, {});
};

let storeUnit = function(unit) {
    unit.types.forEach(unitTypeId => {
        let unitType = s2js.UNIT_TYPE[unitTypeId],
            targetArray = units[unitType];
        if (!targetArray) return;
        targetArray.push(unit);
    });
};

let storeUnits = function() {
    Object.keys(units).forEach(key => {
        units[key] = [];
    });
    Object.values(allUnits).forEach(storeUnit)
};

let removeEntry = function(entry, a) {
    let index = a.indexOf(entry);
    a.splice(index, 1);
};

let getDistance = function(u1, u2) {
    return Math.sqrt(
        Math.pow(u2.pos.x - u1.pos.x, 2) +
        Math.pow(u2.pos.y - u1.pos.y, 2) +
        Math.pow(u2.pos.z - u1.pos.z, 2)
    );
};

let getClosestUnit = function(a, source) {
    let closestDistance = 9999999;
    return a.reduce((target, unit) => {
        let d = getDistance(unit, source);
        if (d < closestDistance) {
            closestDistance = d;
            return unit;
        }
        return target;
    }, null);
};

let mineMinerals = function() {
    let availableMinerals = units.MINERAL.filter(m => !m.worker),
        availableWorkers = units.WORKER.filter(w => !w.mineral);
    availableWorkers.forEach(worker => {
        if (worker.mineral) return;
        let mineral = getClosestUnit(availableMinerals, worker);
        removeEntry(mineral, availableMinerals);
        mineral.worker = worker;
        worker.mineral = mineral;
        s2js.CommandUnit(worker.tag, ABILITIES.HARVEST_GATHER_SCV, mineral.tag);
    });
};

let trainWorkers = function() {
    let mainBase = units.TOWN_HALL[0];
    for (let i = stats.minerals; i >= 50; i -= 50) {
        s2js.CommandUnit(mainBase.tag, ABILITIES.TRAIN_SCV);
    }
};

let executeActions = function() {
    mineMinerals();
    trainWorkers();
};

while (s2js.Update()) {
    updateCount++;
    if (updateCount % 5 !== 1) continue;
    updateStats();
    updateUnits();
    storeUnits();
    executeActions();
}


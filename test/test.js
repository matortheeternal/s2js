let s2js = require('../index');
let {UNIT_CLASSES, UNIT_CLASS_IDS, ABILITY_IDS, RACE_IDS} = s2js;
let mapPath = `${__dirname}\\BelShirVestigeLE.SC2Map`;

s2js.StartGame({
    participants: [{
        type: 'bot',
        race: RACE_IDS.Terran
    }, {
        type: 'computer',
        race: RACE_IDS.Zerg
    }],
    map: mapPath
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
    return unit.classes.includes(UNIT_CLASS_IDS.MINERAL) ||
        unit.classes.includes(UNIT_CLASS_IDS.GEYSER);
};

let getUnitInfo = function(unit) {
    return isResourceUnit(unit) ?
        s2js.GetResourceInfo(unit.tag) :
        s2js.GetUnitInfo(unit.tag);
};

let buildUnit = function(tag) {
    let classes = s2js.GetUnitClasses(tag),
        unit = { tag, classes };
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
    unit.classes.forEach(unitTypeId => {
        let unitClass = UNIT_CLASSES[unitTypeId],
            targetArray = units[unitClass];
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

let getMineral = function(source, maxWorkers) {
    let closestDistance = 9999999;
    return units.MINERAL.reduce((target, unit) => {
        if (unit.workers && unit.workers.length >= maxWorkers)
            return target;
        let d = getDistance(unit, source);
        if (d < closestDistance) {
            closestDistance = d;
            return unit;
        }
        return target;
    }, null);
};

let harvestMinerals = function(worker, mineral) {
    if (!mineral.workers) mineral.workers = [];
    mineral.workers.push(worker);
    worker.mineral = mineral;
    s2js.CommandUnit(worker.tag, ABILITY_IDS.HARVEST_GATHER_SCV, mineral.tag);
};

let mineMinerals = function() {
    let availableWorkers = units.WORKER.filter(w => !w.mineral);
    availableWorkers.forEach(worker => {
        if (worker.mineral) return;
        let mineral = getMineral(worker, 2);
        harvestMinerals(worker, mineral);
    });
};

let trainWorkers = function() {
    let mainBase = units.TOWN_HALL[0];
    for (let i = stats.minerals; i >= 50; i -= 50) {
        s2js.CommandUnit(mainBase.tag, ABILITY_IDS.TRAIN_SCV);
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
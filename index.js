let lib = require('bindings')('s2js');
let { applyEnums } = require('./src/js/helpers');

let s2js = {};

applyEnums(s2js, require('./src/js/races'), 'RACES');
applyEnums(s2js, require('./src/js/units'), 'UNITS');
applyEnums(s2js, require('./src/js/abilities'), 'ABILITIES', 'ABILITY_IDS');
applyEnums(s2js, require('./src/js/buffs'), 'BUFFS');
applyEnums(s2js, require('./src/js/upgrades'), 'UPGRADES');
applyEnums(s2js, require('./src/js/unitTypes'), 'UNIT_TYPES');

let addParticipant = {
    bot: lib.AddBot,
    computer: lib.AddComputer
};

Object.assign(s2js, {
    StartGame: function(options) {
        options.participants.forEach(p => {
            addParticipant[p.type](p.race);
        });
        lib.StartGame(options.map);
    },
    Update: lib.Update,
    GetStats: lib.GetStats,
    GetUnits: lib.GetUnits,
    GetUnitTypes: lib.GetUnitTypes,
    GetUnitInfo: lib.GetUnitInfo,
    GetResourceInfo: lib.GetResourceInfo,
    CommandUnit: lib.CommandUnit
});

module.exports = s2js;

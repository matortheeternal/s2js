# s2js
Native node addon which wraps around [s2client-api](https://github.com/blizzard/s2client-api).

## Usage
```
npm install matortheeternal/s2js --save
```

See [the test bot](https://github.com/matortheeternal/test/test.js) for an example.

## Plans
- Create a fleshed-out bot using the API and expand the API functionality to match its needs.
- Build a system to run multiple JS bots against each other.
- Build a system to run a JS bot against a C++ bot.

## API

### Enumerations
Every enumeration in the API has both an ID to name map and a name to ID map.  The `*_IDS` object maps from names to IDs, where the `*` object maps from IDs to names.

* `UNIT_TYPES`, `UNIT_TYPE_IDS`
* `UNIT_CLASSES`, `UNIT_CLASS_IDS`
* `ABILITIES`, `ABILITY_IDS`
* `BUFFS`, `BUFF_IDS`
* `RACES`, `RACE_IDS`
* `UPGRADES`, `UPGRADE_IDS`
* `ALLIANCES`, `ALLIANCE_IDS`
* `DISPLAY_TYPES`, `DISPLAY_TYPE_IDS`
* `CLOAK_STATES`, `CLOAK_STATE_IDS`

**examples:**  
- `UNIT_TYPE_IDS.TERRAN_SCV` - The unique ID for Terran SCVs.
- `UNIT_TYPES[45]` - The name of the unit with ID 45.
- `ABILITY_IDS.TRAIN_MARINE` - The unique ID for the train marine ability.
- `RACE_IDS.Terran` - The unique ID for the Terran race.

### StartGame(options)
Starts a new StarCraft 2 game.

**arguments:**  
`options` - Required options to start the game.
* `participants` - Array of participants in the game.
   * `type` - String for the type of participant: 'bot' or 'computer'.
   * `race` - RACE_ID to use for the participant.  E.g. `RACE_IDS.Zerg`.
* `map` - Path to the map file to use.  Using an absolute path is recommended.

**returns:**  
Nothing.

### GetGameStatus()
Gets the current status of the game.

**returns:**  
* `'Initializing'` - The game hasn't started yet.
* `'Game started'` - The game has been started and you can now use `Update()` and start controlling your bot.
* Any other value - An error has occurred.  The status returned is the error message.


### Update()
Runs the game loop.

**returns:**  
False if the game has ended, true otherwise. 

### GetStats()
Gets game stats for the bot player.

**returns:**
An object with the following properties:
* `minerals` - minerals
* `vespene` - vespene gas
* `food_cap` - supply cap
* `food_used` - supply used
* `food_army` - supply used by army units
* `food_workers` - supply used by worker units
* `idle_workers` - number of idle workers
* `army_count` - number of army units

### GetUnits()
Gets an array of unique IDs for all the units the bot player has vision of.

**returns:**  
An array of strings, where each string is a hexadecimal ID corresponding to the unit's `tag`.

### GetUnitClasses(unitId)
Gets the classes associated with a unit.

**arguments:**  
`unitId` - Unit ID as returned from `GetUnits()`

**returns:**  
An array of numbers corresponding to `UNIT_CLASS_IDS`.

### GetUnitInfo(unitId)
Gets a variety of information on a unit.

**arguments:**  
`unitId` - Unit ID as returned from `GetUnits()`

**returns:**  
An object with the following properties:
* `health` - The unit's current health.
* `health_max` - The unit's max health.
* `type` - The unit's `UNIT_TYPE_ID`.
* `pos` - The unit's position.
   * `x` - The unit's X position.
   * `y` - The unit's Y position.
* `alliance` - The unit's `ALLIANCE_ID`.
* `shield` - The unit's current shield.  Only present if the unit has `shield_max` > 0.
* `shield_max` - The unit's max shield.  Only present if the unit has `shield_max` > 0.
* `energy` - The unit's current energy.  Only present if the unit has `energy_max` > 0.
* `energy_max` - The unit's max energy.  Only present if the unit has `energy_max` > 0.
* `display` - The unit's `DISPLAY_TYPE_ID`.  Only present for enemies.
* `cloak` - The unit's `CLOAK_TYPE_ID`.  Only present for enemies.

### GetResourceInfo(unitId)
Gets information on a resource unit.

**arguments:**  
`unitId` - Unit ID as returned from `GetUnits()`

**returns:**  
An object with the following properties:
* `pos` - The unit's position.
   * `x` - The unit's X position.
   * `y` - The unit's Y position.
* `contents` - The amount of vespene or minerals available to be harvested from the unit.

### CommandUnit(unitId, abilityId, [target])
Commands a unit to use move or use an ability.

**arguments:**  
`unitId` - Unit ID for the unit to command as returned from `GetUnits()`  
`abilityId` - The id of the ability to use.
`target` - Either a position object with `x` and `y` properties OR a target unit ID.

## Tutorial

### Part 1: Prerequisites
1. Clone [s2client-api](https://github.com/blizzard/s2client-api) and build it on your machine.  
2. Create an environmental variable `S2API_ROOT` set to the directory where you cloned `s2client-api`.  E.g. `E:\dev\git\s2client-api`
3. Install [NodeJS 8.x LTS](https://nodejs.org/en/).  Verify installation by running the following commands from the command line: `npm -v` and `node -v`.
4. Install [Python 2.7.x](https://www.python.org/downloads/) (unless it is already installed).
5. Install windows-build-tools from the command line by the following command: `npm install --global windows-build-tools`

### Part 2: Setting up
1. Create a folder/local repository for you bot's code.
2. Run the `npm init` from the command line in your bot's folder, then follow the prompts and enter options as desired.
3. Run `npm install matortheeternal/s2js --save` from the command line.
4. Create an `index.js` file in your bot's folder and create a `src` folder for additional files.
5. Create a `maps` folder and copy the `BelShirVestigeLE.SC2Map` file into the folder.  (you can find it in the `test` folder in this repository)

### Part 3: Initial code
Put the following code into your `index.js` file:
```js
let s2js = require('s2js');

s2js.StartGame({
    participants: [{
        type: 'bot',
        race: s2js.RACE_IDS.Terran
    }, {
        type: 'computer',
        race: s2js.RACE_IDS.Zerg
    }],
    map: `${__dirname}\\maps\\BelShirVestigeLE.SC2Map`
});

while (s2js.Update()) {
    // call your bot code here
}
```

### Part 4: Write your bot
Use the library to get units and command them to do things after each `Update()` function call.  If you have any questions or have functionality you would like to see added you can create an issue or contact me on the [Starcraft AI discord](https://discord.gg/BfXDftx).
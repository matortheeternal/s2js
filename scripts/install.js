let path = require('path'),
    fs = require('fs');

let buildPaths = ['build_vs2015', 'build_vs2017', 'build'].map(folder =>{
    return path.join(process.env.S2API_ROOT || '', folder);
});

function getBuildPath() {
    return buildPaths.find(path => fs.existsSync(path));
}

const clientApiMissingError =
    'Could not find s2client-api. \n'+
    'Clone from https://github.com/blizzard/s2client-api, follow \n'+
    'the instructions to build it and create an environment variable \n'+
    '`S2API_ROOT` which points to the root folder of the repository.\n';

function canInstall() {
    let buildPath = getBuildPath();
    if (buildPath) console.log('Found s2client-api build at ' + buildPath);
    return buildPath;
}

if (!canInstall())
    throw new Error(clientApiMissingError);

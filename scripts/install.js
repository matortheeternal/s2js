let fs = require('fs');

const libPaths = [
    'build_vs2017/bin/civetweb.lib',
    'build_vs2017/bin/libprotobufd.lib',
    'build_vs2017/bin/sc2apid.lib',
    'build_vs2017/bin/sc2libd.lib',
    'build_vs2017/bin/sc2protocold.lib',
    'build_vs2017/bin/sc2utilsd.lib',
    'include',
    'build_vs2017/generated',
    'contrib/protobuf/src',
    'contrib/civetweb/include'
];

const clientApiMissingError =
    'Could not find s2client-api. \n'+
    'Clone from https://github.com/blizzard/s2client-api, follow '+
    'the instructions to build it and create an environment variable'+
    '`S2API_ROOT` which points to the root folder of the repository.';

function canInstall() {
    return process.env.S2API_ROOT &&
        libPaths.reduce((b, path) => {
            return b && fs.existsSync(`${process.env.S2API_ROOT}\\${path}`)
        }, true);
}

if (!canInstall())
    throw new Error(clientApiMissingError);
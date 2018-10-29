let fs = require('fs'),
    path = require('path'),
    child_process = require('child_process');

function getBuildFolder() {
    return ['build_vs2017', 'build_vs2015', 'build'].find(folder => {
        let fullPath = path.join(process.env.S2API_ROOT, folder);
        return fs.existsSync(fullPath);
    });
}

function getBuildEnv() {
    let env = Object.keys(process.env).reduce((fixedEnv, key) => {
        fixedEnv[key.toUpperCase()] = process.env[key];
        return fixedEnv;
    }, {});
    env.BUILD_FOLDER = getBuildFolder();
    console.log('Using build folder: ' + env.BUILD_FOLDER);
    return env;
}

function build() {
    let gypPath = require.resolve(path.join('node-gyp', 'bin', 'node-gyp.js'));
        args = [gypPath, 'rebuild', '--debug' ];
    console.log('Building:', [process.execPath].concat(args).join(' '));

    let proc = child_process.spawn(process.execPath, args, {
        stdio: [0, 1, 2],
        env: getBuildEnv()
    });

    proc.on('exit', function(errorCode) {
        if (!errorCode) return;

        if (errorCode === 127 ) {
            console.error('node-gyp not found!');
        } else {
            console.error('Build failed with error code:', errorCode);
        }

        process.exit(1);
    });
}

build();
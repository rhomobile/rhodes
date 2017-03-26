
var path = require('path');
var util = require('util');

function JXmobile(x) {
  if (!(this instanceof JXmobile)) return new JXmobile(x);

  this.name = x;
}

function callJXcoreNative(name, args) {
  var params = Array.prototype.slice.call(args, 0);
  var fnc = [name];
  var arr = fnc.concat(params);
  arr.push(cb);
  process.natives.callJXcoreNative.apply(null, arr);
}

JXmobile.prototype.call = function () {
  callJXcoreNative(this.name, arguments);
  return this;
};

var isAndroid = false; //process.platform == "android";

global.Mobile = JXmobile;


// see jxcore.java - jxcore.m
process.setPaths();

if (isAndroid) {
  // bring APK support into 'fs'
  process.registerAssets = function (from) {
    var fs = from;
    if (!fs || !fs.existsSync)
      fs = require('fs');

    var path = require('path');
    var folders = process.natives.assetReadDirSync();
    console.log('folders are: '+JSON.stringify(folders));
    var root = process.cwd();

    // patch execPath to APK folder
    process.execPath = root;




    function createRealPath(pd) {
      var arr = [ pd, pd + "", pd + "/jxcore" ];

      for ( var i = 0; i < 3; ++ i ) {
        try {
          console.log( ">>>>>>>>>>>> WILL TRY PATH: " + arr[i]);


          if (!fs.existsSync(arr[i])) {
            fs.mkdirSync(arr[i]);
          }

        } catch (e) {
                console.error(">>>>>>>>>>>> Permission issues ? ", arr[i], e)
        }
      }
    }

    createRealPath(process.userPath);

    var sroot = root;

    console.log( ">>>>>>>>>>>>>>>>> SROOT: " + sroot );

    var hasRootLink = false;
    if ( root.indexOf('/data/user/') === 0 ) {
      var pd = process.userPath.replace(/\/data\/user\/[0-9]+\//, "/data/data/");
      createRealPath(pd);
      sroot = root.replace(/\/data\/user\/[0-9]+\//,"/data/data/");
      hasRootLink = true;
    }

    console.log( ">>>>>>>>>>>>>>>>> SROOT AFTER CHANGE: " + sroot );

    var jxcore_root;

    var prepVirtualDirs = function() {
      var _ = {};
      for ( var o in folders ) {
        console.log( ">>>>>>>>> prepVirtualDirs, folder: " + o );
        var sub = o.split('/');

        var last = _;

        for ( var i in sub ) {
          var loc = sub[i];
          if (!last.hasOwnProperty(loc)) {
            last[loc] = {};
          }
          last = last[loc];
        }

        last['!s'] = folders[o];
      }

      console.log( ">>>>>>>>>>>>> INSPECT:" + util.inspect( _ ) );

      folders = {};

      var sp = sroot.split('/');
      if ( sp[0] === '' ) {
        sp.shift();
      }

      jxcore_root = folders;

      for ( var o in sp ) {
        if (sp[o] === 'jxcore') {
          continue;
        }

        jxcore_root[sp[o]] = {};
        jxcore_root = jxcore_root[sp[o]];
      }

      jxcore_root['jxcore'] = _;
      jxcore_root = _;

      console.log( ">>>>>>>>>>>>> INSPECT JXCORE_ROOT:" + util.inspect( jxcore_root ) );

    };

    prepVirtualDirs();

    var findIn = function( what, where ) {
      console.log( ">>>>>>>>>>>>>> findIn: " + what + " in: " );

      for ( var o in where ) {
        console.log( ">>>>>>>>>>>>>> findIn: " + o );
      }

      console.log( (new Error()).stack );

      var last = where;
      for ( var o in what ) {
        var subject = what[o];
        if ( !last[subject]) {
          return;
        }

        last = last[subject];
      }
      console.log( ">>>>>>>>>>>>>>>>>>>> findIn res: " + last );
      return last;
    }

    var getLast = function (pathname) {
      console.log(">>>>>>>>>>>>>>> getLast: " + pathname );

        //trim leading and trailing slashes
        while (pathname[0] == '/')
            pathname = pathname.substr(1);

        while (pathname[pathname.length - 1] == '/')
            pathname = pathname.substr(0, pathname.length - 1);

        var dirs = pathname.split('/');

        var res = findIn(dirs, jxcore_root);
        if (!res)
            res = findIn(dirs, folders);

      console.log(">>>>>>>>>>>>>>> getLast result: " + res );


        return res;
    };

    var stat_archive = {};
    var existssync = function (pathname) {

        console.log( ">>>>>>>>>>>>>>> EXISTSYNC: "  + pathname );


        var n = pathname.indexOf(root);




        if (hasRootLink && n == -1)
            n = pathname.indexOf(sroot);


        console.log( ">>>>>>>>>>>>>>> EXISTSYNC index of (s)root:"  + n + " root: " + root + " sroot: " + sroot );


        if (n === 0 || n === -1) {
            if (n === 0) {
                pathname = pathname.replace(root, '');
                if (hasRootLink)
                    pathname = pathname.replace(sroot, '');
            }

            console.log( ">>>>>>>>>>>>>>> EXISTSYNC fixed pathname: "  + pathname );

            var last;
            if (pathname !== '') {
                last = getLast(pathname);
                if (!last)
                    return false;
            } else {
                last = jxcore_root;
            }

            var result;
            // cache result and send the same again
            // to keep same ino number for each file
            // a node module may use caching for dev:ino
            // combinations
            if (stat_archive.hasOwnProperty(pathname))
                return stat_archive[pathname];

            if (typeof last['!s'] === 'undefined') {
                result = { // mark as a folder
                    size : 340,
                    mode : 16877,
                    ino : fs.virtualFiles.getNewIno()
                };
            } else {
                result = {
                    size : last['!s'],
                    mode : 33188,
                    ino : fs.virtualFiles.getNewIno()
                };
            }

            stat_archive[pathname] = result;
            return result;
        }
    };

    var readfilesync = function (pathname) {
        if (!existssync(pathname))
            throw new Error(pathname + " does not exist");

        var rt = root;
        var n = pathname.indexOf(rt);

        if (n != 0 && hasRootLink) {
            n = pathname.indexOf(sroot);
            rt = sroot;
        }

        if (n === 0) {
            pathname = pathname.replace(rt, "");
            //pathname = path.join('jxcore/', pathname);
            return process.natives.assetReadSync(pathname);
        }
    };

    var readdirsync = function (pathname) {
        var rt = pathname.indexOf('/data/') === 0 ? (hasRootLink ? sroot : root)
            : root;
        var n = pathname.indexOf(rt);
        if (n === 0 || n === -1) {
            var last = getLast(pathname);
            if (!last || typeof last['!s'] !== 'undefined')
                return null;

            var arr = [];
            for ( var o in last) {
                var item = last[o];
                if (item && o != '!s')
                    arr.push(o);
            }
            return arr;
        }

        return null;
    };




/*
    try {
      // force create www/jxcore sub folder so we can write into cwd
      if (!fs.existsSync(process.userPath)) {
        fs.mkdir(process.userPath);
        if (!fs.existsSync(root)) {
          fs.mkdir(root);
        }
      }
    } catch (e) {
      console.error("Problem creating assets root at ", root);
      console.error("You may have a problem with writing files");
      console.error("Original error was", e);
    }

    var jxcore_root;

    var prepVirtualDirs = function () {
      var _ = {};
      for (var o in folders) {
        var sub = o.split('/');
        var last = _;
        for (var i in sub) {
          var loc = sub[i];
          if (!last.hasOwnProperty(loc)) last[loc] = {};
          last = last[loc];
        }
        last['!s'] = folders[o];
      }

      folders = {};
      var sp = root.split('/');
      if (sp[0] === '') sp.shift();
      jxcore_root = folders;
      for (var o in sp) {
        if (sp[o] === 'jxcore')
          continue;

        jxcore_root[sp[o]] = {};
        jxcore_root = jxcore_root[sp[o]];
      }

      jxcore_root['jxcore'] = _; // assets/www/jxcore -> /
      jxcore_root = _;
    };

    prepVirtualDirs();

    var findIn = function (what, where) {
      var last = where;
      for (var o in what) {
        var subject = what[o];
        if (!last[subject]) return;

        last = last[subject];
      }

      return last;
    };

    var getLast = function (pathname) {
      while (pathname[0] == '/')
        pathname = pathname.substr(1);

      while (pathname[pathname.length - 1] == '/')
        pathname = pathname.substr(0, pathname.length - 1);

      var dirs = pathname.split('/');

      var res = findIn(dirs, jxcore_root);
      if (!res) res = findIn(dirs, folders);
      return res;
    };

    var stat_archive = {};
    var existssync = function (pathname) {
      var n = pathname.indexOf(root);
      if (n === 0 || n === -1) {
        if (n === 0) {
          pathname = pathname.replace(root, '');
        }

        var last;
        if (pathname !== '') {
          last = getLast(pathname);
          if (!last) return false;
        } else {
          last = jxcore_root;
        }

        var result;
        // cache result and send the same again
        // to keep same ino number for each file
        // a node module may use caching for dev:ino
        // combinations
        if (stat_archive.hasOwnProperty(pathname))
          return stat_archive[pathname];

        if (typeof last['!s'] === 'undefined') {
          result = { // mark as a folder
            size: 340,
            mode: 16877,
            ino: fs.virtualFiles.getNewIno()
          };
        } else {
          result = {
            size: last['!s'],
            mode: 33188,
            ino: fs.virtualFiles.getNewIno()
          };
        }

        stat_archive[pathname] = result;
        return result;
      }
    };

    var readfilesync = function (pathname) {
      if (!existssync(pathname)) throw new Error(pathname + " does not exist");

      var n = pathname.indexOf(root);
      if (n === 0) {
        pathname = pathname.replace(root, "");
        //pathname = path.join('www/jxcore/', pathname);
        return process.natives.assetReadSync(pathname);
      }
    };

    var readdirsync = function (pathname) {
      var n = pathname.indexOf(root);
      if (n === 0 || n === -1) {
        var last = getLast(pathname);
        if (!last || typeof last['!s'] !== 'undefined') return null;

        var arr = [];
        for (var o in last) {
          var item = last[o];
          if (item && o != '!s') arr.push(o);
        }
        return arr;
      }

      return null;
    };
*/
    var extension = {
      readFileSync: readfilesync,
      readDirSync: readdirsync,
      existsSync: existssync
    };

    fs.setExtension("jxcore-java", extension);
    var node_module = require('module');

    node_module.addGlobalPath(process.execPath);
    node_module.addGlobalPath(process.userPath);

  };

  process.registerAssets();

  // if a submodule monkey patches 'fs' module, make sure APK support comes with it
  var extendFS = function() {
    process.binding('natives').fs += "(" + process.registerAssets + ")(exports);";
  };

  extendFS();

  // register below definitions for possible future sub threads
  jxcore.tasks.register(process.setPaths);
  jxcore.tasks.register(process.registerAssets);
  jxcore.tasks.register(extendFS);
} else {
  jxcore.tasks.register(process.setPaths);
}

global.Mobile.onError = function(message, stack) {
    //TODO
};

// define feunction for inform RHomobile about http server started
global.Mobile.httpServerStarted = function() {
    process.natives.httpServerStarted();
}

// load Rhomobile API
var fs = require('fs');
var jsp = path.join(process.cwd(), '../rhoapi/rhoapi.js' );
var jsf = fs.readFileSync(jsp, 'utf8');
eval(jsf);

Rho.Log.info("Platform = " + process.platform, 'Node.js JS');
Rho.Log.info("Process ARCH = " + process.arch, 'Node.js JS');


global.Mobile.loadMainFile = function (filePath) {
  try {
    require(path.join(process.cwd(), filePath));
  } catch (e) {
    Error.captureStackTrace(e);
    Rho.Log.error('ERROR with loading app.js!', 'Node.js JS');
    Rho.Log.error(e.toString(), 'Node.js JS');
  }
};

process.on('uncaughtException', function (e) {
  Error.captureStackTrace(e);
  Rho.Log.error('ERROR:Exception !', 'Node.js JS');
  Rho.Log.error(e.message.toString(), 'Node.js JS');
  Rho.Log.error(JSON.stringify(e.stack).toString(), 'Node.js JS');
  Mobile.onError(e.message, JSON.stringify(e.stack));
});


// load Rhodes application code
jsp = path.join(process.cwd(), '../rhoapp.js' );
jsf = fs.readFileSync(jsp, 'utf8');
eval(jsf);

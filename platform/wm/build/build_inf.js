var fso,output_file;

//TBD need to move sources and settings to a separate config files

var stdout = WScript.StdOut;

var settings = new Object();
settings['wm6'] = ['Windows Mobile 6 Professional SDK (ARMV4I)','VersionMin=5.02','VersionMax=7.99'];
settings['wm653'] = ['Windows Mobile 6.5.3 Professional DTK (ARMV4I)','VersionMin=5.02','VersionMax=7.99'];
settings['ce5'] = ['MC3000c50b (ARMV4I)','VersionMin=5.00','VersionMax=7.99'];

main();

function p(str) {
    output_file.WriteLine(str);
}

function expand_source(es,name,path,section,destination,flag) {
    var s = new Object();
    s.name = name;
    s.path = path;
    s.section = ""+section+"_"+name.replace(/ /g,"_");
    
    if (destination == null || destination == "")
    {
      if (flag == true)
        s.destination = "";
      else
	    s.destination = name;
    }
    else {
      s.destination = destination+"\\"+name;	
    }
    
    s.folder = fso.GetFolder(path);
    es.push(s);

    var fc = new Enumerator(s.folder.SubFolders);
    for (; !fc.atEnd(); fc.moveNext()) {
        expand_source(es,fc.item().Name,fc.item().Path,s.section,s.destination,false);
    }
}

function expand_sources_rho(sources) {
    var es = new Array();
    for (var i in sources) {
        expand_source(es,sources[i][0],sources[i][1],"copyfiles","rho",false);
    }
    return es;
}

function expand_sources(es, sources) {
    for (var i in sources) {
        expand_source(es, sources[i][0], sources[i][1], "copyfiles", "",true);
    }
    return es;
}

function get_copyfiles_sections(es) {
    var str = "";
    for (var i in es) {
        str = str+","+es[i].section;
    }
    return str;
}

function get_source_disks_names(es) {
    var disk = 5;

    for (var i in es) {
        es[i].disk = disk;
        p(""+disk+"=,\"\",,\""+es[i].path+"\\\"");
        disk++;
    }
}

function get_file_list(es) {
    var file_list = new Array();
    for (var i in es) {
        var fc = new Enumerator(es[i].folder.files);
        for (; !fc.atEnd(); fc.moveNext()) {
            var f = new Object();
            f.name = fc.item().Name;
            f.fullname = es[i].folder.Path+"\\"+f.name;
            f.section = es[i].section;
            f.disk = es[i].disk;
            file_list.push(f);
        }
    }
    return file_list;
}

function is_dublicate(flist,file) {
    for(var i in flist) {
        if ( (flist[i].name == file.name) &&
            (flist[i].fullname != file.fullname) ) {
            return true;
        }
    }
    return false;
}

function resolve_dublicates(es) {
    var dups = new Array();
    var flist = get_file_list(es);
    var n = 1;
    for(var i in flist) {
        if ( is_dublicate(flist,flist[i]) ) {
            flist[i].localname = flist[i].name+".copy"+n;
            flist[i].copy = flist[i].fullname+".copy"+n;

            var f = fso.GetFile(flist[i].fullname);
            f.Copy(flist[i].copy);

            dups.push(flist[i]);
            n++;
        } else {
            flist[i].localname = flist[i].name;
        }
    }

    var cleanup_file = fso.CreateTextFile("cleanup.js");

    cleanup_file.WriteLine("var f;");
    cleanup_file.WriteLine("var fso = new ActiveXObject(\"Scripting.FileSystemObject\");");
    for(var i in dups) {
        var copy = dups[i].copy.replace(/\\/g,"\\\\");
        cleanup_file.WriteLine("f = fso.GetFile(\""+copy+"\");");
        cleanup_file.WriteLine("f.Delete();");
    }

    cleanup_file.Close();

    return flist;
}

function get_source_disks_files(es) {

    var f = resolve_dublicates(es);

    for (var i in f) {
        p("\""+f[i].localname+"\"="+f[i].disk);
    }

    return f;
}

function get_destination_dirs(es) {
    for (var i in es) {
        p(es[i].section+"=0,\"%InstallDir%"+"\\"+es[i].destination+"\"");
    }
}

function get_files_for_section(section,f) {
    var list = new Array();
    for (var i in f) {
        if (f[i].section == section) {
            list.push(f[i]);
        }
    }
    return list;
}

function fill_copyfiles_sections(es,f) {
    for (var i in es) {
        p("["+es[i].section+"]");
        var sf = get_files_for_section(es[i].section,f);
        for (var i in sf) {
            p("\""+sf[i].name+"\",\""+sf[i].localname+"\",,0");
           }
           p("");
    }
}

function expand_extensions(platform) {
    var ret = new Array();
    var folder = fso.GetFolder("..\\bin\\" + settings[platform][0] + "\\rhodes\\Release\\");
    var fc = new Enumerator(folder.files);
       for (; !fc.atEnd(); fc.moveNext()) {
           var name = fc.item().Name;
           if (name.indexOf(".dll") != -1) {
               stdout.WriteLine("name: " + name);
               var f = new Object();
               f.name = name;
            ret.push(f);
        }
       }
    return ret;
}

function fill_extensions_source_disk_files(exts) {
    for (var i in exts) {
        var line = "\"" + exts[i].name + "\"=1";
        stdout.WriteLine("Add ext source disk file: " + line);
        p(line);
    }
}

function fill_extensions_files(exts) {
    for (var i in exts) {
        var line = "\"" + exts[i].name + "\",\"" + exts[i].name + "\",,0";
        stdout.WriteLine("Add ext file: " + line);
        p(line);
    }
}

function fill_registry_keys() {
	var regfName = "regs.txt";
	
	if (fso.FileExists(regfName)) {
		var regf = fso.OpenTextFile(regfName);
		var contents = regf.ReadAll();
		regf.Close();
		
		p(contents);
		p("")
	}
}

function pinf(platform,es,exts,name,vendor,srcdir,show_shortcut,is_icon,webkit,rhogempath,usereruntime,include_motocaps,is_custom_config) {
    p("[Version]");
    p("Signature=\"$Windows NT$\"");
    p("Provider=\""+vendor+"\"");
    p("CESignature=\"$Windows CE$\"");
    p("");
    p("[CEStrings]");
    p("AppName=\""+name+"\"");
    p("InstallDir=%CE1%\\%AppName%");
    p("");
    p("[Strings]");
    p("Manufacturer=\""+vendor+"\"");
    p("");
    p("[CEDevice]");
    p(settings[platform][1]);
    p(settings[platform][2]);
    p("BuildMax=0xE0000000");
    p("");
    p("[DefaultInstall]");
    if (show_shortcut && (!usereruntime)){
        p("CEShortcuts=Shortcuts");
    }
    p("AddReg=RegKeys");
    p("CopyFiles=CopyToInstallDir"+((!usereruntime) && webkit ? ",CopyWebKitBin,CopyNPAPI,CopyConfig,CopySystemFiles" : "")+((!usereruntime) && (!webkit) && include_motocaps? ",CopyConfig" : "")+(show_shortcut && usereruntime ? ",Shortcuts" : "")+get_copyfiles_sections(es));
    p("");
    p("[SourceDisksNames]");
    if (usereruntime) {
        p("1=,\"\",,\"" + srcdir + "\\..\\\"");
    } else {
        p("1=,\"\",,\"..\\bin\\"+settings[platform][0]+"\\rhodes\\Release\\\"");
        if (webkit) {
            p("2=,\"\",," + rhogempath + "\"\\\"");
            p("3=,\"\",," + rhogempath + "\"\\NPAPI\\\"");
            p("4=,\"\",," + rhogempath + "\"\\Config\\\"");
        }else
        {
            if(include_motocaps)
            {
                p("2=,\"\",," + rhogempath + "\"\\Config\\\"");
            }
        }
    }
    get_source_disks_names(es);
    p("");
    p("[SourceDisksFiles]");
    if (usereruntime) {
        p("\"" + name + ".lnk\"=1");
    } else {
        p("\"" + name + ".exe\"=1");
        if (webkit) {
            p("\"eklibrary.dll\"=2");
            p("\"prtlib.dll\"=2");
            p("\"ipc_manager.dll\"=2");
            p("\"webkit.dll\"=2");
            p("\"openssl.dll\"=2");
            p("\"PBEngine_WK.dll\"=2");
            p("\"OutProcessWK.exe\"=2");
            p("\"npwtg_jsobjects.dll\"=3");
            p("\"npwtg_legacy.dll\"=3");
            if (!is_custom_config) {
                p("\"Config.xml\"=4");
            }
            p("\"Plugin.xml\"=4");
            p("\"RegEx.xml\"=4");
        }else
        {
            if(include_motocaps)
            {
                if (!is_custom_config) {
                    p("\"Config.xml\"=2");
                }
                p("\"Plugin.xml\"=2");
                p("\"RegEx.xml\"=2");
            }
        }
    }
    fill_extensions_source_disk_files(exts);
    var f = get_source_disks_files(es);
    p("");
    p("[DestinationDirs]");
    if (show_shortcut){
        p("Shortcuts=0,\"%CE11%\"");
    }
    if ((!usereruntime) && webkit) {
        p("CopySystemFiles=0,\"%CE2%\"");
    }
    p("CopyToInstallDir=0,\"%InstallDir%\"");    
    if ((!usereruntime) && webkit) {
        p("CopyWebKitBin=0,\"%InstallDir%\"");
        p("CopyNPAPI=0,\"%InstallDir%\\NPAPI\"");
        p("CopyConfig=0,\"%InstallDir%\\Config\"");
    }
    if ((!usereruntime) && (!webkit) && include_motocaps) {
        p("CopyConfig=0,\"%InstallDir%\\Config\"");
    }
    
    get_destination_dirs(es);
    p("");
    p("[CopyToInstallDir]");
    if (usereruntime) {
        p("\"" + name + ".lnk\",\"" + name + ".lnk\",,0");
    } else {
        p("\"" + name + ".exe\",\"" + name + ".exe\",,0");
        p("\"license_rc.dll\",\"license_rc.dll\",,0");
        if (webkit) {
            p("");
            p("[CopyWebKitBin]");
            p("\"eklibrary.dll\",\"eklibrary.dll\",,0");
            p("\"webkit.dll\",\"webkit.dll\",,0");
            p("\"openssl.dll\",\"openssl.dll\",,0");
            p("\"PBEngine_WK.dll\",\"PBEngine_WK.dll\",,0");
            p("\"OutProcessWK.exe\",\"OutProcessWK.exe\",,0");
            p("");
            p("[CopyNPAPI]");
            p("\"npwtg_jsobjects.dll\",\"npwtg_jsobjects.dll\",,0");
            p("\"npwtg_legacy.dll\",\"npwtg_legacy.dll\",,0");
            p("");
            p("[CopyConfig]");
            if (!is_custom_config) {
                p("\"Config.xml\",\"Config.xml\",,0");
            }
            p("\"Plugin.xml\",\"Plugin.xml\",,0");
            p("\"RegEx.xml\",\"RegEx.xml\",,0");
            p("");
            p("[CopySystemFiles]");
            p("\"prtlib.dll\",\"prtlib.dll\",,0");
            p("\"ipc_manager.dll\",\"ipc_manager.dll\",,0");
        }else
        {
            if(include_motocaps)
            {
                p("");
                p("[CopyConfig]");
                if (!is_custom_config) {
                    p("\"Config.xml\",\"Config.xml\",,0");
                }
                p("\"Plugin.xml\",\"Plugin.xml\",,0");
                p("\"RegEx.xml\",\"RegEx.xml\",,0");
            }
        }
    }
    fill_extensions_files(exts);
    p("");
    fill_copyfiles_sections(es,f);
    if (show_shortcut){
        p("");
        p("[Shortcuts]");
        if (usereruntime)
            p("\"" + name + ".lnk\",\"" + name + ".lnk\",,0");
        else
            p("\""+name+"\",0,\"" + name + ".exe\",%CE11%");
    }
    p("");
    p("[RegKeys]");
    fill_registry_keys()
    p("");
}

function main() {
	
    // args(0) = .inf filename
    // args(1) = platform ('wm6' or 'ce5')
    // args(2) = app_name
    // args(3) = vendor
    // args(4) = srcdir
    // args(5) = hidden_app
    // args(6) = include motorola webkit binaries and configs?
    // args(7) = rhoelements gem folder path
    // args(8) = use RhoElements runtime?
    // args(9) = use Motorola extextension?
    // args(10)... = additional files

    var args = WScript.Arguments;
    fso = new ActiveXObject("Scripting.FileSystemObject");
    output_file = fso.CreateTextFile(args(0));
    var srcdir = args(4)
    var is_icon = fso.FileExists(srcdir+"/icon/icon.ico");
    var show_shortcut = (args(5) == "0");
    var include_webkit = (args(6) == "1");
    var usereruntime = (args(8) == "1");
    var include_motocaps = (args(9) == "1");
    var is_custom_config = fso.FileExists(srcdir+"/apps/Config.xml");

    var sources = new Object();
    sources['db'] = ["db","..\\..\\..\\platform\\shared\\db\\res\\db"];

    if (!usereruntime)
        sources['lib']= ["lib",srcdir+"/lib"];
    else if (is_icon)
        sources['icon']= ["icon",srcdir+"/icon"];
    sources['apps']= ["apps",srcdir+"/apps"];

    var es = expand_sources_rho(sources);
    
    for (var idx = 10; idx < args.length; idx++)
    {
    	if (args(idx) == null)
    		break;
    		
    	var sources_add = new Object();
        sources_add['files']= ["add" + idx,args(idx)];
        es  = expand_sources(es, sources_add);
    }

    var exts;
    if (!usereruntime) {
        exts = expand_extensions(args(1));
    }
    pinf(args(1),es,exts,args(2),args(3),srcdir,show_shortcut,is_icon,include_webkit,args(7),usereruntime,include_motocaps,is_custom_config);

    output_file.Close();
}

var fso,output_file;

//TBD need to move sources and settings to a separate config files

var stdout = WScript.StdOut;

var settings = new Object();
settings['wm6'] = ['Windows Mobile 6 Professional SDK (ARMV4I)','VersionMin=5.02','VersionMax=6.99'];

main();

function p(str) {
	output_file.WriteLine(str);
}

function expand_source(es,name,path,section,destination) {
	var s = new Object();
	s.name = name;
	s.path = path;
	s.section = ""+section+"_"+name.replace(/ /g,"_");
	s.destination = destination+"\\"+name;
	s.folder = fso.GetFolder(path);
	es.push(s);

	var fc = new Enumerator(s.folder.SubFolders);
   	for (; !fc.atEnd(); fc.moveNext()) {
		expand_source(es,fc.item().Name,fc.item().Path,s.section,s.destination);
   	}
}

function expand_sources(sources) {
	var es = new Array();
	for (var i in sources) {
		expand_source(es,sources[i][0],sources[i][1],"copyfiles","rho");
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
	var disk = 2;

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

function pinf(platform,es,exts,name,vendor,show_shortcat) {

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
	if (show_shortcat){
	    p("CEShortcuts=Shortcuts");
	}
	p("AddReg=RegKeys");
	p("CopyFiles=CopyToInstallDir"+get_copyfiles_sections(es));
	p("");
	p("[SourceDisksNames]");
	p("1=,\"\",,\"..\\bin\\"+settings[platform][0]+"\\rhodes\\Release\\\"");
	get_source_disks_names(es);
	p("");
	p("[SourceDisksFiles]");
	p("\"" + name + ".exe\"=1");
	fill_extensions_source_disk_files(exts);
	var f = get_source_disks_files(es);
	p("");
	p("[DestinationDirs]");
	if (show_shortcat){
    	p("Shortcuts=0,%CE2%\Start Menu");
    }
	p("CopyToInstallDir=0,\"%InstallDir%\"");
	get_destination_dirs(es);
	p("");
	p("[CopyToInstallDir]");
	p("\"" + name + ".exe\",\"" + name + ".exe\",,0");
	fill_extensions_files(exts);
	p("");
	fill_copyfiles_sections(es,f);
	if (show_shortcat){
	    p("");
	    p("[Shortcuts]");
	    p("\""+name+"\",0,\"" + name + ".exe\",%CE11%");
	}
	p("");
	p("[RegKeys]");
	p("");
}

function main() {
	var args = WScript.Arguments;
	fso = new ActiveXObject("Scripting.FileSystemObject");
	output_file = fso.CreateTextFile(args(0));

    var sources = new Object();
    sources['db'] = ["db","..\\..\\..\\platform\\shared\\db\\res\\db"];
    //sources['sqlite3']= ["sqlite3","..\\..\\shared\\sqlite3"];
    sources['lib']= ["lib",args(4)+"/lib"];
    sources['apps']= ["apps",args(4)+"/apps"];

	var es = expand_sources(sources);
	var exts = expand_extensions(args(1));
	pinf(args(1),es,exts,args(2),args(3), args(5) == "0");

	output_file.Close();
}

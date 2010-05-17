#common functions for compiling android
#
# uses following globals
# $objdir[]       list of paths to compiled library object files
# $libname[]      list of paths to compiled library
# $build_release  set to true to disable debug binaries

if RUBY_PLATFORM =~ /(win|w)32$/
  $bat_ext = ".bat"
  $exe_ext = ".exe"
  $ndkhost = "windows"
else
  $bat_ext = ""
  $exe_ext = ""
  $ndkhost = `uname -s`.downcase!.chomp! + "-x86"
end

def setup_ndk(ndkpath,apilevel)
  $ndkgccver = "4.2.1"
  $ndktools = ndkpath + "/build/prebuilt/#{$ndkhost}/arm-eabi-#{$ndkgccver}"
  $ndksysroot = ndkpath + "/build/platforms/android-#{apilevel}/arch-arm"

  $gccbin = $ndktools + "/bin/arm-eabi-gcc" + $exe_ext
  $gppbin = $ndktools + "/bin/arm-eabi-g++" + $exe_ext
  $arbin = $ndktools + "/bin/arm-eabi-ar" + $exe_ext
  $ranlib = $ndktools + "/bin/arm-eabi-ranlib" + $exe_ext
  $stripbin = $ndktools + "/bin/arm-eabi-strip" + $exe_ext
end

def cc_def_args
  if $cc_def_args_val.nil?
    args = []
    args << "--sysroot"
    args << $ndksysroot
    #args << "-fvisibility=hidden"
    args << "-fPIC"
    args << "-mandroid"
    args << "-DANDROID"
    args << "-DOS_ANDROID"
    args << "-DRHO_DEBUG"
    args << "-g"
    if $build_release
      args << "-O2"
      args << "-DNDEBUG"
    else
      args << "-O0"
      args << "-D_DEBUG"
    end
    $cc_def_args_val = args
  end
  $cc_def_args_val.dup
end

def cpp_def_args
  if $cpp_def_args_val.nil?
    args = []
    args << "-fvisibility-inlines-hidden"
    args << "-fexceptions -frtti" unless USE_STLPORT
    $cpp_def_args_val = args
  end
  $cpp_def_args_val.dup
end

def get_def_args(filename)
  if filename =~ /\.[cC]$/
    cc_def_args
  elsif filename =~ /\.[cC]([cC]|[xXpP][xXpP])$/
    cpp_def_args + cc_def_args
  end
end

def cc_get_ccbin(filename)
  if filename =~ /\.[cC]$/
    $gccbin
  elsif filename =~ /\.[cC]([cC]|[xXpP][xXpP])$/
    $gppbin
  end
end

def cc_deps(filename, objdir, additional)
  #puts "Check #{filename}..."
  depfile = File.join objdir, File.basename(filename).gsub(/\.[cC]([cC]|[xXpP][xXpP])?$/, ".d")
  if File.exists? depfile
    if FileUtils.uptodate? depfile, File.read(depfile).gsub(/(^\s+|\s+$)/, '').split(/\s+/)
      return []
    end
  end
  ccbin = cc_get_ccbin(filename)
  args = get_def_args(filename)
  args += additional unless additional.nil?
  out = `#{ccbin} #{args.join(' ')} -MM -MG #{filename}`
  out.gsub!(/^[^:]*:\s*/, '') unless out.nil?
  out.gsub!(/\\\n/, ' ') unless out.nil?
  out = "" if out.nil?
  #out = File.expand_path(__FILE__) + ' ' + out

  mkdir_p objdir unless File.directory? objdir
  File.open(depfile, "w") { |f| f.write(out) }

  out.split(/\s+/)
end

def cc_run(command, args, chdir = nil)
  save_cwd = FileUtils.pwd
  FileUtils.cd chdir unless chdir.nil?
  argv = [command]
  argv += args
  cmdstr = argv.map! { |x| x.to_s }.map! { |x| x =~ / / ? '"' + x + '"' : x }.join(' ')
  puts cmdstr
  $stdout.flush
  argv = cmdstr if RUBY_VERSION =~ /^1\.8/
  IO.popen(argv) do |f|
    while data = f.gets
      puts data
      $stdout.flush
    end
  end
  ret = $?
  FileUtils.cd save_cwd
  ret == 0
end

def cc_compile(filename, objdir, additional = nil)
  filename.chomp!
  objname = File.join objdir, File.basename(filename).gsub(/\.[cC]([cC]|[xXpP][xXpP])?$/, ".o")

  return true if FileUtils.uptodate? objname, [filename] + cc_deps(filename, objdir, additional)

  mkdir_p objdir unless File.directory? objdir

  ccbin = cc_get_ccbin(filename)

  args = get_def_args(filename)
  args << "-Wall"
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-c"
  args << filename
  args << "-o"
  args << objname
  cmdline = ccbin + ' ' + args.join(' ')
  cc_run(ccbin, args)
end

def cc_ar(libname, objects)
  return true if FileUtils.uptodate? libname, objects
  cc_run($arbin, ["crs", libname] + objects)
end

def cc_link(outname, objects, additional = nil, deps = nil)
  dependencies = objects
  dependencies += deps unless deps.nil?
  return true if FileUtils.uptodate? outname, dependencies
  args = []
  args << "-nostdlib"
  args << "-Wl,-shared,-Bsymbolic"
  args << "-Wl,--no-whole-archive"
  args << "-Wl,--no-undefined"
  args << "-Wl,-z,defs"
  args << "-shared"
  args << "-fPIC"
  args << "-Wl,-soname,#{outname}"
  args << "-o"
  args << outname
  args += objects
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-L#{$ndksysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$ndksysroot}/usr/lib"
  args << "#{$ndksysroot}/usr/lib/libstdc++.so"
  args << "#{$ndksysroot}/usr/lib/libsupc++.so" unless USE_STLPORT
  args << "#{$ndktools}/lib/gcc/arm-eabi/#{$ndkgccver}/interwork/libgcc.a"
  args << "#{$ndksysroot}/usr/lib/libc.so"
  args << "#{$ndksysroot}/usr/lib/libm.so"
  cc_run($gccbin, args)
end

def cc_clean(name)
  [$objdir[name], $libname[name]].each do |x|
    rm_rf x if File.exists? x
  end
end

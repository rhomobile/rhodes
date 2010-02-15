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
    args << "-fPIC"
    args << "-mandroid"
    args << "-DANDROID"
    args << "-DOS_ANDROID"
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
    args << "-fexceptions -frtti" unless USE_STLPORT
    $cpp_def_args_val = args
  end
  $cpp_def_args_val.dup
end

def cc_get_ccbin(filename)
  if filename =~ /\.[cC]$/
    $gccbin
  elsif filename =~ /\.[cC]([cC]|[xXpP][xXpP])$/
    $gppbin + ' ' + cpp_def_args.join(' ')
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
  args = cc_def_args
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

def cc_run(command, args)
  cmdline = command + ' ' + args.join(' ')
  puts cmdline
  puts `#{cmdline}`
  $stdout.flush
end

def cc_compile(filename, objdir, additional = nil)
  filename.chomp!
  objname = File.join objdir, File.basename(filename).gsub(/\.[cC]([cC]|[xXpP][xXpP])?$/, ".o")

  return true if FileUtils.uptodate? objname, [filename] + cc_deps(filename, objdir, additional)

  mkdir_p objdir unless File.directory? objdir

  ccbin = cc_get_ccbin(filename)

  args = cc_def_args
  args << "-Wall"
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-c"
  args << filename
  args << "-o"
  args << objname
  cmdline = ccbin + ' ' + args.join(' ')
  cc_run(ccbin, args)
  return $? == 0
end

def cc_ar(libname, objects)
  return true if FileUtils.uptodate? libname, objects
  cc_run($arbin, ["crs", libname] + objects)
  return $? == 0
end

def cc_link(outname, objects, additional = nil, deps = nil)
  dependencies = objects
  dependencies += deps unless deps.nil?
  return true if FileUtils.uptodate? outname, dependencies
  args = []
  args << "-nostdlib"
  args << "-Wl,-shared,-Bsymbolic"
  args << "-Wl,--whole-archive"
  args << "-Wl,--no-whole-archive"
  args << "-Wl,--no-undefined"
  args << "-Wl,-z,defs"
  args << "#{$ndksysroot}/usr/lib/libc.so"
  args << "#{$ndksysroot}/usr/lib/libstdc++.so" if USE_STLPORT
  args << "#{$ndksysroot}/usr/lib/libm.so"
  args << "-L#{$ndksysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$ndksysroot}/usr/lib"
  args << "#{$ndktools}/arm-eabi/lib/interwork/libstdc++.a" unless USE_STLPORT
  args << "#{$ndktools}/arm-eabi/lib/interwork/libsupc++.a" unless USE_STLPORT
  args << "#{$ndktools}/lib/gcc/arm-eabi/#{$ndkgccver}/interwork/libgcc.a"
  args << "-shared"
  args << "-fPIC"
  args << "-Wl,-soname,#{outname}"
  args << "-o"
  args << outname
  args += objects
  args += additional if additional.is_a? Array and not additional.empty?
  args << "#{$ndktools}/arm-eabi/lib/interwork/libstdc++.a" unless USE_STLPORT
  args << "#{$ndktools}/arm-eabi/lib/interwork/libsupc++.a" unless USE_STLPORT
  cc_run($gccbin, args)
  return $? == 0
end

def cc_clean(name)
  [$objdir[name], $libname[name]].each do |x|
    rm_rf x if File.exists? x
  end
end

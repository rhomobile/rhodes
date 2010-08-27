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

def num_cpus
  num = nil
  if RUBY_PLATFORM =~ /linux/
    num = `cat /proc/cpuinfo | grep processor | wc -l`.gsub("\n", '')
  elsif RUBY_PLATFORM =~ /darwin/
    num = `sysctl -n hw.ncpu`.gsub("\n", '')
  elsif RUBY_PLATFORM =~ /w(in)?32/
    num = ENV['NUMBER_OF_PROCESSORS']
  else
    num = 1
  end
  num = num.to_i
  num = 1 if num == 0
  num
end

def get_sources(name)
    File.read(File.join($builddir, name + '_build.files')).split("\n")
end

def setup_ndk(ndkpath,apilevel)
  $ndkgccver = "unknown"
  ["4.4.0", "4.2.1"].each do |ver|
    tools = File.join(ndkpath, "build/prebuilt", $ndkhost, "arm-eabi-#{ver}")
    next unless File.directory? tools
    $ndkgccver = ver
    $ndktools = tools
    break
  end
  $ndksysroot = File.join(ndkpath, "build/platforms/android-#{apilevel}/arch-arm")

  ['gcc', 'g++', 'ar', 'strip', 'objdump'].each do |tool|
    name = tool.gsub('+', 'p')
    eval "$#{name}bin = $ndktools + '/bin/arm-eabi-#{tool}' + $exe_ext"
  end
  
  # Detect rlim_t
  if $have_rlim_t.nil?
    $have_rlim_t = false
    resource_h = File.join(ndkpath, 'build', 'platforms', "android-#{apilevel}", "arch-arm", "usr", "include", "sys", "resource.h")
    if File.exists? resource_h
      File.open(resource_h, 'r') do |f|
        while line = f.gets
          if line =~ /^\s*typedef\b.*\brlim_t\s*;\s*$/
            $have_rlim_t = true;
            break;
          end
        end
      end
    end
  end
end

def cc_def_args
  if $cc_def_args_val.nil?
    args = []
    args << "--sysroot"
    args << $ndksysroot
    #args << "-fvisibility=hidden"
    args << "-fPIC"
    args << "-Wno-psabi" if $ndkgccver != "4.2.1"
    args << "-mandroid"
    args << "-DANDROID"
    args << "-DOS_ANDROID"
    args << "-DRHO_DEBUG"
    args << "-DHAVE_RLIM_T" if $have_rlim_t
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
  ret.success?
end

def cc_compile(filename, objdir, additional = nil)
  filename.chomp!
  objname = File.join objdir, File.basename(filename) + ".o"

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

def cc_build(name, objdir, additional = nil)
  sources = get_sources(name)
  
  # Ruby 1.8 has problems with Thread.join on Windows
  if RUBY_PLATFORM =~ /w(in)?32/ and RUBY_VERSION =~ /^1\.8\./
    sources.each do |f|
      return false unless cc_compile f, objdir, additional
    end
    true
  else
    jobs = num_cpus
    jobs += 1 if jobs > 1

    srcs = []
    for i in (0..jobs-1)
      srcs[i] = []
    end

    sources.each do |src|
      idx = sources.index(src)%jobs
      srcs[idx] << src
    end

    ths = []
    srcs.each do |src|
      ths << Thread.new do
        success = true
        src.each do |f|
          success = cc_compile f, objdir, additional
          break unless success
        end
        success
      end
    end

    ret = true
    ths.each do |th|
      success = th.value
      ret = success unless success
    end
    ret
  end
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
  args << "-fPIC"
  args << "-Wl,-soname,#{File.basename(outname)}"
  args << "-o"
  args << outname
  args += objects
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-L#{$ndksysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$ndksysroot}/usr/lib"
  if $cxxlibs.nil?
    $cxxlibs = []
    if USE_STLPORT
      $cxxlibs << File.join($ndksysroot, "usr/lib/libstdc++.so")
    else
      $cxxlibs << `#{$gccbin} -mthumb-interwork -print-file-name=libstdc++.a`.gsub("\n", "")
      $cxxlibs << `#{$gccbin} -mthumb-interwork -print-file-name=libsupc++.a`.gsub("\n", "")
    end
  end
  args += $cxxlibs
  $libgcc = `#{$gccbin} -mthumb-interwork -print-file-name=libgcc.a`.gsub("\n", "") if $libgcc.nil?
  args << $libgcc if $ndkgccver != "4.2.1"
  args << "#{$ndksysroot}/usr/lib/libc.so"
  args << "#{$ndksysroot}/usr/lib/libm.so"
  args << $libgcc if $ndkgccver == "4.2.1"
  cc_run($gccbin, args)
end

def cc_clean(name)
  [$objdir[name], $libname[name]].each do |x|
    rm_rf x if File.exists? x
  end
end

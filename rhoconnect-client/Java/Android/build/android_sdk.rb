#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------



#common functions for compiling android
#
# uses following globals
# $objdir[]       list of paths to compiled library object files
# $libname[]      list of paths to compiled library
# $build_release  set to true to disable debug binaries

USE_TRACES = Rake.application.options.trace

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

def setup_sdk(sdkpath, api_level)
    unless File.exists? sdkpath
      puts "Missing or invalid 'android' section in rhobuild.yml: '#{sdkpath}'"
      exit 1
    end

#    $rhobindir = File.join($basepath, "bin")
#    $shareddir = File.join($androidpath, "..", "shared")
#    $srcdir = File.join($bindir, "RhoBundle")
#    $targetdir = File.join($bindir, "target")
#    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']
#    $tmpdir = File.join($bindir, "tmp")
#    $resourcedir = File.join($tmpdir, "resource")
#    $libs = File.join($androidpath, "Rhodes", "libs")

    puts "+++ Looking for platform..." if USE_TRACES
    top_level = api_level

    api_levels = Array.new
    platforms = File.join sdkpath, "platforms"

    Dir.glob(File.join(platforms, "*")).each do |platform|

      props = File.join(platform, "source.properties")
      unless File.file? props
        puts "+++ WARNING! No source.properties found in #{platform}"
        next
      end

      level = -1
      marketversion = nil
      File.open(props, "r") do |f|
        while line = f.gets
          level = $1.to_i if line =~ /^\s*AndroidVersion\.ApiLevel\s*=\s*([0-9]+)\s*$/
          marketversion = $1 if line =~ /^\s*Platform\.Version\s*=\s*([^\s]*)\s*$/
        end
      end

      puts "+++ API LEVEL of #{platform}: #{level}" if USE_TRACES
      api_levels.push level

      if level >= top_level
        top_level = level
        $androidplatform = File.basename(platform)
        #$found_api_level = level
      end
    end

    api_levels.sort!

    if USE_TRACES
      puts "Found API levels:"
      api_levels.each do |level|
        puts level
      end
      #puts "Corresponding API level #{requested_api_level}"
    end

#    if !is_api_level_installed
#      puts "ERROR!!! API version is not found in installed Android SDK: #{$emuversion}"
#      exit 1
#    end

#    if $androidplatform.nil?
#      ajar = File.join($androidsdkpath, 'platforms', 'android-' + $min_sdk_level.to_s, 'android.jar')
#      if USE_TRACES
#        puts "Using target path: "+ ajar
#      end
#      $androidplatform = 'android-' + $min_sdk_level.to_s if File.file?(ajar)
#    end

    if $androidplatform.nil?
      puts "+++ No required platform (API level >= #{api_level}) found, can't proceed"
      puts "+++ Looks like you have no installed required Android platform package."
      puts "+++ To solve that, please strictly follow instructions from http://wiki.rhomobile.com/index.php/BuildingRhodes#Prerequisites_5"
      exit 1
    else
      puts "SDK platform path: #{File.join platforms, $androidplatform}"
    end
    $stdout.flush

    top_level

end

def setup_ndk(ndkpath, apilevel)
  puts "setup_ndk(#{ndkpath}, #{apilevel})" if USE_TRACES

  unless File.exists? ndkpath
    puts "Invalid 'android-ndk' section in rhobuild.yml: '#{ndkpath}'"
    exit 1
  end

  if ndkpath =~ /\s/
    errfmt = "WARNING!!! Path to Android %s contain spaces! It will not work because of the Google toolchain restrictions. Move it to another location and reconfigure rhodes."
    puts(errfmt % "NDK")
    exit 1
  end

  $ndktools = nil
  $ndkabi = "unknown"
  $ndkgccver = "unknown"
  ["arm-linux-androideabi-4.4.3", "arm-eabi-4.4.0", "arm-eabi-4.2.1"].each do |abi|
    variants = []
    variants << File.join(ndkpath, "toolchains", abi, "prebuilt", $ndkhost)
    variants << File.join(ndkpath, "build/prebuilt", $ndkhost, abi)
    variants.each do |variant|
      next unless File.directory? variant
      $ndktools = variant
      $ndkabi = abi.gsub(/^(.*)-([^-]*)$/, '\1')
      $ndkgccver = abi.gsub(/^(.*)-([^-]*)$/, '\2')
      break
    end
    break unless $ndktools.nil?
  end
  if $ndktools.nil?
    raise "Can't detect NDK toolchain path (corrupted NDK installation?)"
  end

  variants = []
  variants << "platforms"
  variants << File.join("build", "platforms")

  api_levels = Array.new

  variants.each do |variant|
    puts "Check NDK folder: #{variant}" if USE_TRACES
    Dir.glob(File.join(ndkpath, variant, "*")).each do |platform|
      sys_root = File.join platform, "arch-arm"
      puts "Checking #{sys_root} for NDK nsysroot"  if USE_TRACES
      next unless File.directory? sys_root
      next unless platform =~ /android-([0-9]+)$/
      api_level = $1.to_i
      api_levels.push api_level
      puts "NDK API level: #{api_level}" if USE_TRACES
    end
  end

  api_levels.sort!

  last_api_level = 0
  api_levels.each do |cur_api_level|
    puts "Checking is API level enough: #{cur_api_level}"  if USE_TRACES
    break if cur_api_level > apilevel.to_i
    last_api_level = cur_api_level
  end

  variants.each do |variant|
    sysroot = File.join(ndkpath, variant, "android-#{last_api_level}/arch-arm")
    next unless File.directory? sysroot
    $ndksysroot = sysroot
    break
  end
  if $ndksysroot.nil?
    raise "Can't detect NDK sysroot (corrupted NDK installation?)"
  end
  puts "NDK platform sysroot: #{$ndksysroot}"

  ['gcc', 'g++', 'ar', 'strip', 'objdump'].each do |tool|
    name = tool.gsub('+', 'p')
    eval "$#{name}bin = $ndktools + '/bin/#{$ndkabi}-#{tool}' + $exe_ext"
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
    args << "-fno-exceptions"
    args << "-fno-rtti"
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
  argv = cmdstr if RUBY_VERSION =~ /^1\.[89]/
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

def get_object(source, dir)
  return File.join dir, File.basename(source) + ".o"
end

def cc_compile(filename, objdir, additional = nil)
  #filename.chomp!
  objname = get_object filename, objdir

  return true if FileUtils.uptodate? objname, [filename] + cc_deps(filename, objdir, additional)

  mkdir_p objdir unless File.directory? objdir

  ccbin = cc_get_ccbin(filename)

  args = get_def_args(filename)
  args << "-Wall"
  args << "-Wextra"
  args << "-Wno-unused"
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-c"
  args << filename
  args << "-o"
  args << objname
  cmdline = ccbin + ' ' + args.join(' ')
  cc_run(ccbin, args)
end

def get_sources(name)
    File.read(File.join($basepath, name + '_build.files')).split("\n")
end

def get_objects(sources, objdir)
  objects = []
  sources.each do |filename|
    objects << get_object(filename, objdir)
  end

  if USE_TRACES
    puts "sources:"
    sources.each do |src|
      puts src
    end
    puts "objects:"
    objects.each do |obj|
      puts obj
    end
    puts ""
  end

  objects
end

def cc_build(sources, buildpath, additional = nil)

  # Ruby 1.8 has problems with Thread.join on Windows
  if RUBY_PLATFORM =~ /w(in)?32/ and RUBY_VERSION =~ /^1\.8\./
    sources.each do |src|
      f = File.join $rootpath, src
      return false unless cc_compile f, buildpath, additional
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
      f = File.join $rootpath, src
      puts "job for: #{f}" if USE_TRACES

      idx = sources.index(src)%jobs
      srcs[idx] << f
    end

    ths = []
    srcs.each do |src|
      ths << Thread.new do
        success = true
        src.each do |f|
          success = cc_compile f, buildpath, additional
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
  puts "linking #{outname}"
  dependencies = objects
  unless deps.nil?
    if USE_TRACES
      deps.each do |dep|
        puts "link dep: #{dep}"
      end
    end
    dependencies += deps
  end
  #return true if FileUtils.uptodate? outname, dependencies
  args = ["-g", "--sysroot", $ndksysroot]
  if $ndkabi == "arm-eabi"
    args << "-nostdlib"
    args << "-Wl,-shared,-Bsymbolic"
  else
    args << "-shared"
  end
  #args << "-Wl,--no-whole-archive"
  args << "-Wl,--no-undefined"
  args << "-Wl,-z,defs"
  args << "-fPIC"
  args << "-Wl,-soname,#{File.basename(outname)}"
  args << "-o"
  args << outname
  args += objects
  args += additional if additional.is_a? Array and not additional.empty?
  unless USE_OWN_STLPORT
    args << "-L#{File.join($androidndkpath, "sources","cxx-stl","stlport","libs","armeabi")}"
    args << "-L#{File.join($androidndkpath, "tmp","ndk-digit","build","install","sources","cxx-stl","stlport","libs","armeabi")}"
    args << "-lstlport_static"
  end
  args << "-L#{$ndksysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$ndksysroot}/usr/lib"
  if $cxxlibs.nil?
    $cxxlibs = []
    $cxxlibs << File.join($ndksysroot, "usr/lib/libstdc++.so")
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

def jar_build(name, classpath, jarname, buildpath)
  javac = File.join($rhoconfig["env"]["paths"]["java"], "javac" + $exe_ext)
  jarbin = File.join($rhoconfig["env"]["paths"]["java"], "jar" + $exe_ext)

  args = [
      "-d", buildpath,
      "-source", "1.6",
      "-target", "1.6",
      "-nowarn",
      "-encoding", "latin1",
      "-classpath", classpath,
      "@#{File.join($basepath, name + '_build.files')}"
  ]
  cc_run(javac, args)
  unless $?.success?
    puts "Error compiling java code"
    exit 1
  end

  files = []
  Dir.glob(File.join(buildpath, "*")).each do |f|
    relpath = Pathname.new(f).relative_path_from(Pathname.new(buildpath)).to_s
    files << relpath
  end
  unless files.empty?
    reljarpath = Pathname.new(jarname).relative_path_from(Pathname.new(buildpath)).to_s
    args = ["cf", reljarpath]
    args += files
    cc_run(jarbin, args, buildpath)
    unless $?.success?
      puts "Error running jar"
      exit 1
    end
  end

end

def apk_build(sdk, apk_name, res_name, dex_name, debug)
    puts "Building APK file..."
    prev_dir = Dir.pwd
    Dir.chdir File.join(sdk, "tools")
    #"-classpath", File.join("lib", "sdklib.jar"), "com.android.sdklib.build.ApkBuilderMain",
    if debug
        params = [apk_name, "-z", res_name, "-f", dex_name]
    else
        params = [apk_name, "-u", "-z", res_name, "-f", dex_name]
    end

    if RUBY_PLATFORM =~ /(win|w)32$/
        apkbuilder = "apkbuilder" + $bat_ext
    else
        apkbuilder = File.join(".", "apkbuilder" + $bat_ext)
    end

    Jake.run apkbuilder, params

    unless $?.success?
        Dir.chdir prev_dir
        puts "Error building APK file"
        exit 1
    end
    Dir.chdir prev_dir
end

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

require 'tempfile'
require 'open3'
require 'stringio'

#common functions for compiling android
#
# uses following globals
# $objdir[]       list of paths to compiled library object files
# $libname[]      list of paths to compiled library
# $debug          set to true to enable debug binaries

USE_TRACES = Rake.application.options.trace

if RUBY_PLATFORM =~ /(win|w)32$/
  $bat_ext = ".bat"
  $exe_ext = ".exe"
else
  $bat_ext = ""
  $exe_ext = ""
end

$output_lock = Mutex.new

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

def get_sources(sourcelist)
    res = File.read(sourcelist).split(/\r?\n/)
end

def get_objects(sources, objdir)
    sources.map { |src| File.join(objdir, File.basename(src) + ".o") }    
end

def detect_toolchain(ndkpath, abi)
  $ndktools = nil
  $ndkabi = "unknown"
  $ndkgccver = "unknown"
  ndkhostvariants = []
  if RUBY_PLATFORM =~ /(win|w)32$/
      bufcheck64 = `WMIC OS get OSArchitecture`.split[1]
      ndkhostvariants << 'windows-x86_64' if bufcheck64 and bufcheck64.include?('64')
      ndkhostvariants << 'windows'
  else
      ndkhostvariants = [
        `uname -s`.downcase!.chomp! + "-" + `uname -m`.chomp!, 
        `uname -s`.downcase!.chomp! + '-x86'
      ]
  end

  toolchainversions = ['4.8','4.9']

  toolchain = 'unknown-toolchain'
  if abi == 'arm'
    toolchain = 'arm-linux-androideabi'
  elsif abi == 'x86'
    toolchain = 'x86'
  elsif abi == 'x86_64'
    toolchain = 'x86_64'
  elsif abi == 'mips'
    toolchain = 'mipsel-linux-android'
  else
    raise "Unknown ABI: {abi}";
  end

  ndkhostvariants.each do |ndkhost|
      puts "Checking toolchain for host: #{ndkhost}" if USE_TRACES

      toolchainversions.each do |version|
        variants = []
        variants << File.join(ndkpath,'build','prebuilt',ndkhost,"#{toolchain}-#{version}")
        variants << File.join(ndkpath,'toolchains',"#{toolchain}-#{version}",'prebuilt',ndkhost)

        variants.each do |variant|
          puts "Check toolchain path: #{variant}" if USE_TRACES    
          next unless File.directory? variant

          $ndktools = variant
          $ndkabi = toolchain#toolchain.gsub(/^(.*)-([^-]*)$/, '\1')
          $ndkgccver = version#toolchain.gsub(/^(.*)-([^-]*)$/, '\2')
          
          $ndkabi = 'i686-linux-android' if $ndkabi == 'x86'
          $ndkabi = 'x86_64-linux-android' if $ndkabi == 'x86_64'

          puts "Toolchain is detected: #{$ndktools}, abi: #{$ndkabi}, version: #{$ndkgccver}" if USE_TRACES
          
          ['gcc', 'g++', 'ar', 'strip', 'objdump'].each do |tool|
              name = tool.gsub('+', 'p')

              toolpath = check_tool( tool, $ndktools, $ndkabi)

              eval "$#{name}bin = $ndktools + '/bin/#{$ndkabi}-#{tool}' + $exe_ext"
          end

          return
        end
      end
  end

  if $ndktools.nil?
    raise "Can't detect NDK toolchain path (corrupted NDK installation?)"
  end  
end

def check_tool( tool, ndktoolsdir, abi )
  toolpath = File.join(ndktoolsdir,'bin',"#{abi}-#{tool}#{$exe_ext}")
  puts "Checking tool path #{toolpath} for tool #{tool}" if USE_TRACES
  
  if File.file? toolpath
    return toolpath
  else
    raise "Can't find tool #{tool} at path #{toolpath} (corrupted NDK installation or unsupported NDK?)"
  end
end

def setup_ndk(ndkpath,apilevel,abi)
  puts "setup_ndk(#{ndkpath}, #{apilevel}, #{abi})" if USE_TRACES
  
  detect_toolchain ndkpath, abi

  variants = []
  variants << "platforms"
  variants << File.join("build", "platforms")

  api_levels = Array.new

  max_ndk_api_level = 19 #we use some functions missing from API 20 and forth

  variants.each do |variant|
    puts "Check NDK folder: #{variant}" if USE_TRACES
    Dir.glob(File.join(ndkpath, variant, "*")).each do |platform|
      sys_root = File.join platform, "arch-arm"
      puts "Checking #{sys_root} for NDK nsysroot"  if USE_TRACES
      next unless File.directory? sys_root
      next unless platform =~ /android-([0-9]+)$/
      api_level = $1.to_i 
      api_levels.push api_level if (api_level<=max_ndk_api_level)
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
    sysroot = File.join(ndkpath, variant, "android-#{last_api_level}/arch-#{abi}")
    next unless File.directory? sysroot
    $ndksysroot = sysroot
    break
  end
  if $ndksysroot.nil?
    raise "Can't detect NDK sysroot (corrupted NDK installation?)"
  end
  puts "NDK sysroot: #{$ndksysroot}"

  $androidndkpath = ndkpath unless $androidndkpath

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
    args = []
    args << "--sysroot"
    args << $ndksysroot
    args << "-fPIC"
    args << "-Wall"
    args << "-Wextra"
    args << "-Wno-sign-compare"
    args << "-Wno-unused"
    args << '-Wno-unused-parameter'
    args << "-mandroid"
    args << "-DANDROID"
    args << "-DOS_ANDROID"
    args << "-DRHO_DEBUG"
    args << "-DHAVE_RLIM_T" if $have_rlim_t
    args << "-g"
    unless $debug
      args << "-O2"
      args << "-DNDEBUG"
    else
      args << "-Og"
      args << "-ggdb"
      args << "-fstack-protector-all"
      args << "-D_DEBUG"
      args << "-Winit-self"
      args << "-Wshadow"
      args << "-Wcast-align"
      args << "-Wvla"
      args << "-Wstack-protector"
    end
    args
end

def cpp_def_args
    args = cc_def_args()
    args << "-fvisibility-inlines-hidden"
    args << "-fno-exceptions"
    args << "-fno-rtti"
    args << "-std=c++11"
    args << "-Wno-reorder"
    #args << "-I\"#{File.join($androidndkpath,'sources','cxx-stl','stlport','stlport')}\""
    args << "-I\"#{File.join($androidndkpath,'sources','cxx-stl','gnu-libstdc++',$ndkgccver,'include')}\""
    args << "-I\"#{File.join($androidndkpath,'sources','cxx-stl','gnu-libstdc++',$ndkgccver,'include','backward')}\""
    args << "-I\"#{File.join($androidndkpath,'sources','cxx-stl','gnu-libstdc++',$ndkgccver,'libs','armeabi','include')}\""
    args
end

def get_def_args(filename)
  if filename =~ /\.[cC]$/
    cc_def_args
  elsif filename =~ /\.[cC]([cC]|[xXpP][xXpP])$/
    cc_def_args + cpp_def_args
  end
end

def cc_get_ccbin(filename)
  if filename =~ /\.[cC]$/
    $gccbin
  elsif filename =~ /\.[cC]([cC]|[xXpP][xXpP])$/
    $gppbin
  end
end

#def deps_uptodate?(target, source, deps)
#  deplist = [deps, source]
#  if(File.exists? deps)
#    deplist += File.read(deps).gsub(/(^\s+|\s+$)/, '').split(/\s+/)
#    return File.exists?(target) and FileUtils.uptodate?(target, deplist)
#  else
#    return false
#  end
#end

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

def cc_run(command, args, chdir = nil, coloring = true, env = nil)
  save_cwd = FileUtils.pwd
  FileUtils.cd chdir unless chdir.nil?
  argv = [command]
  argv += args
  cmdstr = argv.map! { |x| x.to_s }.map! { |x| x =~ / / ? '' + x + '' : x }.join(' ')

  isWinXP = false
  if RUBY_PLATFORM =~ /(win|w)32$/
    winName = `WMIC OS get Name`
    isWinXP = true if winName =~ /Windows XP/
  end

  env = ENV unless env
  
  if isWinXP
    puts '-' * 80
    %{#{cmdstr}}
  else
    out = StringIO.new
    ret = nil
    Open3.popen2e(env, cmdstr) do |i,f,t|
      warning = false
      error = false
      while data = f.gets
        if data =~ /error:/
          error = true
        elsif data =~ /warning:/
          warning = true
        elsif data =~ /note:/
        else
          warning = false
          error = false
        end

        if error && coloring
          out.write "\e[31m#{data}\e[0m"
        elsif warning && coloring
          out.write "\e[33m#{data}\e[0m"
        else
          out.puts data
        end
      end
      ret = t.value
    end

    $output_lock.synchronize {
      puts '-' * 80
      puts "PWD: " + FileUtils.pwd
      puts cmdstr
      puts out.string
    }
    out.close
  end
  
  FileUtils.cd save_cwd unless chdir.nil?
  ret.success?
end

def cc_compile(filename, objdir, additional = nil)
  #filename.chomp!
  objname = File.join objdir, File.basename(filename) + ".o"

  return true if FileUtils.uptodate? objname, [filename] + cc_deps(filename, objdir, additional)

  mkdir_p objdir unless File.directory? objdir

  ccbin = cc_get_ccbin(filename)

  args = get_def_args(filename)
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-c"
  args << '"' + filename+ '"'
  args << "-o"
  args << '"' + objname + '"'
  cmdline = ccbin + ' ' + args.join(' ')
  cc_run(ccbin, args)
end

def cc_build(sources, objdir, additional = nil)
  
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
  puts "#{libname} is uptodate: #{FileUtils.uptodate?(libname, objects)}"
  FileUtils.uptodate?(libname, objects) or cc_run($arbin, ["crs", "\"#{libname}\""] + objects.collect { |x| "\"#{x}\"" })
end

def get_stl_link_args(abi)
  args = []
  args << "-L#{File.join($androidndkpath, "sources","cxx-stl","gnu-libstdc++",$ndkgccver,'libs',abi)}"
  args << "-lgnustl_static"
  args
end

def cc_link(outname, objects, additional = nil, deps = nil)
  dependencies = objects
  dependencies += deps unless deps.nil?
  return true if FileUtils.uptodate? outname, dependencies

  args = []
  if $ndkabi == "arm-eabi"
    args << "-nostdlib"
    args << "-Wl,-shared,-Bsymbolic"
  else
    args << "-shared"
  end
  args << "-Wl,--no-whole-archive"
  args << "-Wl,--no-undefined"
  args << "-Wl,-z,defs"
  args << "-fPIC"
  args << "-Wl,-soname,#{File.basename(outname)}"
  args << "--sysroot"
  args << $ndksysroot
  args << "-o"
  args << "\"#{outname}\""
  args += objects.collect { |x| "\"#{x}\""}
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-L#{$ndksysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$ndksysroot}/usr/lib"
  args << "#{$ndksysroot}/usr/lib/libc.so"
  args << "#{$ndksysroot}/usr/lib/libm.so"

  cc_run($gccbin, args)
end

def cc_clean(name)
  [$objdir[name], $libname[name]].each do |x|
    rm_rf x if File.exists? x
  end
end

def java_compile(outpath, classpath, srclist)
    javac = $config["env"]["paths"]["java"] + "/javac" + $exe_ext

    args = []
    args << "-g"
    args << "-d"
    args << outpath
    args << "-source"
    args << "1.6"
    args << "-target"
    args << "1.6"
    args << "-nowarn"
    args << "-encoding"
    args << "latin1"
    args << "-classpath"
    args << classpath
    args << "@#{srclist}"
    puts Jake.run(javac, args)
    unless $?.success?
        raise "Error compiling java code"
    end
end

def java_build(jarpath, buildpath, classpath, srclists)
    deps = []

    fullsrclist = nil

    if srclists.count == 1
      fullsrclist = srclists.first
      File.open(fullsrclist, "r") do |f|
          while line = f.gets
              line.chomp!
              deps << line.delete("\"")
          end
      end
    else
      fullsrclist = Tempfile.new 'RhodesSRC_build'
      srclists.each do |srclist|
        lines = []
        File.open(srclist, "r") do |f|
          while line = f.gets
            line.chomp!
            deps << line.delete("\"")
            fullsrclist.write "#{line}\n"
          end
        end
      end
      fullsrclist.close
      fullsrclist = fullsrclist.path
    end
    
    if FileUtils.uptodate?(jarpath, deps)
      puts "#{jarpath} is uptodate: true"
      return
    end

    puts "Compiling java sources: #{srclists.inspect}"

    java_compile(buildpath, classpath, fullsrclist)
    
    args = ["cf", jarpath, '.']
    Jake.run($jarbin, args, buildpath)
    unless $?.success?
        raise "Error creating #{jarpath}"
    end

end

def apk_build(sdk, apk_name, res_name, dex_name, debug)
    puts "Building APK file..."
    prev_dir = Dir.pwd
    Dir.chdir File.join(sdk, "tools")

    params = ['-Xmx1024m', '-classpath', $sdklibjar, 'com.android.sdklib.build.ApkBuilderMain', apk_name]
    if debug
        params += ['-z', res_name, '-f', dex_name]
    else
        params += ['-u', '-z', res_name, '-f', dex_name]
    end
    
    Jake.run File.join($java, 'java'+$exe_ext), params
    unless $?.success?
        Dir.chdir prev_dir
        raise 'Error building APK file'
    end
    Dir.chdir prev_dir
end

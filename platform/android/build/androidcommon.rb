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
require 'pathname'
require_relative 'ndkwrapper'
require_relative 'hostplatform'

#common functions for compiling android
#
# uses following globals
# $objdir[]       list of paths to compiled library object files
# $libname[]      list of paths to compiled library
# $debug          set to true to enable debug binaries

USE_TRACES = Rake.application.options.trace

$output_lock = Mutex.new

def num_cpus
  HostPlatform.num_cpus
end

def get_sources(sourcelist)
    res = File.read(sourcelist).split(/\r?\n/)
end

def get_objects(sources, objdir)
    sources.map { |src| File.join(objdir, File.basename(src) + ".o") }    
end

def setup_ndk(ndkpath,apilevel,abi)
  puts "setup_ndk(#{ndkpath}, #{apilevel}, #{abi})" if USE_TRACES
  $apilevel = apilevel
  ndk = NDKWrapper.new( ndkpath )
  
  tools = ndk.detect_toolchain abi
  tools.each { |name, path| eval "$#{name}bin = path" }  

  $ndksysroot = ndk.sysroot apilevel, abi 

  $ndkgccver = ndk.gccver
  $androidndkpath = ndkpath unless $androidndkpath

  $sysincludes = ndk.sysincludes apilevel, abi
  $link_sysroot = ndk.link_sysroot apilevel, abi

  puts "NDK sysroot: #{$ndksysroot}, linker sysroot: #{$link_sysroot}, GCC v#{$ndkgccver}, sysincludes: #{$sysincludes}"

  # Detect rlim_t
  if $have_rlim_t.nil?
    $have_rlim_t = false
    resource_h = File.join(ndkpath, 'build', 'platforms', "android-#{apilevel}", "arch-#{abi}", "usr", "include", "sys", "resource.h")
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
    args << "-isystem #{$sysincludes}" if $sysincludes    
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
    args << "-D__ANDROID_API__=#{$apilevel}" if $apilevel
    args << "-DHAVE_RLIM_T" if $have_rlim_t
    unless $debug
      args << "-O2"
      args << "-DNDEBUG"
    else
      args << "-O0"
      args << "-g"
      args << "-D_DEBUG"
      args << "-fstack-protector-all"
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
    
    dirArmeabi = File.join($androidndkpath,'sources','cxx-stl','gnu-libstdc++',$ndkgccver,'libs','armeabi','include')
    if !File.directory?(dirArmeabi)
      dirArmeabi = File.join($androidndkpath,'sources','cxx-stl','gnu-libstdc++',$ndkgccver,'libs','armeabi-v7a','include')
    end
    args << "-I\"#{dirArmeabi}\""
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

def cc_run(command, args, chdir = nil, coloring = true, env = nil, verbose = true)
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
      puts out.string if verbose
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
  #args << "-lgnustl_static"
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
  #args << "-static-libstdc++"
  args << "-Wl,--no-whole-archive"
  args << "-Wl,--no-undefined"
  args << "-Wl,-z,defs"
  args << "-fPIC"
  args << "-Wl,-soname,#{File.basename(outname)}"
  args << "--sysroot"
  args << $link_sysroot
  args << "-o"
  args << "\"#{outname}\""
  args += objects.collect { |x| "\"#{x}\""}
  args += additional if additional.is_a? Array and not additional.empty?
  args << "-L#{$link_sysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$link_sysroot}/usr/lib"
  args << "#{$link_sysroot}/usr/lib/libc.so"
  args << "#{$link_sysroot}/usr/lib/libm.so"
  #args << "#{$link_sysroot}/usr/lib/libstdc++.so"

  localabi = "armeabi"
  if $gccbin.include? "toolchains/x86"
    localabi = "x86"
  end
  if $gccbin.include? "toolchains\\x86"
    localabi = "x86"
  end
  
  #libandroid_support = File.join($androidndkpath, "sources", "cxx-stl", "llvm-libc++", "libs", localabi)
  #if File.exists? libandroid_support
  #  args << "-L\"#{libandroid_support}\""
  #  args << "-landroid_support"
  #  puts "libandroid_support exists"
  #else
  #  localabi = "armeabi-v7a"
  #  libandroid_support = File.join($androidndkpath, "sources", "cxx-stl", "llvm-libc++", "libs", localabi)
  #  if File.exists? libandroid_support
  #    args << "-L\"#{libandroid_support}\""
  #    args << "-landroid_support"
  #    puts "libandroid_support exists"
  #  else
  #    puts "libandroid_support does not exists"
  #  end
  #end

  libgnustl_static = File.join($androidndkpath, "sources", "cxx-stl", "gnu-libstdc++", "4.9", "libs", localabi)
  if File.exists? libgnustl_static
    args << "-L\"#{libgnustl_static}\""
    args << "-lgnustl_static"
    puts "libgnustl_static exists"
  else
    localabi = "armeabi-v7a"
    libgnustl_static = File.join($androidndkpath, "sources", "cxx-stl", "gnu-libstdc++", "4.9", "libs", localabi)
    if File.exists? libgnustl_static
      args << "-L\"#{libgnustl_static}\""
      args << "-lgnustl_static"
      puts "libgnustl_static exists"
    else
      puts "libgnustl_static does not exists"
    end
  end

  cc_run($gccbin, args)
end

def cc_clean(name)
  [$objdir[name], $libname[name]].each do |x|
    rm_rf x if File.exists? x
  end
end

def java_compile(outpath, classpath, srclist)
    javac = $config["env"]["paths"]["java"] + "/javac#{HostPlatform.exe_ext}"

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

    #append buildpath with .java so no unwanted artefacts included into final jar
    buildpath = File.join(buildpath,'.java')
    mkdir_p buildpath unless File.directory? buildpath

    java_compile(buildpath, classpath, fullsrclist)

    args = []

    if USE_TRACES
      args << "cfv"
    else
      args << "cf"
    end

    args << jarpath
    args << '.'

    $logger.debug "java_build args: #{args}"
    
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

    params += ['-u', '-z', res_name, '-f', dex_name]

    Jake.run File.join($java, "java#{HostPlatform.exe_ext}"), params
    unless $?.success?
        Dir.chdir prev_dir
        raise 'Error building APK file'
    end
    Dir.chdir prev_dir
end

require_relative 'hostplatform'

class NDKWrapper

  @root_path = nil

  @rev_major = nil
  @rev_minor = nil
  @rev_tiny  = nil

  @gccver = "unknown"  

  def self.create( ndk_path )
  end

  def initialize( ndk_path )
    @root_path = ndk_path
    parse_properties
  end

  def is_valid_ndk?
    not (@rev_major.nil? or @rev_minor.nil? or @rev_tiny.nil?)
  end

  def parse_properties
    File.open( File.join( @root_path, 'source.properties' ) ).each { |line|
      prop,val = line.strip.split('=')
      prop.strip!.downcase!
      val.strip!.downcase!

      case prop
      when 'pkg.desc'
        parse_desc val
      when 'pkg.revision'
        parse_rev val
      end
    }
  end

  def parse_desc( val )
    raise "Not an NDK location: #{@root_path}" unless val == 'android ndk'
  end

  def parse_rev( val )
    @rev_major, @rev_minor, @rev_tiny = val.split('.').map { |v| v.to_i }
  end

  def link_sysroot( api, abi )
    linkabi = abi == "aarch64" ? "arm64" : abi
    #locate closest available NDK platform for target API
    n_api = api.to_i
    n_api.downto(1) { |i|
      path = File.join( @root_path, 'platforms', "android-#{i.to_s}", "arch-#{linkabi}" )

      return path if File.directory?(path)
    }

    raise "Unable to detect NDK link root for API: #{api}, ABI: #{abi}"      
  end

  def link_sysroot_level_ext( api, abi )

    if @rev_major >= 18
      triple = {
        'arm'   => 'arm-linux-androideabi',
        'aarch64' => 'aarch64-linux-android',
        'mips'  => 'mipsel-linux-android',
        'mips64'=> 'mips64el-linux-android',
        'x86'   => 'i686-linux-android',
        'x86_64'=> 'x86_64-linux-android'
      }
      File.join( sysroot_18, 'usr', 'lib', triple[abi], api.to_s)
    else
      nil
    end
  end

  def sysroot( api, abi )
    if @rev_major >= 18
      sysroot_18
    elsif @rev_major >= 16
      sysroot_16 api,abi
    else
      sysroot_pre_16 api,abi
    end
  end

  def sysroot_16( api, abi )
    File.join( @root_path, 'sysroot' )
  end

  def toolchain
    $toolchain
  end

  def sysroot_18
    ndkhostvariant = ''
    if HostPlatform.windows?
      ndkhostvariant = 'windows-x86_64' if $bufcheck64 and $bufcheck64.include?('64')
    else
      ndkhostvariant = `uname -s`.downcase!.chomp! + "-" + `uname -m`.chomp!
    end

    return File.join( @root_path, 'toolchains', 'llvm', 'prebuilt', ndkhostvariant, 'sysroot')
  end

  def sysroot_pre_16( api, abi )
    variants = []
    variants << "platforms"
    variants << File.join("build", "platforms")

    api_levels = Array.new

    max_ndk_api_level = 19 #we use some functions missing from API 20 and forth

    variants.each do |variant|
      puts "Check NDK folder: #{variant}" if Rake.application.options.trace
      Dir.glob(File.join(@root_path, variant, "*")).each do |platform|
        sys_root = File.join platform, "arch-arm"
        puts "Checking #{sys_root} for NDK nsysroot"  if Rake.application.options.trace
        next unless File.directory? sys_root
        next unless platform =~ /android-([0-9]+)$/
        api_level = $1.to_i 
        api_levels.push api_level if (api_level<=max_ndk_api_level)
        puts "NDK API level: #{api_level}" if Rake.application.options.trace
      end
    end
    
    api_levels.sort!

    last_api_level = 0
    api_levels.each do |cur_api_level|
      puts "Checking is API level enough: #{cur_api_level}"  if Rake.application.options.trace
      break if cur_api_level > max_ndk_api_level
      last_api_level = cur_api_level
    end

    sys_abi_root = nil
    variants.each do |variant|
      sys_abi_root = File.join(@root_path, variant, "android-#{last_api_level}/arch-#{abi}")
      next unless File.directory? sys_abi_root      
      break
    end
    if sys_abi_root.nil?
      raise "Can't detect NDK sysroot (corrupted NDK installation?)"
    end

    sys_abi_root
  end

  def detect_toolchain( abi )
    ndktools = nil
    ndkabi = "unknown"
    ndkhostvariants = []
    if HostPlatform.windows?
        $bufcheck64 = `WMIC OS get OSArchitecture`.encode('UTF-8', invalid: :replace, replace: "").split[1]
        ndkhostvariants << 'windows-x86_64' if $bufcheck64 and $bufcheck64.include?('64')
        ndkhostvariants << 'windows'
    else
        ndkhostvariants = [
          `uname -s`.downcase!.chomp! + "-" + `uname -m`.chomp!, 
          `uname -s`.downcase!.chomp! + '-x86'
        ]
    end

    if(@rev_major >= 18)
      toolchainversions = ['8.0.0']
    else
      toolchainversions = ['4.8','4.9']
    end

    $toolchain = 'unknown-toolchain'
    if abi == 'arm'
      if @rev_major >= 18
        $toolchain = 'armv7a-linux-androideabi'
      else
        $toolchain = 'arm-linux-androideabi'
      end
    elsif abi == 'x86'
      if @rev_major >= 18
        $toolchain = 'i686-linux-android'
      else
        $toolchain = 'x86'
      end
    elsif abi == 'x86_64'
      if @rev_major >= 18
        $toolchain = 'x86_64-linux-android'
      else
        $toolchain = 'x86_64'
      end
    elsif abi == 'mips'
      if @rev_major >= 18
        raise "Mips not supported!"
      else
        $toolchain = 'mipsel-linux-android'
      end
    elsif abi == 'aarch64'
      if @rev_major >= 18
        $toolchain = 'aarch64-linux-android'
      else
        raise "Aarch64 not supported for this ndk version!"
      end
    else
      raise "Unknown ABI: {abi}";
    end

    ndkhostvariants.each do |ndkhost|
      puts "Checking toolchain for host: #{ndkhost}" if Rake.application.options.trace

      toolchainversions.each do |version|
        variants = []

        if(@rev_major >= 18)
          variants << File.join(@root_path,'toolchains','llvm','prebuilt',ndkhost)
        else
          variants << File.join(@root_path,'build','prebuilt',ndkhost,"#{$toolchain}-#{version}")
          variants << File.join(@root_path,'toolchains',"#{$toolchain}-#{version}",'prebuilt',ndkhost)
        end


        variants.each do |variant|
          puts "Check toolchain path: #{variant}"# if Rake.application.options.trace    
          next unless File.directory? variant

          ndktools = variant
          ndkabi = $toolchain
          @gccver = version
          
          ndkabi = 'i686-linux-android' if ndkabi == 'x86'
          ndkabi = 'x86_64-linux-android' if ndkabi == 'x86_64'

          puts "Toolchain is detected: #{ndktools}, abi: #{ndkabi}, version: #{@gccver}" #if Rake.application.options.trace

          tools = {}
          
          [ 'gcc', 'g++', 'ar', 'strip', 'objdump'].each do |tool|
              name = tool.gsub('+', 'p')
              tools[name] = check_tool( tool, ndktools, ndkabi)
          end

          return tools
        end
      end
    end

    if ndktools.nil?
      raise "Can't detect NDK toolchain path (corrupted NDK installation?)"
    end 
  end

  def gccver
    @gccver
  end

  def rev_major
    @rev_major
  end

  def check_tool( tool, ndktoolsdir, abi )
    if(@rev_major >= 18)
      abi = 'arm-linux-androideabi' if abi == 'armv7a-linux-androideabi'
    end

    toolpath = File.join(ndktoolsdir,'bin',"#{abi}-#{tool}#{HostPlatform.exe_ext}")
    if(@rev_major >= 18)
      if (tool == 'gcc')
        toolpath = File.join(ndktoolsdir,'bin',"clang#{HostPlatform.exe_ext}")
      end
      if (tool == 'g++')
        toolpath = File.join(ndktoolsdir,'bin',"clang++#{HostPlatform.exe_ext}")
      end
    end
    puts "Checking tool path #{toolpath} for tool #{tool}" if Rake.application.options.trace
    
    if File.file? toolpath
      return toolpath
    else
      raise "Can't find tool #{tool} at path #{toolpath} (corrupted NDK installation or unsupported NDK?)"
    end
  end

  def sysincludes(api,abi)
    if @rev_major >= 16
      #https://android.googlesource.com/platform/ndk/+/ndk-release-r16/docs/UnifiedHeaders.md
      triple = {
        'arm'   => 'arm-linux-androideabi',
        'aarch64' => 'aarch64-linux-android',
        'mips'  => 'mipsel-linux-android',
        'mips64'=> 'mips64el-linux-android',
        'x86'   => 'i686-linux-android',
        'x86_64'=> 'x86_64-linux-android'
      }
      if  @rev_major >= 18
        File.join( sysroot_18, 'usr', 'include', triple[abi])
      else
        File.join( sysroot_16(api,abi), 'usr', 'include', triple[abi])
      end
    else
      nil
    end
  end

  private :parse_properties, :parse_desc, :parse_rev, :initialize, :sysroot_16, :sysroot_pre_16, :check_tool
end
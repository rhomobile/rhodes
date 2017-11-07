require 'zip'
require 'singleton'

module AndroidTools

class MavenDepsExtractor
  include Singleton

  @logger = nil

  def set_logger( logger )
    @logger = logger
  end

  def initialize

    @dependencies = {}

    @rhoroot = File.join( File.dirname(__FILE__),'..','..','..')
    @m2home = File.join( @rhoroot, 'res', 'build-tools', 'maven' )
    @mvnbin = File.join( @m2home, 'bin', 'mvn' )

    if RUBY_PLATFORM =~ /(win|w)32$/
      @mvnbin += '.bat'
    end

    @jars = []
    @res_dirs = []
    @asset_dirs = []
    @jni_libs = []
    @manifests = []

    @have_v4_support_lib = false

  end

=begin
  # leaving this just in case, since maven will resolve dep versions itself

  def compare_versions( v1, v2 )
    version1 = v1.split('.').map { |s| s.to_i }
    version2 = v2.split('.').map { |s| s.to_i }

    version1.each_with_index do |v,i|
      diff = v - version2[i]
      return diff if (diff != 0)
    end

    return 0
  end

  #return true if have deps with version higher or equal than required
  def remove_dependencies_with_lesser_versions( dep )
    depGroup,depPackage,depVersion = split_dependency(dep)

    @logger.info "Will filter out dependecies #{depGroup}:#{depPackage} if version is less than #{depVersion}"

    haveExistingDependencyWithNonLessVersion = false
    @dependencies.each do |extension,dependencies|
      dependencies.delete_if do |dependency|
        group,package,version = split_dependency(dependency)
        if (group==depGroup) and (package==depPackage)
          @logger.info "Found already existing dependency #{dependency} for module #{extension}, will check version"
          if ( compare_versions(depVersion,version) > 0 )
            @logger.info "New dependency has higher version than existing one: #{depVersion} > #{version}. Will replace it."
            #remove existing dependency
            true
          else
            @logger.info "Existing dependency version is higher or equal than new one: #{depVersion} <= #{version}"
            haveExistingDependencyWithNonLessVersion = true
          end          
        end        
      end
    end
    return haveExistingDependencyWithNonLessVersion
  end
=end

  def add_dependency( mod=nil, dep )
    if !mod
      mod = '.rho'
    end

    @logger.info "Adding maven dependency for #{mod}: #{dep}"
    #return if remove_dependencies_with_lesser_versions(dep)

    if !@dependencies[mod]
      @dependencies[mod] = [ dep ]
    else
      @dependencies[mod] << dep
    end
  end

  def get_dependencies
    @dependencies
  end

  def set_temp_dir( d )
    raise "Maven extractor directory was already set to #{@temp_dir}" if @temp_dir

    @temp_dir = File.join(d,'.m2')

    rm_r @temp_dir if File.directory?(@temp_dir)
    mkdir_p(@temp_dir)
  end

  def set_java_home ( d )
    @java_home = File.dirname(d) #no /bin part
  end

  def extract_all
    @logger.info 'Extracting maven dependencies'

    @logger.info 'Removing maven cache for android packages - just in case'
    com_cache_dir = File.join(@m2home,'m2','com')
    rm_r com_cache_dir if File.directory?(com_cache_dir)

    copy_dir = File.join( @temp_dir, '.tmp')
    mkdir_p copy_dir    
    copy_dependencies copy_dir

    @logger.info "Extracting dependencies"
    extract copy_dir, @temp_dir

    rm_r copy_dir   
  end

  def extract( src, dst )
    @logger.info "Extracting dependencies to #{dst}"

    Dir[File.join(src,'*')].each do |f|

      @have_v4_support_lib = true if ( File.basename(f) =~ /support-v4/ )

      if File.extname(f) == '.aar'        
        target = File.join(dst,File.basename(f,'.aar'))

        if !File.exist?( target )
          @logger.debug "Dependency artefact #{f} is AAR so will unzip it to #{target}"
          mkdir_p target
          Jake.unzip(f,target)
          rm f

          assets = File.join(target,'assets')
          manifest = File.join(target,'AndroidManifest.xml')
		  r_txt = File.join(target,'R.txt')
          res = File.join(target,'res')
          libs = File.join(target,'libs')
          jni = File.join(target,'jni')
          classes = File.join(target,'classes.jar')

          @asset_dirs << assets if (File.directory?(assets) and !Dir[File.join(assets,'*')].empty? )
          @manifests << manifest if File.file?(manifest) and File.size?(r_txt)
          @res_dirs << res if (File.directory?(res) and !Dir[File.join(res,'*')].empty?)
          @jars += Dir[File.join(libs,'*.jar')]
          @jars << classes if File.file?(classes)
          @jni_libs += Dir[(File.join(jni,'**','*.so'))]

        end
      elsif File.extname(f) == '.jar'
        target = File.join(dst,File.basename(f))
        if !File.exist?(target)
          mv(f,target) 
          @jars << target
        end
      end        
    end

  end

  def maven_env
    { 'JAVA_HOME' => @java_home, 'M2_HOME' => @m2home }
  end

  def dep_copy_cmd
    argv = []
    argv << @mvnbin
    argv << 'dependency:copy-dependencies'
#    argv << '-o' #work offline
    argv << "-Dmaven.repo.local=#{File.join(@m2home,'m2')}"
    argv << '-e' if USE_TRACES
    argv << '-X' if USE_TRACES
  
    argv.join(' ')
  end

  def copy_dependencies( path )    
    pom = File.join( path, 'pom.xml')
    generate_pom(pom)

    cmd = dep_copy_cmd
    env = maven_env
    pwd = path

    @logger.info "Running Maven dependency copy plugin"
    Jake.run3( cmd, pwd, env )
    rm pom

    @logger.debug "Moving extracted dependencies to #{path}"
    Dir[File.join(path,'target','dependency','*')].each { |f| mv(f,path) }
    rm_r File.join(path,'target')
  end

  def split_dependency( dep )
    dep.split(':')
  end

  def generate_pom(path)
    require 'erb'

    @logger.info "Generating pom.xml for dependencies at #{path}"

    dependencies = @dependencies.values.flatten.map { |d| Hash[ ["grp_id","art_id","ver"].zip(split_dependency(d)) ] }

    repos = [
      File.join($androidsdkpath,'extras','google','m2repository'),
      File.join($androidsdkpath,'extras','android','m2repository')
    ]

    tpl = File.read(File.join(@rhoroot,'platform','android','build','pom.erb'))
    erb = ERB.new tpl
    f = File.open( path,'w')
    f.write(erb.result binding)
    f.close
  end

  def aapt_args
    args = []
=begin    
    @jars.each do |j|
      args << '-I'
      args << j
    end
=end
    @res_dirs.each do |d|
      args << '-S'
      args << d
    end

    args << "--auto-add-overlay" if !@res_dirs.empty?

    return args

  end

  def jars
    @jars
  end
  
  def extract_packages
     require 'nokogiri'
     packages = []
     @manifests.each do |m|
       doc = File.open(m) { |f| Nokogiri::XML(f) }
	   attr = doc.xpath("//manifest").attr("package")
	   packages << attr.to_s
     end
	 return packages 
  end

  def classpath(separator)
    @jars.join(separator)
  end

  def have_v4_support_lib?
    @have_v4_support_lib
  end

end

end
require 'zip'
require 'singleton'

module AndroidTools

class MavenDepsExtractor
  include Singleton

  def initialize
    $logger.debug "MavenDepsExtractor.initialize"

    @dependencies = {}

    rhoroot = File.join( File.dirname(__FILE__),'..','..','..')
    @m2home = File.join( rhoroot, 'res', 'build-tools', 'maven' )
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

  def add_dependency( mod=nil, dep )
    if !mod
      mod = '.rho'
    end

    $logger.info "Adding maven dependency for #{mod}: #{dep}"

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
    $logger.info 'Extracting maven dependencies'

    $logger.info 'Removing maven cache for android packages - just in case'
    com_cache_dir = File.join(@m2home,'m2','com')
    rm_r com_cache_dir if File.directory?(com_cache_dir)

    @dependencies.each do |name,deps|
      $logger.debug "Extracting dependencies for extension #{name}"

      path = @temp_dir#File.join( @temp_dir, name )
      #mkdir_p( path )
      deps.each { |dep| extract( dep, path ) }
    end
  end

  def extract( dep, path )
    $logger.debug "Extracting dependency #{dep} to #{path}"

    extract_dir = File.join( @temp_dir, '.tmp')
    mkdir_p extract_dir

    copy_dependency( dep, extract_dir )

    $logger.debug "Processing dependencies"

    Dir[File.join(extract_dir,'*')].each do |f|

      @have_v4_support_lib = true if ( File.basename(f) =~ /support-v4/ )

      if File.extname(f) == '.aar'        
        target = File.join(path,File.basename(f,'.aar'))

        if !File.exist?( target )
          $logger.debug "Dependency artefact #{f} is AAR so will unzip it to #{target}"
          mkdir_p target
          Jake.unzip(f,target)
          rm f

          assets = File.join(target,'assets')
          manifest = File.join(target,'AndroidManifest.xml')
          res = File.join(target,'res')
          libs = File.join(target,'libs')
          jni = File.join(target,'jni')
          classes = File.join(target,'classes.jar')

          raise "ERROR: res directory does not exist for AAR package" if !File.directory?( res )
          raise "ERROR: classes.jar does not exist for AAR package" if !File.exist?( classes )
          raise "ERROR: AndroidManifest.xml does not exist for AAR package" if !File.exist?( manifest )

          @asset_dirs << assets if (File.directory?(assets) and !Dir[File.join(assets,'*')].empty? )
          @manifests << manifest
          @res_dirs << res if !Dir[File.join(res,'*')].empty?
          @jars += Dir[File.join(libs,'*.jar')]
          @jars << classes
          @jni_libs += Dir[(File.join(jni,'**','*.so'))]

        end
      elsif File.extname(f) == '.jar'
        target = File.join(path,File.basename(f))
        if !File.exist?(target)
          mv(f,target) 
          @jars << target
        end
      end        
    end

    rm_r extract_dir
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

  def copy_dependency( dep, path )
    pom = File.join( path, 'pom.xml')
    generate_pom( dep, pom )

    cmd = dep_copy_cmd
    env = maven_env
    pwd = path

    $logger.info "Running Maven dependency copy plugin"
    Jake.run3( cmd, pwd, env )
    rm pom

    $logger.debug "Moving extracted dependencies to #{path}"
    Dir[File.join(path,'target','dependency','*')].each { |f| mv(f,path) }
    rm_r File.join(path,'target')
  end

  def split_dependency( dep )
    dep.split(':')
  end

  def generate_pom(dep, path)

    $logger.debug "Generating POM file for #{dep} in #{path}"

    dep_grp_id, dep_art_id, dep_ver = split_dependency( dep )

    r1 = File.join($androidsdkpath,'extras','google','m2repository')
    r2 = File.join($androidsdkpath,'extras','android','m2repository')

    pom =
      "<project xmlns='http://maven.apache.org/POM/4.0.0'\n"\
      "  xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'\n"\
      "  xsi:schemaLocation='http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd'>\n"\
      "  <modelVersion>4.0.0</modelVersion>\n"\
      "  <groupId>com.rhomobile.rhodes</groupId>\n"\
      "  <artifactId>deps-extractor</artifactId>\n"\
      "  <version>1.0-SNAPSHOT</version>\n"\
      "  <name>Maven Quick Start Archetype</name>\n"\
      "  <url>http://maven.apache.org</url>\n"\
      "  <dependencies>\n"\
      "    <dependency>\n"\
      "      <groupId>#{dep_grp_id}</groupId>\n"\
      "      <artifactId>#{dep_art_id}</artifactId>\n"\
      "      <version>#{dep_ver}</version>\n"\
      "      <type>aar</type>\n"\
      "    </dependency>\n"\
      "  </dependencies>\n"\
      "  <repositories>\n"\
      "    <repository>\n"\
      "      <id>repo1</id>\n"\
      "      <name>android-maven-repo-1</name>\n"\
      "      <url>file://#{r1}</url>\n"\
      "    </repository>\n"\
      "    <repository>\n"\
      "      <id>repo2</id>\n"\
      "      <name>android-maven-repo-2</name>\n"\
      "      <url>file://#{r2}</url>\n"\
      "    </repository>\n"\
      "  </repositories>\n"\
      "  <build>\n"\
      "    <plugins>\n"\
      "      <plugin>\n"\
      "        <groupId>org.apache.maven.plugins</groupId>\n"\
      "        <artifactId>maven-dependency-plugin</artifactId>\n"\
      "        <version>2.10</version>\n"\
      "        <executions>\n"\
      "          <execution>\n"\
      "            <id>unpack-dependencies</id>\n"\
      "            <phase>package</phase>\n"\
      "            <goals>\n"\
      "              <goal>unpack-dependencies</goal>\n"\
      "            </goals>\n"\
      "            <configuration>\n"\
      "              <!-- configure the plugin here -->\n"\
      "              <outputDirectory>.</outputDirectory>\n"\
      "            </configuration>\n"\
      "          </execution>\n"\
      "        </executions>\n"\
      "      </plugin>\n"\
      "    </plugins>\n"\
      "  </build>\n"\
      "</project>"

    f = File.open( path,'w')
    f.write( pom )
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

  def classpath(separator)
    @jars.join(separator)
  end

  def have_v4_support_lib?
    @have_v4_support_lib
  end

end

end
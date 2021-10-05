require_relative 'android_tools'
require_relative 'maven_deps_extractor'
require 'fileutils'
require 'zip'
require 'pathname'
require 'singleton'
require 'securerandom'
require 'json'

class AabBuilder
    include Singleton

    attr_accessor :output_path, :res_dir, :dex_path, :apk_path, :manifest, :logger, :aapt2, :build_dir, :androidjar, :bundletool, :javabin, :rjava_dir, :maven_deps, :no_compress_exts
    attr_accessor :keystore, :storealias, :storepass, :keypass, :debug

    #This is the final step, run it from android.rake after all is done:
    #- resources
    #- manifest
    #- native code
    #- java code
    #- dependencies and extensions
    #
    # Provide old-style APK for input, we'll take SOs, assets and other files from there. Also provide compiled DEX.
    # Manifest and resources will be taken from build_resources step
    def build
        validate

        prepare_bundle_folder_struct
        
        extract_protobuf_data_to_bundle_prep_dir

        extract_legacy_data_to_bundle_prep_dir
        
        build_base_zip   
        
        create_config_file

        #Finally build AAB archive from prepared base.zip
        @bundle = File.join(@intermediate,'bundle.aab')
        args = [ '-jar', @bundletool, 'build-bundle', "--modules=#{@base_zip}", "--output=#{@bundle}" , "--config=#{@config_file}"]
        Jake.run( @javabin, args )
    end

    def create_config_file       

        cfg = {}
        cfg['compression'] = {}
        cfg['compression']['uncompressedGlob'] = 
            [ 
                "res/raw/**",                                               #don't compress any raw resources
            ] + @no_compress_exts.map { |ext| "assets/**.#{ext}" }          #don't compress specific extensions in assets
        
        @config_file = File.join(@intermediate,'config.json')
        File.open( @config_file, 'w') { |f| f.write( cfg.to_json ) }
    end

    def build_base_zip
        root = Pathname.new(@prep_bundle_dir)

        @base_zip = File.join( @intermediate, 'base.zip' )
        Zip::File.open( @base_zip, Zip::File::CREATE) { |z|
            Dir[File.join(@prep_bundle_dir,'**','*')].filter{|f| File.file? f}.each{ |f|
                p = Pathname.new(f)
                z.add( p.relative_path_from(root) ,f)
            }
        }
    end

    def extract_legacy_data_to_bundle_prep_dir
        Zip::File.open(@apk_path) { |zip_file|

            zip_file.each {|f|

                next if f.name=='AndroidManifest.xml'
                next if f.name=='resources.arsc'
                next if f.name.start_with? 'res/'

                if (f.name.start_with? 'assets/')
                    dst = @prep_bundle_dir                
                elsif (f.name.start_with? 'lib/')
                    next if (File.extname(f.name) != '.so')
                    dst = @prep_bundle_dir                
                else
                    dst = File.join(@prep_bundle_dir,'root')
                end
                
                fpath = File.join(dst,f.name)
                dir = File.dirname(fpath)
                mkdir_p dir unless File.directory? dir
                zip_file.extract(f,fpath)
            }
        }

        #put prepared DEX to bundle dir
        cp @dex_path, File.join(@prep_bundle_dir,'dex')
    end

    def extract_protobuf_data_to_bundle_prep_dir
        Zip::File.open(@pbprep) { |zip_file|

            zip_file.extract('AndroidManifest.xml', File.join( @prep_bundle_dir,'manifest', 'AndroidManifest.xml') )
            zip_file.extract('resources.pb', File.join( @prep_bundle_dir,'resources.pb') )

            zip_file.filter {|f| f.name.start_with?('res/') }.each { |f|            
                fpath = File.join( @prep_bundle_dir,f.name)
                dir = File.dirname(fpath)
                mkdir_p dir unless File.directory? dir
                zip_file.extract(f,fpath)
            }
        }
    end

    def prepare_bundle_folder_struct
        @prep_bundle_dir = File.join( @intermediate, 'prep_bundle_dir')
        mkdir_p(@prep_bundle_dir)
        mkdir_p(File.join(@prep_bundle_dir,'manifest'))
        mkdir_p(File.join(@prep_bundle_dir,'dex'))
        mkdir_p(File.join(@prep_bundle_dir,'res'))
        mkdir_p(File.join(@prep_bundle_dir,'root'))
        mkdir_p(File.join(@prep_bundle_dir,'assets'))
        mkdir_p(File.join(@prep_bundle_dir,'lib'))
    end

    def compile_res_dir_to_flat( dir )
        flatsdir = File.join(@intermediate, 'flats')
        mkdir_p flatsdir unless File.directory? flatsdir

        #make random dir for each target resource dir
        target = File.join( flatsdir, SecureRandom.uuid )
        raise "Resource target #{target} should not exist at that point" if File.exists?(target)
        mkdir_p target

        Dir[File.join(dir,'**','*')]
            .filter { |f| File.file?(f) }
                .map { |f| PathToWindowsWay(f) }
                    .each { |f|
                        Jake.run( @aapt2, ['compile', f, '-o', target ] )    
                    }

        #return *.flat list
        Dir[File.join(target,'**','*.flat')]        
    end

    #This is the first step to run. Builder must be initialized at this point
    #It will compile resources and generate/put R.java to specified dir so android.rake can use it for other build steps
    def build_resources
        validate

        @logger.debug "Resource dirs for AAB builder: #{@res_dir}"

        flat_list = []

        flat_list += compile_res_dir_to_flat(@res_dir)

        #Add all resources from maven dependencies
        @maven_deps.res_dirs.each {|d|
            flat_list += compile_res_dir_to_flat(d)
        }

        reslist = File.join(@intermediate,'flat.txt')
        File.open( reslist, 'w' ) { |f| f.write flat_list.join(' ') }

        #prepared archive name with protobuf data
        @pbprep = File.join(@intermediate,'output.apk')

        #make a dir to put generated R.java
        rdir = File.join(@intermediate,'rjava')
        mkdir_p (rdir)

        args = [ 'link', 
            '--proto-format', 
            '-o', @pbprep, 
            '-I', @androidjar, 
            '--manifest', @manifest, 
            '-R', '@'+File.join(reslist),
            '--auto-add-overlay',
            '--java',            
            PathToWindowsWay(rdir)
        ]        

        if @no_compress_globs
            @no_compress_globs.each do |ext|
              args << '-0'
              args << ext
            end
          end
        
        #Finally run link to generate R.java for compiled resources. We need manifest ready at this point
        Jake.run( @aapt2, args )       

        mkdir_p @rjava_dir
        #We don't really care of package name and relative path where R.java is located
        #We just need to put generated R.java to specified @rjava_dir, so let's just find generated R.java and put it to target dir
        Dir[ File.join(rdir,'**','R.java') ].each { |f|
            cp_r( f, @rjava_dir )
        }        
    end

    def init
        init_dirs
    end

    def init_dirs
        @intermediate = File.join( @build_dir, '.bundle')
        FileUtils.rm_r @intermediate if File.directory? @intermediate
        FileUtils.mkdir_p @intermediate

    end

    def validate
        raise 'AAB Builder set up incorrectly' unless defined? @res_dir and
            defined? @output_path and
            defined? @dex_path and
            defined? @apk_path and
            defined? @manifest and
            defined? @logger and
            defined? @aapt2 and
            defined? @build_dir and
            defined? @androidjar and
            defined? @bundletool and
            defined? @javabin and
            defined? @rjava_dir and
            defined? @maven_deps and
            defined? @no_compress_exts
    end

    def sign(target)
        if @debug
            AndroidTools.signAabDebug @bundle, target
          else
            AndroidTools.signAab @bundle, target, $keystore, $keypass, $storepass, $storealias
          end
    end
end
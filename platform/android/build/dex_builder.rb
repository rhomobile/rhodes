require_relative '../../../lib/build/jake.rb'
require_relative 'hostplatform.rb'
require 'singleton'

module AndroidTools

    class DexBuilder
      include Singleton

      attr_accessor :logger, :sdk_path, :build_tools_path, :androidplatform, :javabin

      def build( jarlist = [], outdex )
        tools_path = detect_tools_path

        bin,args = build_cmd_line(tools_path, jarlist, outdex)

        @logger.debug "BIN: #{bin}, ARGS: #{args}"
        return false unless bin

        Jake.run(bin,args)
        $?.success?
      end

      def detect_tools_path
        
        if (@build_tools_path)
            path = @build_tools_path
        else
            path_guess_1 = File.join(@sdk_path, "platforms", @androidplatform, "tools")
            if (File.directory?(path_guess_1))
                path = path_guess_1                
            else
                path_guess_2 = File.join(@sdk_path, "platform-tools")
                if (File.directory?(path_guess_2))
                    path = path_guess_2
                end
            end

        end

        path
      end

        def build_cmd_line(tools_path, jarlist, outdex)

            @logger.error( "DEX builder: path to build tools not set" ) unless @build_tools_path

            @logger.info( "DEX builder: looking for DEX tool in #{@build_tools_path}")

            #try to find dx.jar first
            dexjar = File.join(@build_tools_path,"lib","dx.jar")
            if File.file?(dexjar)

                @logger.info( "DEX builder: found dx.jar")

                args = []
                args << "-Xmx1024m"
                args << "-jar"
                args << dexjar
                args << "--dex"
                args << "--output=#{outdex}"
                args.concat jarlist

                return @javabin, args
                
            else #then go for d8
                d8 = File.join(@build_tools_path,"d8"+HostPlatform::bat_ext)                
                if File.file?(d8)
                    @logger.info( "DEX builder: found d8")

                    raise "I'm expecting to have output dex named classes.dex for d8 tool but found #{File.basename(outdex)}" if (File.basename(outdex) != 'classes.dex')
                    
                    args = []
                    args << '--output'
                    args << "#{File.dirname(outdex)}"
                    #args << '--no-desugaring'
                    args.concat jarlist
                    args << '--min-api' 
                    args << $app_config["android"]["minSDK"]
                    
                    return d8, args
                else
                    @logger.error( "Can't find proper DEX builder in the build tools")
                end

                return nil,nil
            end
        end
    end
end
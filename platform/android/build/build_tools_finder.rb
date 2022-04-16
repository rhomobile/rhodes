module AndroidTools
    def findBuildToolsPath( version, sdk_path, platform)
        if File.exist?(File.join(sdk_path, "build-tools"))

            build_tools_ver = nil
            if version
                #will try to find user-specified build tools
                path = File.join(sdk_path,'build-tools',version)
                if File.directory?(path)
                    build_tools_ver = version            
                end
            end
    
    
            build_tools = {}

            if !build_tools_ver
                Dir.foreach(File.join(sdk_path, "build-tools")) do |entry|
                    next if entry == '.' or entry == '..'

                    #Lets read source.properties file to get highest available build-tools
                    src_prop_path = File.join(sdk_path, "build-tools",entry,"source.properties")
                    next unless File.file?(src_prop_path)

                    File.open(src_prop_path) do |f|
                    f.each_line do |line|
                        build_tools[entry] = line.split('=')[1].gsub("\n",'') if line.match(/^Pkg.Revision=/)
                    end
                    end
                end

                latest_build_tools = build_tools.sort_by{|folder_name,sdk_version| sdk_version}.last
                build_tools_ver = latest_build_tools[0]
            end
        end

        if build_tools_ver
            build_tools_path = File.join(sdk_path,'build-tools',build_tools_ver)
        else
            build_tools_path = File.join(sdk_path, "platforms", platform, "tools")
        end

        build_tools_path
    end
    module_function :findBuildToolsPath
end
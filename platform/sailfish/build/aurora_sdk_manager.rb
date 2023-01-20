module Aurora    

    class SDKManager
        
        def initialize( root, target )
            buildManagerImpl root, target
        end

        def current_build_sdk_dir
            managerImpl.current_build_sdk_dir
        end

        private

        def buildManagerImpl root, target

            v = detectSDKVerFromRoot root

            if ( v < 4 )
                @managerImpl = LegacyManagerImpl.new root, target
            else
                @managerImpl = ManagerV4.new root, target
            end
            
        end

        def detectSDKVerFromRoot root
            4
        end

        attr_accessor :managerImpl
    end

    private

    class LegacyManagerImpl

        attr_accessor   :sysName, :current_build_sdk_dir

        def initialize root, target
            aurora = false

            if isWindows?
                if Dir.exist?(File.join(root, "settings", "AuroraOS-SDK"))
                  aurora = true
                  @current_build_sdk_dir = File.join(root, "settings", "AuroraOS-SDK", "libsfdk", "build-target-tools", "Aurora OS Build Engine", target)
                elsif Dir.exist?(File.join(root, "settings", "SailfishSDK"))
                  aurora = false
                  @current_build_sdk_dir = File.join(root, "settings", "SailfishSDK", "libsfdk", "build-target-tools", "Sailfish OS Build Engine", target)
                elsif Dir.exist?(File.join(root, "settings", "SailfishOS-SDK"))
                  aurora = false
                  @current_build_sdk_dir = File.join(root, "settings", "SailfishOS-SDK", "mer-sdk-tools", "Sailfish OS Build Engine", target)
                else
                  raise "Can't recognize build SDK!"                  
                end
                @current_build_sdk_dir = $current_build_sdk_dir.gsub("\\", "/")
              else
                if Dir.exist?(File.join(File.expand_path('~'), ".config", "AuroraOS-SDK"))
                  aurora = true
                  @current_build_sdk_dir = File.join(File.expand_path('~'), ".config", "AuroraOS-SDK", "libsfdk", "build-target-tools", "Aurora OS Build Engine", target)
                elsif Dir.exist?(File.join(File.expand_path('~'), ".config", "SailfishSDK"))
                  aurora = false
                  @current_build_sdk_dir = File.join(File.expand_path('~'), ".config", "SailfishSDK", "libsfdk", "build-target-tools", "Sailfish OS Build Engine", target)
                elsif Dir.exist?(File.join(File.expand_path('~'), ".config", "SailfishOS-SDK"))
                  aurora = false
                  @current_build_sdk_dir = File.join(File.expand_path('~'), ".config", "SailfishOS-SDK", "mer-sdk-tools", "Sailfish OS Build Engine", target)
                else
                  raise "Can't recognize build SDK!"                  
                end
            end
          
            if aurora
                @sysName = "Aurora OS"
            else
                @sysName = "Sailfish OS"
            end
        end

        private

        def isWindows?
            if /cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM
                return true
            else
                return false
            end
        end
    end

    class ManagerV4

        attr_accessor   :sysName, :current_build_sdk_dir
        
        def initialize root, target
        end
        
    end

end
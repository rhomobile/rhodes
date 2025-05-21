
require 'fileutils'

class IPhoneBuild
  class << self

    def load_env_for_build_rhodes_ios_extension
        $targetdir = ENV['TARGET_TEMP_DIR']
        raise "TARGET_TEMP_DIR is not set" if $targetdir.nil?

        #$tempdir = ENV['TEMP_FILES_DIR']
        #raise "TEMP_FILES_DIR is not set" if $tempdir.nil?

        $rootdir = ENV['RHO_ROOT']
        raise "RHO_ROOT is not set" if $rootdir.nil?

        $app_path = ENV["RHO_APP_DIR"]
        raise "RHO_APP_DIR is not set" if $app_path.nil?

        $xcodebuild = ENV['XCODEBUILD']
        raise "XCODEBUILD is not set" if $xcodebuild.nil?

        $configuration = ENV['CONFIGURATION']
        raise "CONFIGURATION is not set" if $configuration.nil?

        $sdk = ENV['SDK_NAME']
        raise "SDK_NAME is not set" if $sdk.nil?

        #$bindir = ENV['PLATFORM_DEVELOPER_BIN_DIR']
        #raise "PLATFORM_DEVELOPER_BIN_DIR is not set" if $bindir.nil?

        #$sdkroot = ENV['SDKROOT']
        #raise "SDKROOT is not set" if $sdkroot.nil?

        $arch = ENV['ARCHS']
        raise "ARCHS is not set" if $arch.nil?

        $other_cflags = ENV['RHO_OTHER_CFLAGS']
        #, 'OTHER_CFLAGS="-fembed-bitcode"'

        #$gccbin = $bindir + '/gcc-4.2'
        #$arbin = $bindir + '/ar'
    end

    def build_rhodes_ios_extension(ext_name, ext_lib_name, xcode_project_target_name, additional_options = {})

        puts "Build \""+ext_name+"\" extension START"

        iphone_path = '.'

        simulator = $sdk =~ /iphonesimulator/

        if $configuration == 'Distribution'
           $configuration = 'Release'
        end

        result_path = iphone_path + '/build/' + $configuration + '-' + ( simulator ? "iphonesimulator" : "iphoneos")
        result_lib = result_path + '/lib'+xcode_project_target_name+'.a'
        target_lib = $targetdir + '/lib'+ext_lib_name+'.a'

        currentdir = Dir.pwd()
        build_path = File.join(currentdir, '/build/' + $configuration + '-' + ( simulator ? "iphonesimulator" : "iphoneos"))


        FileUtils.rm_rf 'build'
        FileUtils.rm_rf target_lib

        additional_string = nil
        options = {}

        project_workspace_path = File.join(currentdir, ext_name + ".xcworkspace")
        pods_path = File.join(currentdir, "Pods")


        podfile_path = File.join(currentdir, "Podfile")

        if (File.exist?(podfile_path)) && ( (!File.exist?(project_workspace_path)) || (!File.exist?(pods_path)))
            #we shoudl setup project for cocoapods
            puts "We found Podfile, but workspace or Pods are not exists - we should run 'pod install' to generte workspace and Pods"

            require File.join(ENV['RHO_ROOT'], 'lib','build','jake')

            Jake.run('pod', ['install'])
        end


        if File.exist?(project_workspace_path)
            args = ['build', '-workspace', ext_name + ".xcworkspace", '-scheme', xcode_project_target_name, '-configuration', $configuration, '-sdk', $sdk]
            additional_string = " BUILT_PRODUCTS_DIR="+ build_path+" CONFIGURATION_BUILD_DIR="+build_path
            options = {:string_for_add_to_command_line => additional_string}
        else
            args = ['build', '-target', xcode_project_target_name, '-configuration', $configuration, '-sdk', $sdk]
            #args = ['build', '-scheme', xcode_project_target_name, '-configuration', $configuration, '-sdk', $sdk]
        end

        args << ('OTHER_CFLAGS="'+$other_cflags.to_s+'"') if !$other_cflags.nil?

        require File.join(ENV['RHO_ROOT'], 'platform','iphone','rbuild','iphonecommon')


        ret = IPhoneBuild.run_and_trace($xcodebuild,args, options)

        # copy result to $targetdir
        FileUtils.cp result_lib,target_lib
        FileUtils.cp result_lib,File.join($app_path, "project", "iphone", 'ExtLibs', 'lib'+ext_lib_name+'.a')

        puts "Build \""+ext_name+"\" extension : result library filepath ["+target_lib+"]"

        puts "Build \""+ext_name+"\" extension FINISH"

        return ret
    end


  end

end

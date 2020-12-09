
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

        result_lib = iphone_path + '/build/' + $configuration + '-' + ( simulator ? "iphonesimulator" : "iphoneos") + '/lib'+xcode_project_target_name+'.a'
        target_lib = $targetdir + '/lib'+ext_lib_name+'.a'

        FileUtils.rm_rf 'build'
        FileUtils.rm_rf target_lib

        args = ['build', '-target', xcode_project_target_name, '-configuration', $configuration, '-sdk', $sdk]
        args << ('OTHER_CFLAGS="'+$other_cflags.to_s+'"') if !$other_cflags.nil?

        require File.join(ENV['RHO_ROOT'], 'platform','iphone','rbuild','iphonecommon')

        ret = IPhoneBuild.run_and_trace($xcodebuild,args)

        # copy result to $targetdir
        FileUtils.cp result_lib,target_lib

        puts "Build \""+ext_name+"\" extension : result library filepath ["+target_lib+"]"

        puts "Build \""+ext_name+"\" extension FINISH"

        return ret
    end


  end

end

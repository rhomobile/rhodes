require_relative 'android_tools'
require 'fileutils'

class ApkBuilder
    attr_accessor :sdk_path, :output_path, :res_pkg, :dex_path, :debug, :build_tools_path
    attr_accessor :keystore, :storealias, :storepass, :keypass    

    def build
        validate

        tmpdir = Dir.mktmpdir 'rhodes_apk_build'

        begin

            unaligned_apk = File.join tmpdir, 'unaligned.apk'
            AndroidTools.apk_build @sdk_path, unaligned_apk, @res_pkg, @dex_path, @debug
            
            unsigned_apk = File.join tmpdir, 'unsigned.apk'
            AndroidTools.alignApk unaligned_apk, unsigned_apk
            
            if @debug
                AndroidTools.signApkDebug unsigned_apk, @output_path
            else
                AndroidTools.signApk unsigned_apk, @output_path, @keystore, @keypass, @storepass, @storealias
            end

        ensure

            FileUtils.remove_entry(tmpdir)
        end

    end

    private
    def validate
        raise 'APK Builder set up incorrectly' unless defined? @sdk_path and
            defined? @output_path and
            defined? @res_pkg and
            defined? @dex_path and
            defined? @debug
    end
end
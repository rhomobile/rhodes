#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------

namespace "config" do

  task :set_osx_platform do
    $current_platform = "osx" unless $current_platform
  end

  task :osx => [:set_osx_platform, "config:common"] do
    $make = "make"
    $name_tool = "install_name_tool"
    $move = "mv"
    $remove = "rm"
    $qt_project_dir = File.join( $startdir, 'platform/shared/qt/' )
    $build_dir = File.join( $startdir, 'platform/osx/bin/' )

    $devroot = '/Applications/Xcode.app/Contents/Developer'
    $devbin = $devroot + '/usr/bin'
    $xcodebuild = $devbin + '/xcodebuild'
    if !File.exist? $xcodebuild
      $devroot = '/Developer'
      $devbin = '/usr/bin'
      $xcodebuild = $devbin + '/xcodebuild'
    end
  end
end

namespace "build" do
  namespace "osx" do

    task :extensions do
        ENV['RHO_PLATFORM'] = 'osx'
        ENV["PLATFORM_DEVELOPER_BIN_DIR"] = $devbin
        ENV['PWD'] = $startdir
        ENV['RHO_ROOT'] = ENV['PWD']
        ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", "osx", "bin", "extensions")
        ENV["BUILD_DIR"] = File.join(ENV['PWD'], "platform", "osx", "bin", "RhoSimulator", "tmp")
        ENV["ARCHS"] = "x86_64 -g -gdwarf-2 -Xarch_x86_64 -mmacosx-version-min=10.5 -DRHODES_EMULATOR"
        ENV["RHO_ROOT"] = $startdir
        ENV["RHO_QMAKE"] = $qmake
        ENV['RHO_QMAKE_SPEC'] = 'macx-clang'
        ENV['RHO_QMAKE_VARS'] = $rhosimulator_build ? 'RHOSIMULATOR_BUILD=1' : ''

        ENV["XCODEBUILD"] = $xcodebuild
        ENV["CONFIGURATION"] ||= $configuration
        ENV["SDK_NAME"] ||= $sdk

        $app_config["extensions"].each do |ext|
            $app_config["extpaths"].each do |p|
                extpath = File.join(p, ext, 'ext')
                next unless File.exist? File.join(extpath, "build")
                ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform", "osx", "bin", "extensions", ext)
                if ext != 'openssl.so'
                    $extensions_lib << " -l#{ext}"
                    $pre_targetdeps << " ../../../osx/bin/extensions/lib#{ext}.a"
                end
                Jake.run3('./build', extpath, {}, true)
            end
        end
    end

    task :rhosimulator => ["config:rhosimulator", "config:set_osx_platform", "config:osx", "config:qt", "build:rhosimulator_version"] do
        $config["platform"] = $current_platform
        chdir $startdir
        $extensions_lib = ''
        $pre_targetdeps = ''
        init_extensions(pwd, nil)
        Rake::Task["build:osx:extensions"].invoke

        ext_dir = File.join($startdir, 'platform/osx/bin/extensions')
        mkdir_p ext_dir if not File.exist? ext_dir
        File.open(File.join(ext_dir, 'extensions.pri'), "wb") do |fextensions|
            fextensions.write(%{SOURCES += ../../ruby/ext/rho/extensions.c
LIBS += -L../../../osx/bin/extensions#{$extensions_lib}
PRE_TARGETDEPS += #{$pre_targetdeps}
})
        end

        app_path = File.join( $build_dir, 'RhoSimulator/RhoSimulator.app' )
        if File.exist? app_path
          Jake.run3("#{$remove} -Rf #{app_path}", nil, {}, true)
        end

        File.open(File.join($startdir, 'platform/shared/qt/rhodes/resources/Info.plist'), "wb") do |fversion|
            fversion.write( %{<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist SYSTEM "file://localhost/System/Library/DTDs/PropertyList.dtd">
<plist version="0.9">
<dict>
	<key>CFBundleIconFile</key>
	<string>@ICON@</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleGetInfoString</key>
	<string>RhoSimulator #{$rhodes_version}, Copyright 2017 Tau Technologies.</string>
	<key>CFBundleSignature</key>
	<string>@TYPEINFO@</string>
	<key>CFBundleExecutable</key>
	<string>@EXECUTABLE@</string>
	<key>CFBundleIdentifier</key>
	<string>com.rhomobile.@EXECUTABLE@</string>
</dict>
</plist>
})
        end

        qmake = "#{$qmake} -o Makefile -r -spec macx-clang RhoSimulator.pro RHOSIMULATOR_BUILD=1  -early QMAKE_DEFAULT_LIBDIRS=$(xcrun -show-sdk-path)/usr/lib"
        Jake.run3(qmake                        , $qt_project_dir, {}, false)
        #Jake.run3("#{$make} clean"             , $qt_project_dir, {}, false)
        Jake.run3("#{$make} all"               , $qt_project_dir, {}, false)
        Jake.run3("#{$macdeployqt} #{app_path}", $qt_project_dir, {}, false)
        #Jake.run3("#{$make} clean"             , $qt_project_dir, {}, true)
    end
  end
end

namespace "clean" do
  task :osx => ["config:rhosimulator", "config:set_osx_platform", "config:osx"] do
    rm_rf File.join($startdir, 'platform/osx/bin')
  end
  namespace "osx" do
      task :rhosimulator do
        rhoSimDir = File.join( $build_dir, 'RhoSimulator/RhoSimulator.app' )
        FileUtils.rm_rf("#{rhoSimDir}/.", secure: true)
      end
    end
end

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
    $devroot = '/Developer' if $devroot.nil?
    $xcodebuild = $devroot + "/usr/bin/xcodebuild"
    $sdkroot = "/"
  end
end

namespace "build" do
  namespace "osx" do

    task :extensions => "config:osx" do
        ENV['RHO_PLATFORM'] = 'osx'
        ENV["PLATFORM_DEVELOPER_BIN_DIR"] = "/usr/bin"
        ENV["SDKROOT"] = $sdkroot
        ENV['PWD'] = $startdir
        ENV['RHO_ROOT'] = ENV['PWD']
        ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", "osx", "bin", "extensions")
        ENV["BUILD_DIR"] = File.join(ENV['PWD'], "platform", "osx", "bin", "RhoSimulator", "tmp")
        ENV["ARCHS"] = "x86_64 -g -gdwarf-2 -Xarch_x86_64 -mmacosx-version-min=10.5 -DRHODES_EMULATOR"
        ENV["RHO_ROOT"] = $startdir

        ENV["XCODEBUILD"] = $xcodebuild
        ENV["CONFIGURATION"] ||= $configuration
        ENV["SDK_NAME"] ||= $sdk

        $app_config["extensions"].each do |ext|
            $app_config["extpaths"].each do |p|
                extpath = File.join(p, ext, 'ext')
                next unless File.exists? File.join(extpath, "build")
                ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform", "osx", "bin", "extensions", ext)
                $extensions_lib << " -l#{ext}"
                $pre_targetdeps << " ../../../osx/bin/extensions/lib#{ext}.a"
                puts Jake.run('./build', [], extpath)
                exit 1 unless $? == 0
            end
        end
    end

    task :rhosimulator => ["config:set_osx_platform", "config:osx", "config:qt"] do
        $rhosimulator_build = true
        $config["platform"] = $current_platform
        chdir $startdir
        $extensions_lib = ''
        $pre_targetdeps = ''
        init_extensions(pwd, nil)
        Rake::Task["build:osx:extensions"].invoke

        if $extensions_lib.length > 0
            File.open(File.join($startdir, 'platform/osx/bin/extensions/extensions.pri'), "wb") do |fextensions|
                fextensions.write(%{SOURCES += ../../ruby/ext/rho/extensions.c
LIBS += -L../../../osx/bin/extensions#{$extensions_lib}
PRE_TARGETDEPS += #{$pre_targetdeps}
})
            end
        else
            Jake.run($remove,['-Rf', File.join($startdir, 'platform/osx/bin/extensions')])
        end

        app_path = File.join( $build_dir, 'RhoSimulator/RhoSimulator.app' )
        puts Jake.run($remove,['-Rf', app_path ])

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
	<string>RhoSimulator #{$rhodes_version}, Copyright 2010-2012 Rhomobile, Inc.</string>
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

        chdir $qt_project_dir
        args = ['-o', 'Makefile', '-r', '-spec', 'macx-g++', 'RhoSimulator.pro']
        puts Jake.run($qmake,args)
        puts Jake.run($make, ['clean'])
        puts Jake.run($make, ['all'])

        unless $? == 0
          puts "Error building"
          exit 1
        end

        puts Jake.run($macdeployqt, [app_path])

        # Last version of macdeployqt correctly handles these paths, so in the future this code can be removed:
        exe_path = File.join( app_path, 'Contents/MacOS/RhoSimulator' )
        frm_path = File.join( app_path, 'Contents/Frameworks/' )
        fw_path = ['@executable_path/../Frameworks/', '.framework/Versions/Current', '.framework/Versions/4']
        libs = ['QtCore', 'QtGui', 'QtNetwork', 'QtWebKit', 'QtDBus', 'QtXml', 'phonon']
        libs.each {|lib|
          args = [ frm_path + lib + fw_path[1], frm_path + lib + fw_path[2] ]
          puts Jake.run($move,args)
          args = [ '-change', fw_path[0] + lib + fw_path[1] + '/' + lib, fw_path[0] + lib + fw_path[2] + '/' + lib, exe_path]
          puts Jake.run($name_tool,args)
        }
        # end of the depricated macdeployqt fix code

        puts Jake.run($remove,['-Rf', File.join(frm_path, 'QtDeclarative.framework' )])
        puts Jake.run($remove,['-Rf', File.join(frm_path, 'QtOpenGL.framework' )])
        puts Jake.run($remove,['-Rf', File.join(frm_path, 'QtScript.framework' )])
        puts Jake.run($remove,['-Rf', File.join(frm_path, 'QtSql.framework' )])
        puts Jake.run($remove,['-Rf', File.join(frm_path, 'QtSvg.framework' )])
        puts Jake.run($remove,['-Rf', File.join(frm_path, 'QtXmlPatterns.framework' )])

        chdir $qt_project_dir
        puts Jake.run($make, ['clean'])
    end
  end
end

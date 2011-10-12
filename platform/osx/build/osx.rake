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
  end
end

namespace "build" do
  namespace "osx" do
    task :rhosimulator => ["config:set_osx_platform", "config:osx", "config:qt"] do
        app_path = File.join( $build_dir, 'RhoSimulator/RhoSimulator.app' )
        puts Jake.run($remove,['-R', app_path ])

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
	<string>RhoSimulator #{$rhodes_version}, Copyright © 2010-2011 Rhomobile, Inc.</string>
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

        puts Jake.run($remove,['-R', File.join(frm_path, 'QtDeclarative.framework' )])
        puts Jake.run($remove,['-R', File.join(frm_path, 'QtOpenGL.framework' )])
        puts Jake.run($remove,['-R', File.join(frm_path, 'QtScript.framework' )])
        puts Jake.run($remove,['-R', File.join(frm_path, 'QtSql.framework' )])
        puts Jake.run($remove,['-R', File.join(frm_path, 'QtSvg.framework' )])
        puts Jake.run($remove,['-R', File.join(frm_path, 'QtXmlPatterns.framework' )])

        chdir $qt_project_dir
        puts Jake.run($make, ['clean'])
    end
  end
end

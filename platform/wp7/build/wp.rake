namespace "config" do
	task :set_wp_platform do
		$current_platform = "wm" unless $current_platform
	end

	task :wp => [:set_wp_platform, "config:common"] do
		$rubypath = "res/build-tools/RhoRuby.exe"
		$zippath = "res/build-tools/7za.exe"
		$wp7runner = "res/build-tools/RhoAppRunner.exe"
		$wp7explorer = "res/build-tools/wp7explorer.exe"
		$wp7logserver = "res/build-tools/RhoLogserver.exe"
		$genpath = "ClassInitGenerator.exe"
		$builddir = $config["build"]["wppath"] + "/build"
		$vcbindir = $config["build"]["wppath"] + "/bin"
		$appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"] 
		$bindir = $app_path + "/bin"
		$rhobundledir =  $app_path + "/RhoBundle"
		$srcdir =  $bindir + "/RhoBundle"
		$targetdir = $bindir + "/target/wp7"
		$tmpdir =  $bindir +"/tmp"
		$msbuild = $config["env"]["paths"]["msbuild"]
		$msbuild = "msbuild" if $msbuild.nil?
		#$cabwiz = File.join($config["env"]["paths"]["cabwiz"], "cabwiz.exe") if $config["env"]["paths"]["cabwiz"]
		#$cabwiz = "cabwiz" if $cabwiz.nil?
		$sdk = "Windows Phone 7 SDK"
		$sdk = $app_config["wpsdk"] unless $app_config["wpsdk"].nil?

		$excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']
	end
end
 
 namespace "build" do
	namespace "wp" do
		task :extensions => "config:wp" do
			$app_config["extensions"].each do |ext|
				$app_config["extpaths"].each do |p|
					extpath = File.join(p, ext, 'ext')
					next unless File.exists? File.join(extpath, "build.bat")

					ENV['RHO_PLATFORM'] = 'wp'
					ENV['PWD'] = $startdir
					ENV['RHO_ROOT'] = ENV['PWD']
					ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", "wp7", "bin", $sdk, "rhodes", "Release")
					ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform", "wp7", "bin", $sdk, "extensions", ext)
					ENV['MSBUILD'] = $msbuild
					ENV['SDK'] = $sdk

					puts Jake.run("build.bat", [], extpath)
					break
				end
			end
		end
	
		desc "Build WP7 rhobundle"
		#task :rhobundle => ["config:wp", :extensions] do
		task :rhobundle => ["config:wp"] do
			Rake::Task["build:bundle:noiseq"].execute

			#move public folder to root
			cp_r $srcdir + "/apps/public", $srcdir + "/public"
			rm_r $srcdir + "/apps/public"
		end
		
		task :rhobundlemap => ["config:wp"] do
			chdir $srcdir
			file = File.open("RhoBundleMap.txt", "w+")
			chdir $srcdir
			stamp = 0
			Dir.glob(File.join("**", '*.*')).each do |f|
				if f.start_with?('db') || f.end_with?('.rb') || f.end_with?('.erb') || f == "apps/app_manifest.txt" || f == "apps/rhoconfig.txt" ||
					f == "apps/rhoconfig.txt.timestamp" || f == "RhoBundleMap.txt"
					next;
				end
				    #if stamp < File.mtime(f).to_i
					#	stamp = File.mtime(f).to_i
					#end
				file.puts f + "|" + File.mtime(f).to_i.to_s
			end
			file.close
			#file = File.open("timestamp.txt", "w+")
			#file.puts stamp
			#file.close
		end 

		task :rubyext => ["config:wp"] do
			#chdir $startdir + "/res/build-tools"
			chdir "../ironruby/bin/Debug"

			args = []
			args << $startdir + "/" + $vcbindir + "/RhoRubyExtGen/Debug/RhoRubyExtGen.dll"
			args << "/libraries:rho.rubyext"
			args << "/out: " + $startdir + "/"+ $config["build"]["wppath"] +"/RhoRubyLib/" + "Initializers.Generated.cs"
			puts Jake.run($genpath, args)
		end
		
		task :ironruby => ["config:wp"] do   	
		    iron_path = ""
			if $config["env"]["paths"]["ironruby"].nil?
				iron_path = "../ironruby"
			else
				iron_path = $config["env"]["paths"]["ironruby"]
			end
			cp_r File.join( $config["build"]["wppath"], "IronRuby/Languages"), iron_path
			
			chdir File.join( iron_path, "Solutions" )

			args = ['Ruby.sln', '/property:Configuration=Silverlight3Release']
			
			#if (!File.exists? "../bin/Silverlight3Release/Microsoft.Dynamic.dll") &&
			#   (!File.exists? "../bin/Silverlight3Release/Microsoft.Scripting.dll") &&
			#   (!File.exists? "../bin/Silverlight3Release/Microsoft.Scripting.Core.dll") && 
			#   (!File.exists? "../bin/Silverlight3Release/IronRuby.Libraries.dll") && 
			#   (!File.exists? "../bin/Silverlight3Release/IronRuby.dll" )
			#	puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
				Jake.run($msbuild,args)
				unless $? == 0
					puts "Error building"
					exit 1
				end
			#end
 
			chdir $startdir

            iron_release = File.join( $config["build"]["wppath"], "IronRuby/bin/Silverlight3Release")
            rm_rf iron_release
            mkdir_p iron_release
			cp File.join( iron_path, "bin/Silverlight3Release/IronRuby.dll" ), iron_release
			cp File.join( iron_path, "bin/Silverlight3Release/IronRuby.Libraries.dll" ), iron_release
			cp File.join( iron_path, "bin/Silverlight3Release/Microsoft.Dynamic.dll" ), iron_release
			cp File.join( iron_path, "bin/Silverlight3Release/Microsoft.Scripting.dll" ), iron_release
			cp File.join( iron_path, "bin/Silverlight3Release/Microsoft.Scripting.Core.dll" ), iron_release
		end 

		task :rhodes => ["config:wp", "build:wp:rhobundle"] do
		    out_dir = $startdir + "/"+ $config["build"]["wppath"] +"/rhodes"
			cp $app_path + "/icon/icon.png", out_dir if File.exists? $app_path + "/icon/icon.ico"     
		
			chdir $config["build"]["wppath"]

			doc = REXML::Document.new(File.open($startdir+"/"+$config["build"]["wppath"]+"/rhodes/Properties/WMAppManifest.xml"))
			doc.elements.each("Deployment/App") { 
			|element| element.attributes["ProductID"] =  "{"+$app_config["wp"]["productid"]+"}"
			          element.attributes["Title"] =  $app_config["name"]
			}
			File.open($startdir+"/"+$config["build"]["wppath"]+"/rhodes/Properties/WMAppManifest.xml", "w") { |f| doc.write f; f.close }

			#args = ['/M4', 'rhodes.sln', "\"Release|#{$sdk}\""]
			args = ['rhodes.sln', '/property:Configuration=Release']

			puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
			Jake.run($msbuild,args)
			unless $? == 0
				puts "Error building"
				exit 1
			end
 
			chdir $startdir
		end 

		task :devrhobundleRelease => [:rhobundle, :rhobundlemap, "device:wp:addbundletoxapRelease"] do
			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Release/"
			doc = REXML::Document.new(File.open(out_dir + "XapCacheFile.xml"))
			chdir $srcdir
			Dir.glob(File.join("**", '*.*')).each do |f|
				doc.root[1,0] = REXML::Element.new "file lastWriteTime='" + File.mtime(f).strftime("%m/%d/%Y %I:%M:%S %p") + "' source='" + $srcdir.gsub("/", "\\") + "\\" + f.gsub("/", "\\") + "' archivePath='" + f.gsub("/", "\\") + "'" 
			end
			File.open(out_dir + "XapCacheFile.xml", "w") { |f| doc.write f, 2; f.close }
			
			chdir $startdir

			mkdir_p $config["build"]["wppath"] + "/rhodes/obj/Release" if not File.exists? $config["build"]["wppath"] + "/rhodes/obj/Release"
			cp out_dir + "XapCacheFile.xml", $config["build"]["wppath"] + "/rhodes/obj/Release"
		end

		task :devrhobundleDebug => [:rhobundle, :rhobundlemap, "device:wp:addbundletoxapDebug"] do
			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Debug/"
			doc = REXML::Document.new(File.open(out_dir + "XapCacheFile.xml"))
			chdir $srcdir
			Dir.glob(File.join("**", '*.*')).each do |f|
				doc.root[1,0] = REXML::Element.new "file lastWriteTime='" + File.mtime(f).strftime("%m/%d/%Y %I:%M:%S %p") + "' source='" + $srcdir.gsub("/", "\\") + "\\" + f.gsub("/", "\\") + "' archivePath='" + f.gsub("/", "\\") + "'" 
			end
			File.open(out_dir + "XapCacheFile.xml", "w") { |f| doc.write f, 2; f.close }
			
			chdir $startdir

			mkdir_p $config["build"]["wppath"] + "/rhodes/obj/Debug" if not File.exists? $config["build"]["wppath"] + "/rhodes/obj/Debug"
			cp out_dir + "XapCacheFile.xml", $config["build"]["wppath"] + "/rhodes/obj/Debug"
		end
	end
end

def get_app_log()
	args = []
	args << $app_config["wp"]["productid"]
	args << ""
	args << ""
	args << ""
	args << "RhoLog.txt"
	cc_run($wp7runner, args) or return false
	puts "RhoLog.txt stored to " + $app_path
	return true
end
 
 namespace "device" do
	namespace "wp" do
		task :addbundletoxapDebug do
			#mkdir_p $bindir + "/rho" if not File.exists? $bindir + "/rho"
			#cp_r $srcdir + "/apps", $bindir + "/rho"
			#cp_r $srcdir + "/db", $bindir + "/rho"
			#cp_r $srcdir + "/lib", $bindir + "/rho"
			#cp_r $srcdir + "/RhoBundleMap.txt", $bindir + "/rho"
			#cp_r $srcdir + "/timestamp.txt", $bindir + "/rho"

			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Debug/"
			
			chdir $startdir
			args = []
			args << "a"
			args << "-tzip"
			args << out_dir + "rhodes.xap"
			args << $srcdir + "/*"
			puts Jake.run($zippath, args)

			#rm_r $bindir + "/rho"
		end

		task :addbundletoxapRelease do
			#mkdir_p $bindir + "/rho" if not File.exists? $bindir + "/rho"
			#cp_r $srcdir + "/apps", $bindir + "/rho"
			#cp_r $srcdir + "/db", $bindir + "/rho"
			#cp_r $srcdir + "/lib", $bindir + "/rho"
			#cp_r $srcdir + "/RhoBundleMap.txt", $bindir + "/rho"
			#cp_r $srcdir + "/timestamp.txt", $bindir + "/rho"

			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Release/"
			
			chdir $startdir
			args = []
			args << "a"
			args << "-tzip"
			args << out_dir + "rhodes.xap"
			args << $srcdir + "/*"
			puts Jake.run($zippath, args)

			#rm_r $bindir + "/rho"
		end

		desc "Build production for device or emulator"
		task :production => ["config:wp","build:wp:rhobundle","build:wp:rhodes"] do
			#out_dir = $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/Release/"
			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Release/"
			cp  out_dir + "rhodes.xap", out_dir + $appname + ".xap"

			mkdir_p $bindir if not File.exists? $bindir
			mkdir_p $targetdir if not File.exists? $targetdir
			mv out_dir + $appname + ".xap", $targetdir
		end

		task :getlog => "config:wp" do
			#get_app_log() or exit 1
		end
	end
end

namespace "clean" do
  desc "Clean wp"
  task :wp => "clean:wm:all"
  namespace "wm" do
    task :rhodes => ["config:wp"] do
      rm_rf $vcbindir
      rm_rf $targetdir
    end
    task :all => "clean:wm:rhodes"
  end
end

namespace "run" do
		desc "Build, install .xap and run on WP7 emulator"
		task :wp => ["clean:wm:all", "device:wp:production"] do
		if $app_config["wp"]["productid"] != nil
			system("START " + $wp7logserver + " " + $app_path + "/rholog.txt")
			args = []
			args << $app_config["wp"]["productid"]
			args << $app_config["name"]
			args << $app_path + "/icon/icon.png"
			args << $targetdir + "/" + $appname + ".xap"
			args << "emu"
			puts Jake.run($wp7runner, args)
		else
			puts "productid must be set in build.yml"
			puts "productid's format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
		end
		end

		namespace "wp" do
			desc "Start Isolated Storage Explorer"
			task :explorer  => ["config:wp"] do
				args = []
				puts Jake.run($wp7explorer, args)
			end

			desc "Build, install .xap and run on WP7 device"
			task :device => ["clean:wm:all", "device:wp:production"] do
			if $app_config["wp"]["productid"] != nil
			    system("START " + $wp7logserver + " " + $app_path + "/rholog.txt")
				args = []
				args << $app_config["wp"]["productid"]
				args << $app_config["name"]
				args << $app_path + "/icon/icon.png"
				args << $targetdir + "/" + $appname + ".xap"
				args << "dev"
				puts Jake.run($wp7runner, args)
			else
				puts "productid must be set in build.yml"
				puts "productid's format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
			end
			end
		end
end
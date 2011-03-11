 namespace "config" do
	task :wp => ["config:common"] do
		$rubypath = "res/build-tools/RhoRuby.exe"
		$zippath = "res/build-tools/7za.exe"
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
		end
		
		task :rhodes => ["config:wp", "build:wp:rhobundle"] do
		    out_dir = $startdir + "/"+ $config["build"]["wppath"] +"/rhodes"
			cp $app_path + "/icon/icon.png", out_dir if File.exists? $app_path + "/icon/icon.ico"     
		
			chdir $config["build"]["wppath"]

			#args = ['/M4', 'rhodes.sln', "\"Release|#{$sdk}\""]
			args = ['rhodes.sln']
			puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
			puts Jake.run($msbuild,args)
			unless $? == 0
				puts "Error building"
				exit 1
			end
 
			chdir $startdir
		end 

		task :devrhobundle => [:rhobundle, "device:wp:addbundletoxap"] do
			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Debug/"
			mkdir_p $config["build"]["wppath"] + "/rhodes/obj/debug" if not File.exists? $config["build"]["wppath"] + "/rhodes/obj/debug"
			cp out_dir + "XapCacheFile.xml", $config["build"]["wppath"] + "/rhodes/obj/debug"
		end
	end
end
 
 namespace "device" do
	namespace "wp" do
		task :addbundletoxap do
			mkdir_p $bindir + "/rho" if not File.exists? $bindir + "/rho"
			cp_r $srcdir + "/apps", $bindir + "/rho"
			cp_r $srcdir + "/db", $bindir + "/rho"
			cp_r $srcdir + "/lib", $bindir + "/rho"

			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Debug/"
			
			chdir $startdir
			args = []
			args << "a"
			args << "-tzip"
			args << out_dir + "rhodes.xap"
			args << $bindir + "/rho/"
			puts Jake.run($zippath, args)

			rm_r $bindir + "/rho"
		end

		desc "Build production for device or emulator"
		task :production => ["config:wp","build:wp:rhobundle","build:wp:rhodes"] do
			#out_dir = $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/Release/"
			out_dir = $startdir + "/" + $vcbindir + "/rhodes/Debug/"
			cp  out_dir + "rhodes.xap", out_dir + $appname + ".xap"

			mkdir_p $bindir if not File.exists? $bindir
			mkdir_p $targetdir if not File.exists? $targetdir
			mv out_dir + $appname + ".xap", $targetdir
		end
	end
end

namespace "run" do
	namespace "wp" 
		desc "Build, install .xap and run on WP7 emulator"
		task :cab => ["device:wp:production"]
	end
end
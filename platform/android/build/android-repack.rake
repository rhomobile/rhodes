load File.join( File.dirname(__FILE__), 'android.rake' )

namespace "build" do
  namespace "android" do
    task :repack do
      puts "BUILD:ANDROID:REPACK task"

#      Rake::Task['build:android:all'].execute
      Rake::Task['config:android'].execute
      Rake::Task['build:android:rhobundle'].execute
      Rake::Task['package:android'].execute
      Rake::Task['device:android:production'].execute

    end
  end
end
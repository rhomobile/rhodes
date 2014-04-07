load File.join( File.dirname(__FILE__), 'android.rake' )

require 'zip'

namespace 'build' do
  namespace 'android' do

    def get_bundle_map( stream )
      bundle_map = {}

      stream.each { |line|
        vals = line.split('|')

        fpath = vals[0]
        type = vals[1]
        len = vals[2].to_i
        timestamp = vals[3].strip!.to_i

        bundle_map[fpath] = { :type => type, :len => len, :timestamp => timestamp }
      }
      return bundle_map
    end

    task :repack do
      puts 'Repacking final APK with updated bundle'

      Rake::Task['config:android'].execute
      Rake::Task['build:android:rhobundle'].execute

      resourcepkg = $bindir + "/rhodes.ap_"

      has_changes = false

      if File.exists?(resourcepkg) then
        puts 'Make diff maps and repack only changed files'

        begin
          zipfile = Zip::File.open(resourcepkg)

          packed_items = {}

          zipfile.get_input_stream('assets/RhoBundleMap.txt') do |filelist|
            packed_items = get_bundle_map(filelist)
          end

          zipfile.close

          bundle_items = {}

          File.open( File.join( $appassets, 'RhoBundleMap.txt' ) ) do |filelist|
            bundle_items = get_bundle_map(filelist)
          end

          remove_items = []
          add_items = []

          packed_items.each { |key,value| remove_items << key if (!bundle_items.has_key?(key) and value[:type]=='file') }

          bundle_items.each { |key,value|
            if value!=packed_items[key] and value[:type]=='file' then
              add_items << key unless key=='rho.dat'
              remove_items << key if packed_items.has_key?(key)
            end
          }

          currentdir = Dir.pwd()
          Dir.chdir $appassets+'/..'

          unless remove_items.empty? then
            args = [ 'remove', resourcepkg ]

            remove_items.each { |key|
              puts "Removing item from package: #{key}"
              args << 'assets/'+key
              packed_items.delete(key)
            }

            Jake.run($aapt, args)
            unless $?.success?
              raise "Error running AAPT (1)"
            end
          end

          unless add_items.empty? then
            args = [ 'add', resourcepkg ]

            add_items.each { |key|
              puts "Adding item to package: #{key}:#{bundle_items[key]}"
              args << 'assets/'+key
            }

            Jake.run($aapt, args)
            unless $?.success?
              raise "Error running AAPT (1)"
            end
          end


          has_changes = !(remove_items.empty? and add_items.empty?)

          if has_changes then
            puts 'Replacing bundle map and commiting package changes'

            Jake.run($aapt, ['remove',resourcepkg,'assets/RhoBundleMap.txt','assets/rho.dat'])
            unless $?.success?
              raise "Error running AAPT (1)"
            end

            Jake.run($aapt, ['add',resourcepkg,'assets/RhoBundleMap.txt','assets/rho.dat'])
            unless $?.success?
              raise "Error running AAPT (1)"
            end
          else
            puts "No changes detected in the bundle, do nothing"
          end

          Dir.chdir currentdir

          #zipfile.close

        rescue => e
          puts "EXCEPTION: #{e.inspect}"
          raise e
        end

        Rake::Task['device:android:production'].execute if has_changes

      else
        puts 'Pack everything from scratch'
        Rake::Task['device:android:production'].invoke
      end
    end
  end
end
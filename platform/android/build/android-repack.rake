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

          bundle_items = {}

          File.open( File.join( $appassets, 'RhoBundleMap.txt' ) ) do |filelist|
            bundle_items = get_bundle_map(filelist)
          end

          remove_items = []
          add_items = []

          packed_items.each_key { |key| remove_items << key unless bundle_items.has_key?(key) }

          remove_items.each { |key|
            puts "Removing item from package: #{key}"
            zipfile.remove('assets/'+key)
            packed_items.delete(key)
          }

          bundle_items.each { |key,value| (add_items<<key) unless value==packed_items[key] }

          add_items.each { |key|

            val = bundle_items[key]
            target_entry = 'assets/'+key

            if val[:type] == 'file' then

              src_path = File.join($appassets,key)

              if zipfile.find_entry(target_entry) then
                puts "Replacing item in package: #{key}:#{val}"
                zipfile.replace(target_entry,src_path)
              else
                puts "Adding item to package: #{key}:#{val}"
                zipfile.add(target_entry,src_path)
              end

            else
              puts "Adding directory to package: #{key}:#{val}"
              zipfile.remove(target_entry) if zipfile.find_entry(target_entry)
              zipfile.mkdir(target_entry)
            end
            packed_items[key] = val
          }

          has_changes = !(remove_items.empty? and add_items.empty?)

          if has_changes then
            puts 'Replacing bundle map and commiting package changes'
            zipfile.replace('assets/RhoBundleMap.txt',File.join( $appassets, 'RhoBundleMap.txt' ))
            zipfile.commit
          else
            puts "No changes detected in the bundle, do nothing"
          end

          zipfile.close

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
require 'rhom/rhom_source'

module Rho
  class RhoUtils
    @@mapSrc = nil
    def self.load_offline_data(tables=[], dir_prefix=nil)
      columns = []
      tables.each do |filename|
    
        if @@mapSrc.nil?
            @@mapSrc = {}
            arSrcs = Rhom::RhomSource.find(:all)
            arSrcs.each do |src|
                @@mapSrc[src.name()] = src.source_id()
            end
        end
                
        Rhom::RhomDbAdapter.delete_all_from_table(filename)
        Rhom::RhomDbAdapter.start_transaction

        first_row=true
        prefix = dir_prefix.nil? ? "" : dir_prefix
        File.open(File.join(Rho::RhoFSConnector.get_base_app_path,'app',prefix,'fixtures',filename+'.txt')).each do |line|
          if first_row
            columns = line.chomp.split('|'); first_row = false; next;
          end
          parts = line.chomp.split('|')

          row = {}
          columns.each_with_index do |col,idx| 
            if col == 'source_name'
              row['source_id'] = @@mapSrc[parts[idx]]
            else
              row[col] = parts[idx]
            end
          end
          Rhom::RhomDbAdapter.insert_into_table(filename,row)
        end

        Rhom::RhomDbAdapter.commit
        columns = []
      end
    end
  end
end
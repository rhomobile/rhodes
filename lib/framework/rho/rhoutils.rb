module Rho
  class RhoUtils
    def self.load_offline_data(tables=[], dir_prefix=nil)
      columns = []
      tables.each do |filename|
        srcids = {}
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
          srcid = '0'
          attrib = ''
          columns.each_with_index do |col,idx| 
            srcid = parts[idx] if col == 'source_id'
            attrib = parts[idx] if col == 'attrib'
            row[col] = parts[idx]
          end
          Rhom::RhomDbAdapter.insert_into_table(filename,row)
          
          if filename == 'object_values'
            srcids[srcid] = 1
            #Rhom::RhomAttribManager.add_attrib(srcid,attrib)
          end  
        end

        #srcids.each do |id,value|
        #    Rhom::RhomAttribManager.save(id)
        #end
            
        Rhom::RhomDbAdapter.commit
        columns = []
      end
    end
  end
end
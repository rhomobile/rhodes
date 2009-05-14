module Rho
  class RhoUtils
    def self.load_offline_data(tables=[], dir_prefix=nil)
      first_row=true
      columns = []
      tables.each do |filename|
        Rhom::RhomDbAdapter.delete_all_from_table(filename)
        prefix = dir_prefix.nil? ? "" : dir_prefix
        File.open(File.join(Rho::RhoFSConnector.get_base_app_path,'app',prefix,'fixtures',filename+'.txt')).each do |line|
          if first_row
            columns = line.chomp.split('|'); first_row = false; next;
          end
          parts = line.chomp.split('|')

          row = {}
          columns.each_with_index do |col,idx| 
            row[col] = parts[idx]
          end
          Rhom::RhomDbAdapter.insert_into_table(filename,row)
        end
        columns = []
        first_row = true
      end
    end
  end
end
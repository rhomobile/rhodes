require 'rhom/rhom_source'

module Rho
  class RhoUtils
    @@mapSrc = nil
    @@mapSrcByIdx = nil
    def self.load_offline_data(tables=[], dir_prefix=nil)
      columns = []
      tables.each do |filename|
    
        if @@mapSrc.nil?
            @@mapSrc = {}
            @@mapSrcByIdx = {}
            arSrcs = Rhom::RhomSource.find(:all)
            arSrcs.each do |src|
                @@mapSrc[src.name()] = src
                @@mapSrcByIdx[src.source_id()] = src
            end
        end
        
        mapDeleted = {}
        db = ::Rho::RHO.get_user_db()
        db.delete_all_from_table(filename)
        db.start_transaction

        first_row=true
        prefix = dir_prefix.nil? ? "" : dir_prefix
        cur_src = nil
        hashItem = {}
        cur_objid = nil
        File.open(File.join(Rho::RhoFSConnector.get_base_app_path(),'app',prefix,'fixtures',filename+'.txt')).each do |line|
          if first_row
            columns = line.chomp.split('|'); first_row = false; next;
          end
          parts = line.chomp.split('|')

          row = {}
          columns.each_with_index do |col,idx| 
            if col == 'source_name'
              row['source_id'] = @@mapSrc[parts[idx]].source_id()
            else
              row[col] = parts[idx]
            end
          end
          
          if ( cur_objid != row['object'] )
            if cur_src && hashItem && hashItem.length() > 0            
            
                if !mapDeleted[cur_src.name()]
                    db.delete_all_from_table(cur_src.name())
                    mapDeleted[cur_src.name()] = 1
                end
                
                hashItem['object'] = cur_objid
                db.insert_into_table(cur_src.name(),hashItem) 
            end
            
            hashItem = {}
            cur_objid = row['object']            
          end
          
          cur_src = @@mapSrcByIdx[ row['source_id'].to_i ]
          if ( cur_src && cur_src.schema() )  
            hashItem[ row['attrib'] ] = row['value'] if row['value']
          else
            db.insert_into_table(filename,row)
          end  
        end

        if cur_src && hashItem && hashItem.length() > 0            
        
            if !mapDeleted[cur_src.name()]
                db.delete_all_from_table(cur_src.name())
                mapDeleted[cur_src.name()] = 1
            end
            
            hashItem['object'] = cur_objid
            db.insert_into_table(cur_src.name(),hashItem) 
        end

        db.commit
        columns = []
      end
    end
    
    def self.load_offline_data_ex(tables=[], dir_prefix=nil, commit_count=100000)
      columns = []
      tables.each do |filename|

        db = ::Rho::RHO.get_user_db()    
        db.destroy_table(filename)
        db.start_transaction
        
        row_index=0
        prefix = dir_prefix.nil? ? "" : dir_prefix
        query = ""
        
        File.open(File.join(Rho::RhoFSConnector.get_base_app_path(),'app',prefix,'fixtures',filename+'.txt')).each do |line|
          if row_index == 0
            columns = line.chomp.split('|')
            quests = ""
            columns.each do |col|             
                quests += ',' if quests.length() > 0
                quests += '?'
            end
            query = "insert into #{filename} (#{columns.join(',')}) values (#{quests})"
            row_index += 1
            next
          end
          
          parts = line.chomp.split('|')
          
          begin
              db.execute_sql query, parts
          rescue Exception => e
            #puts "load_offline_data : exception insert data: #{e}; data : #{parts}; line : #{row_index}"
          end
          
          if row_index%commit_count == 0          
            puts "commit start"
            db.commit
            db.start_transaction
            puts "commit : #{row_index}"
          end  
          
          row_index += 1
        end

        db.commit
        puts "commit : #{row_index}"
        columns = []
      end
    end    
  end
end
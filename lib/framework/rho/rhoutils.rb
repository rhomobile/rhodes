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

require 'rhom/rhom_source'

module Rho
  class RhoUtils
    @@mapSrc = nil
    @@mapSrcByIdx = nil
    def self.load_offline_data(tables=[], dir_prefix=nil, source_map=nil)
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
        mapPartDeleted = {}
        #db = ::Rho::RHO.get_user_db()
        #db.delete_all_from_table(filename)
        #db.start_transaction
        db = nil
        
        first_row=true
        prefix = dir_prefix.nil? ? "" : dir_prefix
        cur_src = nil
        hashItem = {}
        cur_objid = nil
        f = File.open(File.join(Rho::RhoFSConnector.get_base_app_path(),'app',prefix,'fixtures',filename+'.txt'))
        f.each do |line|
          if first_row
            columns = line.chomp.split('|'); first_row = false; next;
          end
          parts = line.chomp.split('|')

          row = {}
          columns.each_with_index do |col,idx|
            col.strip! 
            if col == 'source_name'
              src_name = parts[idx]
              src_name = source_map[src_name] if source_map
              row['source_id'] = @@mapSrc[src_name].source_id()
            else
              row[col] = parts[idx]
            end
          end
          
          src = @@mapSrcByIdx[ row['source_id'].to_i ]
          src_db = src ? ::Rho::RHO.get_src_db(src.name() ) :  ::Rho::RHO.get_user_db()
          if src_db != db
            db.commit if db
            db = src_db
            db.start_transaction 
            
            if !mapPartDeleted[db]
                #puts  "delete_all_from_table: #{row}"
                db.delete_all_from_table(filename)
                mapPartDeleted[db] = 1
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
          #puts "cur_src: #{cur_src.schema()}" if cur_src
          if ( cur_src && cur_src.schema() )  
            hashItem[ row['attrib'] ] = row['value'] if row['value']
          else
            db.insert_into_table(filename,row)
          end  
        end                
        f.close
        
        if cur_src && hashItem && hashItem.length() > 0            
        
            if !mapDeleted[cur_src.name()]
                db.delete_all_from_table(cur_src.name())
                mapDeleted[cur_src.name()] = 1
            end
            
            hashItem['object'] = cur_objid
            db.insert_into_table(cur_src.name(),hashItem) 
        end

        db.commit if db
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
        
        f = File.open(File.join(Rho::RhoFSConnector.get_base_app_path(),'app',prefix,'fixtures',filename+'.txt'))
        f.each do |line|
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
        f.close
        
        db.commit
        puts "commit : #{row_index}"
        columns = []
      end
    end    
  end
end
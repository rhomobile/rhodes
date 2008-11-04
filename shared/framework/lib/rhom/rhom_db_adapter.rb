#
#  rhom_db_adapter.rb
#  rhodes
#
#  Copyright (C) 2008 Lars Burgess. All rights reserved.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
$:.unshift(File.join(File.dirname(__FILE__), '../../'))
require 'sqlite3/database'

module Rhom
  class RhomDbAdapter
    
    @@database = nil
    
    class << self
      def open(dbfile=nil)
        puts "DB name = " + dbfile.inspect
        unless @@database or dbfile.nil?
          @@database = SQLite3::Database.new(dbfile)
        end
      end
    
      def close
        if @@database and not @@database.closed?
          @@database.close
          @@database = nil
        else
          return false
        end
        return true
      end   
    
      # execute a sql statement
      # optionally, disable the factory processing 
      # which returns the result array directly
      def execute_sql(sql=nil)
        result = []
        if sql
          puts 'query is ' + sql
          # Make sure we lock the sync engine's mutex
          # before we perform a database transaction.
          # This prevents concurrency issues.
          begin
            SyncEngine::lock_sync_mutex
            # execute sql statement inside of transaction
            # result is returned as an array of hashes
            @@database.transaction unless @@database.transaction_active?
            @@database.results_as_hash = true
            result = @@database.execute sql
            @@database.commit
            SyncEngine::unlock_sync_mutex
          rescue Exception => e
            puts "exception when running query: #{e}"
            # make sure we unlock even if there's an error!
            SyncEngine::unlock_sync_mutex
          end
        end
        puts "returned #{result.length.to_s} records..."
        result
      end
    
      def where_str(condition)
        cond = ""
        condition.each do |key,value|
          val = value.is_a?(String) ? "'#{value}'" : "#{value}"
          cond << " #{key} = #{val} and"
        end
        cond[0..cond.length - 5]
      end
    
      def vals_str(values)
        vals = ""
        values.each do |key,value|
          val = value.is_a?(String) ? "'#{value}'" : "#{value}"
          vals << "#{key} = #{val},"
        end
        vals[0..vals.length - 2]
      end
    
      def select_from_table(table=nil,columns=nil,condition=nil,params=nil)
        puts 'inside select (table, columns, condition, params): '
        puts "(#{table.inspect}, #{columns.inspect}, #{condition.inspect}, #{params.inspect}"
        query = nil
        if table and columns and condition
          if params and params['distinct']
            query = "select distinct #{columns} from #{table} where #{where_str(condition)}"
          elsif params and params['order by']
            query = "select #{columns} from #{table} where #{where_str(condition)} \
                     order by #{params['order by']}"
          else
            query = "select #{columns} from #{table} where #{where_str(condition)}"
          end
        elsif table and columns
          query = "select #{columns} from #{table}"                     
        end
        execute_sql query
      end
    
      def insert_into_table(table=nil,values=nil)
        query = nil
        cols = ""
        vals = ""
        if table and values
          values.each do |key,val|
            value = val.is_a?(Fixnum) ? "#{val}," : "'#{val}',"
            cols << "#{key},"
            vals << value
          end
          cols = cols[0..cols.length - 2]
          vals = vals[0..vals.length - 2]
          query = "insert into #{table} (#{cols}) values (#{vals})"
        end
        execute_sql query
      end
    
      def delete_from_table(table=nil,condition=nil)
        query = nil
        if table and condition
          query = "delete from #{table} where #{where_str(condition)}"
        end
        execute_sql query
      end
    
      def delete_all_from_table(table=nil)
        query = nil
        if table
          query = "delete from #{table}"
        end
        execute_sql query
      end
    
      def update_into_table(table=nil,values=nil,condition=nil)
        query = nil
        vals = values.nil? ? nil : vals_str(values)
        if table and condition and vals
          query = "update #{table} set #{vals} where #{where_str(condition)}"
        end
        execute_sql query
      end
    end # class methods
  end # RhomDbAdapter
end # Rhom
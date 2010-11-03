#
#  rhom_db_adapter.rb
#  rhodes
#
#  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
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
require 'rhodes'

module Rhom
class RhomDbAdapter
    
  @database = nil
      
  # maintains a single database connection
  def initialize(dbfile, partition)
    unless @database
      @database = SQLite3::Database.new(dbfile,partition)
    end
  end

  # closes the database if and only if it is open
  #def close
  #  if @database
  #    @database.close
  #    @database = nil
  #  else
  #    return false
  #  end
  #  return true
  #end   

  def is_ui_waitfordb
      @database.is_ui_waitfordb
  end

  def start_transaction
      begin
        @database.start_transaction
      rescue Exception => e
        puts "exception when start_transaction: #{e}"
        raise        
      end
  end

  def commit
      begin
        @database.commit
      rescue Exception => e
        puts "exception when commit transaction : #{e}"
        raise        
      end
  end

  def rollback
      begin
        @database.rollback
      rescue Exception => e
        puts "exception when rollback transaction : #{e}"
        raise
      end
  end

  def lock_db
      begin
        @database.lock_db
      rescue Exception => e
        puts "exception when lock_db: #{e}"
        raise        
      end
  end

  def unlock_db
      begin
        @database.unlock_db
      rescue Exception => e
        puts "exception when unlock_db: #{e}"
        raise        
      end
  end

  # execute a sql statement
  # optionally, disable the factory processing 
  # which returns the result array directly
  def execute_sql(sql, *args)
    _execute_sql(sql, false, args)
  end
  def execute_batch_sql(sql, *args)
    _execute_sql(sql, true, args)
  end

  def _execute_sql(sql, is_batch, args)      
    result = []
    if sql
      #puts "RhomDbAdapter: Executing query - #{sql}; #{args}"
      begin
        result = @database.execute( sql, is_batch, args )
        #puts "result : #{result}"
      rescue Exception => e
        puts "exception when running query: #{e}"
        raise
      end
    end
    #puts "result is: #{result.inspect}"
    result
  end

  class << self
      # generates where clause based on hash
      def where_str(condition)
        where_str = ""
        if condition
          where_str += string_from_key_vals(condition,"and")
          where_str = where_str[0..where_str.length - 5]
        end
        
        where_str
      end
      
      def select_str(select_arr)
        select_str = ""
        select_arr.each do |attrib|
          select_str << "'#{attrib}'" + ","
        end
        select_str.length > 2 ? select_str[0..select_str.length-2] : select_str
      end
    
      # generates value clause based on hash
      def vals_str(values)
        vals = string_from_key_vals_set(values, ",")
        vals[0..vals.length - 2]
      end

      def string_from_key_vals_set(values, delim)
        vals = ""
        values.each do |key,value|
          op = '= '
          vals << " \"#{key}\" #{op} #{get_value_for_sql_stmt(value)} #{delim}"
        end
        vals
      end
      
      # generates key/value list
      def string_from_key_vals(values, delim)
        vals = ""
        values.each do |key,value|
          op = value.nil? ? 'is ' : '= '
          vals << " \"#{key}\" #{op} #{get_value_for_sql_stmt(value)} #{delim}"
        end
        vals
      end
      
      # generates a value for sql statement
      def get_value_for_sql_stmt(value)
        if value.nil? or value == 'NULL'
          "NULL"
        elsif value.is_a?(String)
          s = value.gsub(/'/,"''")
          "'#{s}'"
        else
          s = value.to_s.gsub(/'/,"''")
          "'#{s}'"
        end
      end
    end #self

  # support for select statements
  # this function takes table name, columns (as a comma-separated list),
  # condition (as a hash), and params (as a hash)
  # example usage is the following:
  # select_from_table('object_values', '*', {"source_id"=>2,"update_type"=>'query'},
  #                   {"order by"=>'object'})
  # this would return all columns where source_id = 2 and update_type = 'query' ordered
  # by the "object" column
  def select_from_table(table=nil,columns=nil,condition=nil,params=nil)
  
    raise ArgumentError if !table || !columns
    query = nil
    vals = nil

    if condition
        quests,vals = make_where_params(condition,'AND') 
        if params and params['distinct']
            query = "select distinct #{columns} from #{table} where #{quests}"
        elsif params and params['order by']
            query = "select #{columns} from #{table} where #{quests} order by #{params['order by']}"
        else
            query = "select #{columns} from #{table} where #{quests}"
        end
    else
        query = "select #{columns} from #{table}"
    end
    
    execute_sql query, vals
  end

  def make_where_params(condition,op)
    raise ArgumentError if !condition || !op || op.length == 0
    quests = ""
    vals = []
  
    condition.each do |key,val|
        if quests.length > 0
            quests << ' ' << op << ' '
        end
    
        quests << "\"#{key}\"=?"
        vals << val
    end
    
    return quests,vals
  end

  # inserts a single row into the database
  # takes the table name and values (hash) as arguments
  # exmaple usage is the following:
  # insert_into_table('object_values, {"source_id"=>1,"object"=>"some-object","update_type"=>'delete'})
  # this would execute the following sql:
  # insert into object_values (source_id,object,update_type) values (1,'some-object','delete');
  def insert_into_table(table=nil,values=nil, excludes=nil)
    raise ArgumentError if !table
    cols,quests,vals = make_insert_params(values, excludes)
    query = "insert into #{table} (#{cols}) values (#{quests})"
    execute_sql query, vals
  end

  def make_insert_params(values, excludes)
    raise ArgumentError if !values
    
    cols = ""
    quests = ""
    vals = []
  
    values.each do |key,val|
        next if excludes && excludes[key]
        if cols.length > 0
            cols << ','
            quests << ','
        end
    
        cols << "\"#{key}\""
        quests << '?'
        vals << val
    end
    
    return cols,quests,vals
  end

  # deletes rows from a table which satisfy condition (hash)
  # example usage is the following:
  # delete_from_table('object_values',{"object"=>"some-object"})
  # this would execute the following sql:
  # delete from object_values where object="some-object"
  def delete_from_table(table,condition)
    raise ArgumentError if !table
    quests,vals = make_where_params(condition,'AND') 
    query = "delete from #{table} where #{quests}"
    execute_sql query, vals
  end

  # deletes all rows from a given table
  def delete_all_from_table(table)
    execute_sql "delete from #{table}"
  end

  def table_exist?(table_name)
    @database.table_exist?(table_name)
  end

  def delete_table(table)
    execute_sql "DROP TABLE IF EXISTS #{table}"
  end
  
  #destroy one table  
  def destroy_table(name)
    destroy_tables(:include => [name])
  end
  
  # deletes all rows from all tables, except list of given tables by recreating db-file and save all other tables
  # arguments - :include, :exclude
  def destroy_tables(*args)
      @database.destroy_tables args.first[:include], args.first[:exclude]
  end
  
  # updates values (hash) in a given table which satisfy condition (hash)
  # example usage is the following:
  # update_into_table('object_values',{"value"=>"Electronics"},{"object"=>"some-object", "attrib"=>"industry"})
  # this executes the following sql:
  # update table object_values set value='Electronics' where object='some-object' and attrib='industry';
  def update_into_table(table=nil,values=nil,condition=nil)
    raise ArgumentError if !table || !values
    query = nil
    vals = nil
    if condition
        quests_set, vals_set = make_set_params(values)
        quests_where,vals_where = make_where_params(condition,'AND') 
        query = "update #{table} set #{quests_set} where #{quests_where}"
        vals = vals_set + vals_where
    else
        quests, vals = make_set_params(values)
        query = "update #{table} set #{quests}"
    end
    
    execute_sql query, vals
  end
  
  def make_set_params(values)
    raise ArgumentError if !values
    
    quests = ""
    vals = []
  
    values.each do |key,val|
        if quests.length > 0
            quests << ','
        end
    
        quests << "\"#{key}\"=?"
        vals << val
    end
    
    return quests,vals
  end
  
end # RhomDbAdapter
end # Rhom

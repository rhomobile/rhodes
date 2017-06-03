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

require 'rhodes'

module Rhom
class RhomDbAdapter
    
  @database = nil
  @dbpath = nil  
      
  # maintains a single database connection
  def initialize(dbfile, partition)
    unless @database
      @dbpath = dbfile  
      @database = SQLite3::Database.new(dbfile,partition)
    end
  end

  # closes the database if and only if it is open
  def close
    if @database
      @database.close
      @database = nil
      @dbpath = nil
    else
      return false
    end
    return true
  end   

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
	
  def export
	  begin
		@database.export
	  rescue Exception => e
		puts "exception when export database: #{e}"
		raise        
      end
  end
	
  def import(zipName)
	  begin
		@database.import(zipName)
	  rescue Exception => e
		puts "exception when import database: #{e}"
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
      def get_value_for_sql_stmt(value, convert_value_to_string=true)
        if value.nil? or value == 'NULL'
          "NULL"
        elsif value.is_a?(String)
          s = value.gsub(/'/,"''")
          "'#{s}'"
        else
          if convert_value_to_string
            s = value.to_s.gsub(/'/,"''")
            "'#{s}'"
          else
            "#{value}"            
          end  
        end
      end
      
      def make_where_params(condition,op)
        raise ArgumentError if !condition || !op || op.length == 0
        quests = ""
        vals = []
      
        condition.each do |key,val|
            if quests.length > 0
                quests << ' ' << op << ' '
            end

            if val.nil?        
                quests << "\"#{key}\" IS NULL" 
            else
                quests << "\"#{key}\"=?"
				vals << val
            end
            
        end
        
        return quests,vals
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
    vals = []

    if condition
        quests,vals = RhomDbAdapter.make_where_params(condition,'AND') 
        if params and params['distinct']
            query = "SELECT DISTINCT #{columns} FROM \"#{table}\" WHERE #{quests}"
        elsif params and params['order by']
            query = "SELECT #{columns} FROM \"#{table}\" WHERE #{quests} ORDER BY #{params['order by']}"
        else
            query = "SELECT #{columns} FROM \"#{table}\" WHERE #{quests}"
        end
    else
        query = "SELECT #{columns} FROM \"#{table}\""
    end
    
    execute_sql query, vals
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
    query = "INSERT INTO \"#{table}\" (#{cols}) VALUES (#{quests})"
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

  def set_do_not_bakup_attribute(attr=1)
       if System::get_property('platform') == 'APPLE'
              System.set_do_not_bakup_attribute(@dbpath, attr)
              System.set_do_not_bakup_attribute(@dbpath+'.version', attr)
       end                         
  end

  # deletes rows from a table which satisfy condition (hash)
  # example usage is the following:
  # delete_from_table('object_values',{"object"=>"some-object"})
  # this would execute the following sql:
  # delete from object_values where object="some-object"
  def delete_from_table(table,condition)
    raise ArgumentError if !table
    quests,vals = RhomDbAdapter.make_where_params(condition,'AND') 
    query = "DELETE FROM \"#{table}\" WHERE #{quests}"
    execute_sql query, vals
  end

  # deletes all rows from a given table
  def delete_all_from_table(table)
    execute_sql "DELETE FROM \"#{table}\""
  end

  def table_exist?(table_name)
    @database.table_exist?(table_name)
  end

  def delete_table(table)
    execute_sql "DROP TABLE IF EXISTS \"#{table}\""
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
    vals = []
    if condition
        quests_set, vals_set = make_set_params(values)
        quests_where,vals_where = RhomDbAdapter.make_where_params(condition,'AND') 
        query = "UPDATE \"#{table}\" SET #{quests_set} WHERE #{quests_where}"
        vals = vals_set + vals_where
    else
        quests, vals = make_set_params(values)
        query = "UPDATE \"#{table}\" SET #{quests}"
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

#
#  rhorm.rb
#  rhodes
#  This module represents the rhodes ORM
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
require 'sqlite3'
  
class Rhom
  attr_accessor :database, :attrs
	
  TABLE_NAME='object_values'
  
  class << self
    def init_db_connection
      dbname =  File.join(File.dirname(File.expand_path(__FILE__)), 'syncdb.sqlite')
      puts "DB name = " + dbname
      @database = SQLite3::Database.new(dbname)
    end
  
    def close_db_connection
      if @database
        @database.close
      else
        return false
      end
      return true
    end
  
    def execute_sql(sql=nil)
      result = nil
      if sql
        init_db_connection
        @database.transaction
        result = @database.execute sql
        @database.commit
        close_db_connection
      end
      result
    end
	
    def find(obj_id)
      execute_sql "select * from #{TABLE_NAME} where id=#{obj_id.to_s}"
    end
    
    def find(condition)
      execute_sql "select * from #{TABLE_NAME} where #{condition}"
    end
    
    def find_by_sql(sql)
      execute_sql sql
    end
    
    def update(attributes)
      #TODO: Inspect attributes and generate sql
    end
  end # class methods
end # Rhom

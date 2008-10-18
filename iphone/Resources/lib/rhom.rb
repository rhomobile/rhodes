#
#  rhom.rb
#  rhodes
#  This module represents the rhodes mini OM
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
require 'rhom_object_factory'
  
class Rhom
  attr_accessor :database, :modelname, :factory, :source_id
	
  TABLE_NAME='object_values'
  
  def initialize(modelname, source_id)
    @modelname = modelname
    @source_id = source_id
    @factory = RhomObjectFactory.new(@modelname, @source_id)
	puts 'source_id ' + @source_id.to_s
	@factory.init_attrib_count(Rhom::execute_sql("select count(distinct attrib) as count from \
												#{TABLE_NAME} where source_id = #{@source_id.to_s}", false))
  end
  
  class << self
    def init_db_connection
      dbname =  File.join(File.dirname(File.expand_path(__FILE__)), '../db/syncdb.sqlite')
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
  
    # execute a sql statement
    # optionally, disable the factory processing 
    # which returns the result array directly
    def execute_sql(sql=nil, process=true)
      result = nil
      if sql
        # execute sql statement inside of transaction
        # result is returned as an array of hashes
        init_db_connection
        @database.transaction
        @database.results_as_hash = true
        result = @database.execute sql
        @database.commit
        close_db_connection
      end
      return process ? @factory.get_list(result) : result
    end
  end # class methods
end # Rhom

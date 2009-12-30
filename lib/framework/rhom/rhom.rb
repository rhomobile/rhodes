#
#  rhom.rb
#  rhodes
#  This module represents the rhodes mini OM
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

require 'rhom/rhom_object_factory'
require 'rhom/rhom_object'
require 'rhom/rhom_db_adapter'

module Rhom
  UPDATE_TYPES = ["'create'","'query'","'ask'"]
  
  class RecordNotFound < StandardError
  end
  
  class Rhom
    attr_accessor :factory
  
    def initialize
      @factory = RhomObjectFactory.new
    end
    
    class << Rhom
      def client_id
        c_id = ::Rhom::RhomDbAdapter.select_from_table('client_info','client_id')[0]
        c_id.nil? ? nil : c_id['client_id']
      end
      
      def database_full_reset
        SyncEngine.stop_sync
        
        ::Rhom::RhomDbAdapter.execute_sql("UPDATE client_info SET reset=1")
        ::Rhom::RhomDbAdapter.execute_sql("UPDATE client_info SET initialsync_state=0")
        ::Rhom::RhomDbAdapter.execute_sql("UPDATE sources SET token=0")
        
        ::Rhom::RhomDbAdapter.delete_all_from_table('changed_values')
#        if defined? RHO_DBME
            #::Rhom::RhomAttribManager.reset_all
            ::Rhom::RhomDbAdapter.destroy_table('object_values')
            #::Rhom::RhomDbAdapter.delete_all_from_table('object_values')
#        else
#            ::Rhom::RhomDbAdapter.delete_all_from_table('object_values')
#            ::Rhom::RhomDbAdapter.execute_sql("VACUUM")
#        end    
      end
      
      def database_full_reset_and_logout
        database_full_reset
        SyncEngine.logout
      end
    end #class methods
  end # Rhom
end # Rhom

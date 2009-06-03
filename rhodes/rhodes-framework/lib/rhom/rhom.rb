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
  class Rhom
    attr_accessor :factory
  
    def initialize
      @factory = RhomObjectFactory.new
    end
    
    class << Rhom
      def client_id
        c_id = RhomDbAdapter.select_from_table('client_info','client_id')[0]
        c_id.nil? ? nil : c_id['client_id']
      end
      
      def database_full_reset
        RhomDbAdapter.delete_all_from_table('object_values')
        RhomDbAdapter.delete_all_from_table('client_info')
      end
      
      def database_full_reset_and_logout
        database_full_reset
        SyncEngine.logout
      end
    end #class methods
  end # Rhom
end # Rhom

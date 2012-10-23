 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
module Rhom
  class RhomDbAdapter
    def self.where_str
    end    
    def self.select_str
    end    
    def self.vals_str
    end    
    def self.string_from_key_vals_set
    end    
    def self.string_from_key_vals
    end    
    def self.get_value_for_sql_stmt
    end    
    def self.make_where_params
    end    
    def close
    end    
    def is_ui_waitfordb
    end    
    def start_transaction
    end    
    def commit
    end    
    def rollback
    end    
    def lock_db
    end    
    def unlock_db
    end    
    def export
    end    
    def import
    end    
    def execute_sql
    end    
    def execute_batch_sql
    end    
    def _execute_sql
    end    
    def select_from_table
    end    
    def insert_into_table
    end    
    def make_insert_params
    end    
    def set_do_not_bakup_attribute
    end    
    def delete_from_table
    end    
    def delete_all_from_table
    end    
    def table_exist?
    end    
    def delete_table
    end    
    def destroy_table
    end    
    def destroy_tables
    end    
    def update_into_table
    end    
    def make_set_params
    end    
  end

  module RhomObject
    def self.method_name_reserved?
    end    
    def strip_braces
    end    
    def djb_hash
    end    
    def extract_options
    end    
  end

  class RhomObjectFactory
    def self.init_object
    end    
  end

  module BaseModel
    def self.model_params
    end    
    def self.model_params=
    end    
    def get_model_params
    end    
    def reset_model_params
    end    
    def fixed_schema?
    end    
    def init_defaults
    end    
    def property
    end    
    def set
    end    
    def enable
    end    
    def belongs_to
    end    
    def index
    end    
    def unique_index
    end    
  end

  module FixedSchema
    def self.included
    end    
    def fixed_schema?
    end    
  end

  module PropertyBag
    def self.included
    end    
  end

  class RecordNotFound
  end

  class Rhom
    def self.client_id
    end    
    def self.database_client_reset
    end    
    def self.database_local_reset
    end    
    def self.database_full_reset_ex
    end    
    def self.database_full_reset
    end    
    def self.database_full_reset_and_logout
    end    
    def self.database_fullclient_reset_and_logout
    end    
    def self.database_export
    end    
    def self.database_import
    end    
    def self.search
    end    
    def self.have_local_changes
    end    
    def factory
    end    
    def factory=
    end    
  end

  class RhomSource
    def self.load_all_sources
    end    
    def self.find
    end    
    def self.find_all_ashash
    end    
    def source_id
    end    
    def name
    end    
    def last_updated
    end    
    def last_inserted_size
    end    
    def last_deleted_size
    end    
    def last_sync_duration
    end    
    def last_sync_success
    end    
    def distinct_objects
    end    
    def backend_refresh_time
    end    
    def partition
    end    
    def schema
    end    
    def schema_version
    end    
    def get_lastsync_objectcount
    end    
  end

  def self.any_model_changed?
  end  
end


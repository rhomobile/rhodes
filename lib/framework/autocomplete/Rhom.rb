 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
module Rhom
  class RhomDbAdapter
    def self.where_str(condition)
    end    
    def self.select_str(select_arr)
    end    
    def self.vals_str(values)
    end    
    def self.string_from_key_vals_set(values,delim)
    end    
    def self.string_from_key_vals(values,delim)
    end    
    def self.get_value_for_sql_stmt(value,convert_value_to_string)
    end    
    def self.make_where_params(condition,op)
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
    def import(zipName)
    end    
    def execute_sql(sql,args)
    end    
    def execute_batch_sql(sql,args)
    end    
    def _execute_sql(sql,is_batch,args)
    end    
    def select_from_table(table,columns,condition,params)
    end    
    def insert_into_table(table,values,excludes)
    end    
    def make_insert_params(values,excludes)
    end    
    def set_do_not_bakup_attribute(attr)
    end    
    def delete_from_table(table,condition)
    end    
    def delete_all_from_table(table)
    end    
    def table_exist?(table_name)
    end    
    def delete_table(table)
    end    
    def destroy_table(name)
    end    
    def destroy_tables(args)
    end    
    def update_into_table(table,values,condition)
    end    
    def make_set_params(values)
    end    
  end

  module RhomObject
    def self.method_name_reserved?(method)
    end    
    def strip_braces(str)
    end    
    def djb_hash(str,len)
    end    
    def extract_options(arr)
    end    
  end

  class RhomObjectFactory
    def self.init_object(classname)
    end    
  end

  module BaseModel
    def self.model_params
    end    
    def self.model_params=(req)
    end    
    def get_model_params
    end    
    def reset_model_params
    end    
    def fixed_schema?
    end    
    def init_defaults
    end    
    def property(name,type,option)
    end    
    def set(name,value)
    end    
    def enable(name)
    end    
    def belongs_to(name,owner)
    end    
    def index(name,cols)
    end    
    def unique_index(name,cols)
    end    
  end

  module FixedSchema
    def self.included(model)
    end    
    def fixed_schema?
    end    
  end

  module PropertyBag
    def self.included(model)
    end    
  end

  class RecordNotFound
  end

  class Rhom
    def self.client_id
    end    
    def self.database_client_reset(reset_local_models)
    end    
    def self.database_local_reset
    end    
    def self.database_full_reset_ex(args)
    end    
    def self.database_full_reset(reset_client_info,reset_local_models)
    end    
    def self.database_full_reset_and_logout
    end    
    def self.database_fullclient_reset_and_logout
    end    
    def self.database_export(partition)
    end    
    def self.database_import(partition,zipName)
    end    
    def self.search(args)
    end    
    def self.have_local_changes
    end    
    def factory
    end    
    def factory=(req)
    end    
  end

  class RhomSource
    def self.load_all_sources
    end    
    def self.find(args)
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


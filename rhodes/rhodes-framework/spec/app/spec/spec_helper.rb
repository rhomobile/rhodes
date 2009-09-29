require 'rho/rho'
require 'rhom/rhom'
require 'rho/rhoutils'

describe "rhom initializer", :shared => true do
  before(:each) do
    Rho::RhoUtils.load_offline_data(['client_info','object_values'], 'spec')
  end

  after(:each) do
    Rhom::RhomDbAdapter.start_transaction
    #Rhom::RhomAttribManager.reset_all
    Rhom::RhomDbAdapter.delete_all_from_table('client_info')
    Rhom::RhomDbAdapter.delete_all_from_table('object_values')
    Rhom::RhomDbAdapter.delete_all_from_table('changed_values')
    Rhom::RhomDbAdapter.commit
  end
end
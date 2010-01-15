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


class Object

  # Returns the Enumerator class (either Enumerator or Enumerable::Enumerator)
  # depending of the version.
  #
  def enumerator_class
    SpecVersion.new(RUBY_VERSION) < "1.9" ? Enumerable::Enumerator : Enumerator
  end
end

class Encoding
  ASCII_8BIT = Encoding.find("ASCII-8BIT")
  US_ASCII = Encoding.find("US-ASCII")
  UTF_8 = Encoding.find("UTF-8")

  #UTF_32BE = Encoding.find("UTF-32BE")
end

TOLERANCE = 0.00003 unless Object.const_defined?(:TOLERANCE)
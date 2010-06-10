require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/store.rb'

describe "ENV.store" do
  it_behaves_like(:env_store, :store)
end

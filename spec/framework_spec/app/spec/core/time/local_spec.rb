require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/local'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/time_params'

describe "Time.local" do
  it_behaves_like(:time_local, :local)
  it_behaves_like(:time_params, :local)
end

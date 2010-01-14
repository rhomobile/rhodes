require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/local'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/time_params'

describe "Time.mktime" do
  it_behaves_like(:time_local, :mktime)
  it_behaves_like(:time_params, :mktime)
end

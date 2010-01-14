require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/gm'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/time_params'

describe "Time.gm" do
  it_behaves_like(:time_gm, :gm)
  it_behaves_like(:time_params, :gm)
end

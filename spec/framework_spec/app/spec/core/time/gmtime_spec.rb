require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/gmtime'

describe "Time#gmtime" do
  it_behaves_like(:time_gmtime, :gmtime)
end

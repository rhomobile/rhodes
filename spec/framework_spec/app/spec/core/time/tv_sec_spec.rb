require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/to_i'

describe "Time#tv_sec" do
  it_behaves_like(:time_to_i, :tv_sec)
end

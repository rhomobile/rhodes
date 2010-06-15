require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/common'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/pos'

describe "Dir#seek" do
  it "returns the Dir instance" do
    @dir.seek(@dir.pos).should == @dir
  end

  it_behaves_like :dir_pos_set, :seek
end

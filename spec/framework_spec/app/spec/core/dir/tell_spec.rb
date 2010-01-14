require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/common'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/closed'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/pos'

describe "Dir#tell" do
  it_behaves_like :dir_pos, :tell
end

describe "Dir#tell" do
  it_behaves_like :dir_closed, :tell
end

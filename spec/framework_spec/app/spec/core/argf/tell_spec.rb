require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/pos'

describe "ARGF.tell" do
  it_behaves_like :argf_pos, :tell
end

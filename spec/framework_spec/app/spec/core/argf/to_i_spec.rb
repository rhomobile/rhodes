require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/fileno'

describe "ARGF.to_i" do
  it_behaves_like :argf_fileno, :to_i
end

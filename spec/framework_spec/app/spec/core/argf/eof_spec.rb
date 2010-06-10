require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/eof'

describe "ARGF.eof" do
  it_behaves_like :argf_eof, :eof
end

describe "ARGF.eof?" do
  it_behaves_like :argf_eof, :eof?
end

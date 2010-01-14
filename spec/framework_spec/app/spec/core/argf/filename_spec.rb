require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/filename'

describe "ARGF.filename" do
  it_behaves_like :argf_filename, :filename
end

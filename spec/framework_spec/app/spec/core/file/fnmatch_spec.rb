require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/fnmatch'

describe "File.fnmatch" do
  it_behaves_like(:file_fnmatch, :fnmatch)
end

describe "File.fnmatch?" do
  it_behaves_like(:file_fnmatch, :fnmatch?)
end

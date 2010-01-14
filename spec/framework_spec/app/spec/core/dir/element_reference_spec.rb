require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/common'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/glob'

describe "Dir.[]" do
  it_behaves_like :dir_glob, :[]
end

describe "Dir.[]" do
  it_behaves_like :dir_glob_recursive, :[]
end

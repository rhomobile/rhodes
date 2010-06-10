require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/symlink'

describe "FileTest.symlink?" do
  it_behaves_like :file_symlink, :symlink?, FileTest
  it_behaves_like :file_symlink_nonexistent, :symlink?, File
end

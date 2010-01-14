require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../shared/file/symlink'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "File::Stat#symlink?" do
  it_behaves_like :file_symlink, :symlink?, FileStat
end

require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../shared/file/grpowned'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "File::Stat#grpowned?" do
  it_behaves_like :file_grpowned, :grpowned?, FileStat
end

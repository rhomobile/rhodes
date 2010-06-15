require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../shared/file/executable_real'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "File::Stat#executable_real?" do
  it_behaves_like :file_executable_real, :executable_real?, FileStat
end

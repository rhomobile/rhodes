require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/executable'

describe "File.executable?" do
  it_behaves_like :file_executable, :executable?, File
  it_behaves_like :file_executable_missing, :executable?, File
end

require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/directory'

describe "File.directory?" do
  it_behaves_like :file_directory, :directory?, File
end

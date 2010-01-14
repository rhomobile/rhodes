require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/owned'

describe "File.owned?" do
  it_behaves_like :file_owned, :owned?, File
end

describe "File.owned?" do
  it "needs to be reviewed for spec completeness"
end

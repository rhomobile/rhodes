require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/tty'

describe "IO#tty?" do
  it_behaves_like :io_tty, :tty?
end

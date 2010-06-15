require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/wakeup'

describe "Thread#wakeup" do
  it_behaves_like :thread_wakeup, :wakeup
end

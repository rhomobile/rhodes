#
#  rhom_object_factory_test.rb
#  rhodes
#
#  Copyright (C) 2008 Lars Burgess. All rights reserved.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
$:.unshift File.join(File.dirname(__FILE__),'..','lib')

require 'test/unit'
require 'rhom_object_factory'

class RhomObjectFactoryTest < Test::Unit::TestCase
  
  def test_init_and_set
    RhomObjectFactory.new("Account")
    
    account = Account.new
    account.name = 'hello name'
    account.industry = 'hello industry'
    account.object = '3560c0a0-ef58-2f40-68a5-fffffffffffff'
    account.value = 'xyz industries'
    assert_equal 'hello name', account.name
    assert_equal 'hello industry', account.industry
    assert_equal '3560c0a0-ef58-2f40-68a5-fffffffffffff', account.object
    assert_equal 'xyz industries', account.value
  end
  
  def test_init_with_result_set
    rows = [{"id"=>"1", 
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"abc consulting", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"}, 
      {"id"=>"2",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"Entertainment", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"}]
    factory = RhomObjectFactory.new("Account")
    
    results = factory.get_list(rows)
    p 'results: ' + results.inspect
    assert_equal results.length, 2
    assert_equal results[0].value, "abc consulting"
  end
end

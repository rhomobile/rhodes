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
  
  Object::const_set("RHO_SOURCES", {"Account"=>'1', "Case"=>'2'}) unless defined? RHO_SOURCES
  
  def test_init_and_set
    
    RhomObjectFactory.new
    puts 'calling source_id account: ' + Account.get_source_id
    puts 'calling source_id case: ' + Case.get_source_id
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
  
  def test_init_with_result_set_one_unique
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
    RhomObjectFactory.new
    
    results = Case.get_list(rows)
    results.each_with_index do |result,i|
      puts "result[#{i}]: " + result.inspect
    end
    # we should only have one row
    assert_equal 1, results.length
    assert_equal results[0].name, "abc consulting"
    assert_equal results[0].industry, "Entertainment"
  end
  
  def test_init_with_result_set_two_unique
    rows = [{"id"=>"1", 
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"abc solutions", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"}, 
      {"id"=>"2",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"400 Main St.", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"3",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"sports", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"4",             
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-ef58-2f40-df43-48f39f63741b", 
        "value"=>"xyz consulting", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"5",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-ef58-2f40-df43-48f39f63741b", 
        "value"=>"555 5th street", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"6",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-ef58-2f40-df43-48f39f63741b", 
        "value"=>"software", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"}]
    RhomObjectFactory.new
    
    results = Account.get_list(rows)
    results.each_with_index do |result,i|
      puts "result[#{i}]: " + result.inspect
    end
    assert_equal 2, results.length
    
    
    assert_equal results[0].name, "abc solutions"
    assert_equal results[0].address, "400 Main St."
    assert_equal results[0].industry, "sports"
    #TODO: Both objects are the same, so this fails!
    assert_equal results[1].name, "xyz consulting"
    assert_equal results[1].address, "555 5th street"
    assert_equal results[1].industry, "software"
  end
  
  def test_init_with_result_set_five_unique
    rows = [{"id"=>"1", 
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"abc solutions", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"}, 
      {"id"=>"2",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"400 Main St.", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"3",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-ef58-2f40-68a5-48f39f63741b", 
        "value"=>"sports", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"4",             
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-ef58-2f40-df43-48f39f63741b", 
        "value"=>"xyz consulting", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"5",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-ef58-2f40-df43-48f39f63741b", 
        "value"=>"555 5th street", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"6",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-ef58-2f40-df43-48f39f63741b", 
        "value"=>"software", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"7",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-ef58-0000-df43-48f39f63741b", 
        "value"=>"111 1st street", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"8",             
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-ef58-0000-df43-48f39f63741b", 
        "value"=>"jkl industries", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"9",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-ef58-0000-df43-48f39f63741b", 
        "value"=>"construction", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"10",             
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-1111-2f40-df43-48f39f63741b", 
        "value"=>"mno industries", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"11",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-1111-2f40-df43-48f39f63741b", 
        "value"=>"222 2nd street", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"12",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-1111-2f40-df43-48f39f63741b", 
        "value"=>"plumbing", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"13",             
        "source_id"=>"1", 
        "attrib"=>"name", 
        "object"=>"3560c0a0-2222-2f40-df43-48f39f63741b", 
        "value"=>"pqr industries", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"14",             
        "source_id"=>"1", 
        "attrib"=>"address", 
        "object"=>"3560c0a0-2222-2f40-df43-48f39f63741b", 
        "value"=>"333 3rd street", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"},
      {"id"=>"15",             
        "source_id"=>"1", 
        "attrib"=>"industry", 
        "object"=>"3560c0a0-2222-2f40-df43-48f39f63741b", 
        "value"=>"lighting", 
        "created_at"=>"2008-10-14 01:53:17", 
        "updated_at"=>"2008-10-14 01:53:17", 
        "update_type"=>"create"}]
    RhomObjectFactory.new
    
    results = Account.get_list(rows)
    results.each_with_index do |result,i|
      puts "result[#{i}]: " + result.inspect
    end
    
    assert_equal 5, results.length
    
    
    assert_equal results[0].name, "abc solutions"
    assert_equal results[0].address, "400 Main St."
    assert_equal results[0].industry, "sports"
    assert_equal results[1].name, "xyz consulting"
    assert_equal results[1].address, "555 5th street"
    assert_equal results[1].industry, "software"
    assert_equal results[2].name, "jkl industries"
    assert_equal results[2].address, "111 1st street"
    assert_equal results[2].industry, "construction"
    assert_equal results[3].name, "mno industries"
    assert_equal results[3].address, "222 2nd street"
    assert_equal results[3].industry, "plumbing"
    assert_equal results[4].name, "pqr industries"
    assert_equal results[4].address, "333 3rd street"
    assert_equal results[4].industry, "lighting"
  end
  
  def test_djb_hash
    RhomObjectFactory.new
    vars = {"name"=>"foobarthree", "industry"=>"entertainment"}
    account = Account.new(vars)
    puts 'account: ' + account.inspect
    assert_equal "272128608299468889014", account.object
  end
end

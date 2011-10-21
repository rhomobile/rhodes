#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------

require 'bsearch'
require 'time'

if defined? RHO_ME
  require 'rho/rhoevent_bb'
else
  require 'rho/rhoevent_c'
end

module Rho
	module RhoEvent
    # These values determined experimentally on iPhone
    MIN_TIME = Time.utc(2008, 'jan', 1, 0, 0, 0)
    MAX_TIME = Time.utc(2030, 'dec', 31, 23, 59, 59)

    #TODO: update examples
    # Examples of how to use select method:
    #
      # selected = Rho::RhoEvent.select('first_name' => 'David') { |x| x[1]['last_name']=='Taylor' }
    # ==> returns record(s) of the David Taylor
    #
      # selected = Rho::RhoEvent.select('first_name' => 'Kate')
    # ==> Returns all records of Kate
    #
      # selected = Rho::RhoEvent.select('last_name' => 'User') do |x|
      # 	x[1]['first_name']=='Test' and x[1]['company_name']=="rhomobile"
      # end
    # ==> returns all records of the Test User from the company rhomobile
    #
    def self.select(index, &block)
      key, value = index.keys[0], index.values[0]
      if @events.nil? or @key != key
        @key, @events = key, find(:all).to_a.sort! {|x,y| x[1][key] <=> y[1][key] }
      end
      found = @events[@events.bsearch_range {|x| x[1][key] <=> value}]
      unless found.nil? or block.nil?
        return found.select(&block)
      end
      return found
    end

	end # module RhoEvent
end # module Rho

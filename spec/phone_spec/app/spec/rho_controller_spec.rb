#
#  rho_controller_spec.rb
#  rhodes
#
#  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
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
#require 'spec/spec_helper'
require 'rho/rhocontroller'

module Rho
  class RhoControllerStub < Rho::RhoController
    def send(action)
        render :string => "", :layout => false
    end
    
    def index
        render :string => "", :layout => false
    end
    
  end
end

describe "url_for and link_to" do

    before(:all) do
        @application = AppApplication.new
    end
    
    before do
      @c = ::Rho::RhoControllerStub.new()
      @c.serve(@application,nil,{'application' => 'application', 'model' => 'model', 'request-method' => 'GET', :modelpath => 'model', 'headers' => {} },{})
    end

    it "should generate urls for empty params" do
      @c.url_for().should == '/application/model'
    end

    it "should generate urls for a url" do
      @c.url_for('/some_url').should == '/some_url'
    end

    it "should generate urls for a symbol" do
      @c.url_for(:new).should == 'new'
    end

    it "should generate urls for an action" do
      @c.url_for(:action => :new).should == '/application/model/new'
    end

    it "should generate urls for an action and id" do
      @c.url_for(:action => :show, :id => '{12}').should == '/application/model/{12}/show'
    end

    it "should generate urls for create or index actions" do
      @c.url_for(:action => :index, :id => '{12}').should == '/application/model'
      @c.url_for(:action => :create).should == '/application/model'
    end

    it "should generate urls for an model, action, and id" do
      @c.url_for(:model => 'another_model', :action => :show, :id => '{12}').should == '/application/another_model/{12}/show'
      @c.url_for(:model => :another_model, :action => :show, :id => '{12}').should == '/application/another_model/{12}/show'
    end

    it "should generate urls for an controller, action, and id" do
      @c.url_for(:controller => 'another_controller', :action => :show, :id => '{12}').should == '/application/another_controller/{12}/show'
      @c.url_for(:controller => :another_controller,  :action => :show, :id => '{12}').should == '/application/another_controller/{12}/show'
    end

    it "should generate urls for an application, model, action, and id" do
      @c.url_for(:application => :another_app, :model => :another_model, 
        :action => :show, :id => '{12}').should == '/another_app/another_model/{12}/show'
    end

    it "should generate urls with a query" do
      @c.url_for(:action => :create, :query => {:name => 'John Smith', 
        'address' => "http://john.smith.com"}).should == '/application/model?name=John%20Smith&address=http%3A%2F%2Fjohn.smith.com'
    end

    it "should generate urls with a fragment" do
      @c.url_for(:action => :show, :id => '{12}', :fragment => "an-anchor").should == '/application/model/{12}/show#an-anchor'
    end

    it "should generate link for a url" do
      @c.link_to("Visit Other Site", "http://www.rhomobile.com/").should == "<a href=\"http://www.rhomobile.com/\" >Visit Other Site</a>"
    end

    it "should generate link for an action" do
      @c.link_to("Help", :action => "help").should == "<a href=\"/application/model/help\" >Help</a>"
    end

    it "should generate link for 'delete' action" do
      @c.link_to("Delete", :action => "delete", :id => '{12}').should == "<a href=\"/application/model/{12}/delete\"  onclick=\"var f = document.createElement('form'); f.style.display = 'none';this.parentNode.appendChild(f); f.method = 'POST'; f.action = this.href;f.submit();return false;\">Delete</a>"
    end

    it "should generate link for an action and style it" do
      @c.link_to("Show", { :action => "show", :id => '{12}'},"style=\"height:4px;width:7px;border-width:0px;\"").should == "<a href=\"/application/model/{12}/show\" style=\"height:4px;width:7px;border-width:0px;\">Show</a>"
    end

    it "should generate link for 'delete' action and style it" do
      @c.link_to("Delete", { :action => "delete", :id => '{12}' }, "class=\"delete_link\"").should == "<a href=\"/application/model/{12}/delete\" class=\"delete_link\" onclick=\"var f = document.createElement('form'); f.style.display = 'none';this.parentNode.appendChild(f); f.method = 'POST'; f.action = this.href;f.submit();return false;\">Delete</a>"
    end

    it "should generate link with a query" do
      @c.link_to("Invate",:action => :invite, :query => {:name => 'John Smith', 'address' => "http://john.smith.com"}).should == "<a href=\"/application/model/invite?name=John%20Smith&address=http%3A%2F%2Fjohn.smith.com\" >Invate</a>"
    end

    it "should generate link with a action" do
      res = @c.url_for :controller => :Settings, :action => :login
      res.should == "/application/Settings/login"
    end
        
end #describe "url_for and link_to"

describe "redirect" do

    before(:all) do
        @application = AppApplication.new
    end

    before do
      @response = Hash.new
      @response['headers'] = {}
      @c = Rho::RhoControllerStub.new
      @c.serve(@application,nil,{'application' => 'application', 'model' => 'model', 'request-method' => 'GET', :modelpath => 'model', 'headers' => {} },@response)
    end

    it "should redirect to a url" do
      @c.redirect "/start-page"
      @response['headers']['Location'].should == "/start-page"
      @response['status'].should == 302
      @response['message'].should == 'Moved temporarily'
    end

    it "should redirect to an action" do
      @c.redirect :action => :show, :id => '{12}'
      @response['headers']['Location'].should == "/application/model/{12}/show"
      @response['status'].should == 302
      @response['message'].should == 'Moved temporarily'
    end

    it "should redirect to an action with 301 status code and status message" do
      @c.redirect({:action => :show, :id => '{12}'}, {'status' => 301, 'message' => 'Moved permanently'})
      @response['headers']['Location'].should == "/application/model/{12}/show"
      @response['status'].should == 301
      @response['message'].should == 'Moved permanently'
    end

end #describe "redirect"

class Metadata < SourceAdapter

  def query
    @result ||= {}

    @result['0'] = {'name' => 'John Smith1', 'phone' => '555-1211', 'contact_id' => '1', 'age' => '22', 'year' => '1982'}
    @result['1'] = {'name' => 'John Smith2', 'phone' => '555-1212', 'contact_id' => '2', 'age' => '23', 'year' => '1983'}
    @result['2'] = {'name' => 'John Smith3', 'phone' => '555-1213', 'contact_id' => '3', 'age' => '24', 'year' => '1984'}

    @result
  end

  def sync
    super
  end

  def metadata


    @contact = {:label => 'Contact',
                :name => 'contact',
                :type => 'link_row', # a row which links to another object in a different model
                :value => '{{name}}, {{phone}}', # name and phone of the contact object not the account!
                :id => '{{contact_id}}', # the id field to reference the contact objects
                :model => 'SugarContacts' }

    @name = { :label => 'Account Name',
              :name => :name,
              :value => '{{@account/name}}',
              :type => 'text',
              :div_class => 'show_text' }

    @phone = { :label => 'Phone Number',
               :name => 'phone',
               :value => '{{@account/phone}}', # redundancy here, could just use {{name}} if value is not present
               :type => 'textinput',
               :editable => true,
               :div_class => 'show_number' }

    @create_date = { :label => 'Creation Date',
                     :name => 'date_created',
                     :value => '{{@account/date_created}}',
                     :type => 'text',
                     :editable => false,
                     :default_value => '2010-01-01 00:00:00',
                     :css_class => 'show_date' }

    @do_not_contact = { :label => 'Do Not Contact',
                        :name => 'do_not_contact',
                        :value => '{{@account/do_not_contact}}',
                        :type => 'checkbox',
                        :default_value => false,
                        :div_class => 'checkbox' }

    @size = { :label => 'Account size',
              :name => 'size',
              :value => '{{@account/size}}',
              :type => 'select',
              :allow_multi => false,
              :editable => false,
              :class => 'dropdown',
              :values => ['Small', 'Medium', 'Large', 'Huge'] }

    @address1 = { :label => 'Address Line 1',
                  :name => 'address1',
                  :value => '{{@account/address1}}',
                  :type => 'textinput',
                  :editable => true,
                  :div_class => 'show_text' }

    @address2 = { :label => 'Address Line 2',
                  :name => 'address2',
                  :value => '{{@account/address2}}',
                  :type => 'textinput',
                  :editable => true,
                  :div_class => 'show_text' }

    @address3 = { :label => 'Address Line 3',
                  :name => 'address3',
                  :value => '{{@account/address3}}',
                  :type => 'textinput',
                  :editable => true,
                  :div_class => 'show_text' }


    @panel1 = { :title => 'Main Panel',
                :type => 'panel',
                :children => [@name, @phone, @create_date, @do_not_contact, @size] }

    @panel2 = { :title => 'Address Panel',
                :type => 'panel',
                :children => [@address1, @address2, @address3] }


    @table1 = { :title => 'Contacts SubPanel',
                :type => 'table',
                :repeatable => '{{@contacts}}',
                :children => [@contact] }


    @link = { :text => "A LINK!",
              :url => "http://www.google.com",
              :type => "link"
    }

      @maplink = {:type => 'maplink',
                :text => 'MAP LINK',
                :showmap_url => '/app/Metadata/show_map',
                :map_type => 'hybrid',
                :scroll => 'true',
                :zoom => 'true',
                :next_uri => '/app/Metadata',
                :show_user => 'true',
                :title_location => 'Click Here',
                :subtitle_location => 'to go home',
                :url_location => '/app/',
                :annotations => [{:street_address => "Cupertino, CA 95014", :title => "Cupertino", :subtitle => "zip: 95014", :url => "/app/GeoLocation/show?city=Cupertino"},
                           {:street_address => "Santa Clara, CA 95051", :title => "Santa Clara", :subtitle => "zip: 95051", :url => "/app/GeoLocation/show?city=Santa%20Clara"}]
                }

    @detail_view = { :title => 'Detail View',
                     :type => 'iuipanel',
                     :left_btn_is_back => true,
                     :children => [@panel1, @panel2, @link, @maplink] }

    @ferror = { :label => '{{@errorlabel}}',
              :type => 'text',
              :name => 'name',
              :value => '{{@errors}}'
            }

    @fname = { :label => 'name',
              :type => 'textinput',
              :name => 'name',
              :value => '{{@form/name}}',
              :validation => { :validators => [:required] }
            }

    @fage = { :label => 'age',
              :type => 'textinput',
              :name => 'age',
              :value => '{{@form/age}}',
              :validation => {  :min_value => 13 }
            }

    @fyear = { :label => 'year',
              :type => 'textinput',
              :name => 'year',
              :value => '{{@form/year}}',
              :validation => { :validators => [:required], :max_value => 2010 }
            }
    @fsubmit = { :name => 'submit',
                :value => 'submit',
                :type => 'submit'
              }

    @form = { :name => 'inputform',
              :action => '{{@submiturl}}',
              :type => 'form',
              :children => [@ferror,@fname,@fage,@fyear,@fsubmit]
            }

    @form_view = { :title => 'Form View',
                     :type => 'iuipanel',
                     :left_btn_is_back => true,
                     :children => [@form] }

    {'address' => @detail_view, 'validation' => @form_view}.to_json
  end
end

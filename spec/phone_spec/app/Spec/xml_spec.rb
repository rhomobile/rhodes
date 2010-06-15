require 'rexml/document'

class XmlTest
    def initialize
    end

    def parse_test
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.xml')
        
        file = File.new(file_name)
        doc = REXML::Document.new(file)

        events = REXML::XPath.each( doc, "event" )
        Test_not_equal(events,nil)
        count = 0
        event = nil
        events.each do |e|
            count += 1
            event = e
        end
        Test_equal(count,1)
        Test_not_equal(event,nil)
        
        Test_equal(event.elements['title'].text, "Martini Tasting")
        
		childs = REXML::XPath.each( doc, "//children/child/" )
		ch_count = 0
        childs.each do |ch|
            ch_count += 1
            Test_equal(ch.elements['title'].text, "Momba")            
        end

        Test_equal(ch_count,10)
    end

    class MyStreamListener

        def initialize(parent)
            @parent = parent
        end

        def tag_start name, attrs
            #puts "tag_start: #{name}; #{attrs}"
            @parent.add_event(attrs) if name == 'event'
        end
        def tag_end name
            #puts "tag_end: #{name}"
        end
        def text text
            #puts "text: #{text}"
        end
        def instruction name, instruction
        end
        def comment comment
        end
        def doctype name, pub_sys, long_name, uri
        end
        def doctype_end
        end
        def attlistdecl element_name, attributes, raw_content
        end
        def elementdecl content
        end
        def entitydecl content
        end
        def notationdecl content
        end
        def entity content
        end
        def cdata content
            puts "cdata: #{content}"
        end
        def xmldecl version, encoding, standalone
        end
    end
    
    def add_event(attrs)
        @events<<attrs
    end
    
    def stream_test
    
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.xml')
        file = File.new(file_name)
    
        @events = []
        list = MyStreamListener.new(self)
        REXML::Document.parse_stream(file, list)
    
        Test_equal(@events.size,1)
    end
    
    #def generate_test
        #TODO: generate_test
    #end
    
    def clear
    end
end    
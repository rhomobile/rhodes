require 'mspec/expectations/expectations'
require 'mspec/utils/ruby_name'
require 'mspec/runner/formatters/yaml'

class JUnitFormatter < YamlFormatter
  def initialize(out=nil)
    super
    @tests = []
    @spec_path = ''
  end

  def register
    super
    MSpec.register :load, self
  end

  def load(state = nil)
    spec_path, spec_file = File.split((MSpec.retrieve(:file)+'.rb').gsub(/^spec\//,''))
    if (spec_path.empty? || spec_path.nil?) && spec_file =~ /(.*)_spec/i
      spec_path = $1
    end
    @spec_path = encode_for_xml( spec_path.gsub('/','.') )
  end

  def after(state = nil)
    super
    @tests << {:test => state, :spec => @spec_path, :exception => false} unless exception?
  end

  def exception(exception)
    super
    @tests << {:test => exception, :spec => @spec_path, :exception => true}
  end

  def finish
    switch

    time = @timer.elapsed
    formatted_time = "%.1f" % time
    tests = @tally.counter.examples
    errors = @tally.counter.errors
    failures = @tally.counter.failures
#RHO
	  #printf <<-XML
	print <<-XML
<?xml version="1.0" encoding="UTF-8" ?>
    <testsuites
        testCount="#{tests}"
        errorCount="#{errors}"
        failureCount="#{failures}"
        timeCount="#{formatted_time}" time="#{formatted_time}">
      <testsuite
          tests="#{tests}"
          errors="#{errors}"
          failures="#{failures}"
          time="#{formatted_time}"
          name="Spec Output For #{::RUBY_NAME} (#{::RUBY_VERSION})">

    XML

    @tests.each do |h|
      description = encode_for_xml h[:test].description

      spec_class = h[:spec]

      if description =~ /^([^\s\.\|:]+)(#|\.|::)([^\s]+)/i
        spec_class = $1
      end     

#RHO
#      printf <<-XML, "Spec", description, 0.0
#        <testcase classname="%s" name="%s" time="%f">
#      XML
    print <<-XML
      <testcase classname="#{spec_class}" name="#{description}" time="0.0">
    XML
#RHO
	if h[:exception]
        outcome = h[:test].failure? ? "failure" : "error"
        message = encode_for_xml h[:test].message
        backtrace = encode_for_xml h[:test].backtrace
        print <<-XML
          <#{outcome} message="error in #{description}" type="#{outcome}">
            #{message}
            #{backtrace}
          </#{outcome}>
        XML
      end
      print <<-XML
        </testcase>
      XML
    end

	print <<-XML
      </testsuite>
    </testsuites>
    XML
  end

  private
  LT = "&lt;"
  GT = "&gt;"
  QU = "&quot;"
  AP = "&apos;"
  AM = "&amp;"
#RHO
#  TARGET_ENCODING = "ISO-8859-1"
TARGET_ENCODING = "ASCII-8BIT"
#RHO
  def encode_for_xml(str)
    encode_as_latin1(str).gsub("<", LT).gsub(">", GT).
      gsub('"', QU).gsub("'", AP).gsub("&", AM).
      gsub(/[#{Regexp.escape("\0\1\2\3\4\5\6\7\8")}]/, "?")
  end

  if defined? Encoding
    def encode_as_latin1(str)
      str.encode(TARGET_ENCODING, :undef => :replace, :invalid => :replace)
    end
  else
    require 'iconv'
    def encode_as_latin1(str)
      Iconv.conv("#{TARGET_ENCODING}//TRANSLIT//IGNORE", "UTF-8", str)
    end
  end
end

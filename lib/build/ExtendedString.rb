# Some old code relies on ActiveSupport, but its too heavy to be 
# bundled with rhodes. Instead of that just copy-paste some of used methods

# Partially based on code from ActiveSupport library

# Copyright (c) 2005-2014 David Heinemeier Hansson
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

class Inflections
  @__instance__ = {}

  def self.instance(locale = :en)
    @__instance__[locale] ||= new(locale)
  end

  attr_reader :plurals, :singulars, :uncountables, :humans, :acronyms, :acronym_regex

  def initialize(locale = :en)
    @plurals, @singulars, @uncountables, @humans, @acronyms, @acronym_regex = [], [], [], [], {}, /(?=a)b/

    if locale == :en
      plural(/$/, 's')
      plural(/s$/i, 's')
      plural(/^(ax|test)is$/i, '\1es')
      plural(/(octop|vir)us$/i, '\1i')
      plural(/(octop|vir)i$/i, '\1i')
      plural(/(alias|status)$/i, '\1es')
      plural(/(bu)s$/i, '\1ses')
      plural(/(buffal|tomat)o$/i, '\1oes')
      plural(/([ti])um$/i, '\1a')
      plural(/([ti])a$/i, '\1a')
      plural(/sis$/i, 'ses')
      plural(/(?:([^f])fe|([lr])f)$/i, '\1\2ves')
      plural(/(hive)$/i, '\1s')
      plural(/([^aeiouy]|qu)y$/i, '\1ies')
      plural(/(x|ch|ss|sh)$/i, '\1es')
      plural(/(matr|vert|ind)(?:ix|ex)$/i, '\1ices')
      plural(/^(m|l)ouse$/i, '\1ice')
      plural(/^(m|l)ice$/i, '\1ice')
      plural(/^(ox)$/i, '\1en')
      plural(/^(oxen)$/i, '\1')
      plural(/(quiz)$/i, '\1zes')

      singular(/s$/i, '')
      singular(/(ss)$/i, '\1')
      singular(/(n)ews$/i, '\1ews')
      singular(/([ti])a$/i, '\1um')
      singular(/((a)naly|(b)a|(d)iagno|(p)arenthe|(p)rogno|(s)ynop|(t)he)(sis|ses)$/i, '\1sis')
      singular(/(^analy)(sis|ses)$/i, '\1sis')
      singular(/([^f])ves$/i, '\1fe')
      singular(/(hive)s$/i, '\1')
      singular(/(tive)s$/i, '\1')
      singular(/([lr])ves$/i, '\1f')
      singular(/([^aeiouy]|qu)ies$/i, '\1y')
      singular(/(s)eries$/i, '\1eries')
      singular(/(m)ovies$/i, '\1ovie')
      singular(/(x|ch|ss|sh)es$/i, '\1')
      singular(/^(m|l)ice$/i, '\1ouse')
      singular(/(bus)(es)?$/i, '\1')
      singular(/(o)es$/i, '\1')
      singular(/(shoe)s$/i, '\1')
      singular(/(cris|test)(is|es)$/i, '\1is')
      singular(/^(a)x[ie]s$/i, '\1xis')
      singular(/(octop|vir)(us|i)$/i, '\1us')
      singular(/(alias|status)(es)?$/i, '\1')
      singular(/^(ox)en/i, '\1')
      singular(/(vert|ind)ices$/i, '\1ex')
      singular(/(matr)ices$/i, '\1ix')
      singular(/(quiz)zes$/i, '\1')
      singular(/(database)s$/i, '\1')

      irregular('person', 'people')
      irregular('man', 'men')
      irregular('child', 'children')
      irregular('sex', 'sexes')
      irregular('move', 'moves')
      irregular('zombie', 'zombies')

      uncountable(%w(equipment information rice money species series fish sheep jeans police))
    end
  end

  # Specifies a new acronym. An acronym must be specified as it will appear
  # in a camelized string. An underscore string that contains the acronym
  # will retain the acronym when passed to +camelize+, +humanize+, or
  # +titleize+. A camelized string that contains the acronym will maintain
  # the acronym when titleized or humanized, and will convert the acronym
  # into a non-delimited single lowercase word when passed to +underscore+.
  def acronym(word)
    @acronyms[word.downcase] = word
    @acronym_regex = /#{@acronyms.values.join("|")}/
  end

  # Specifies a new pluralization rule and its replacement. The rule can
  # either be a string or a regular expression. The replacement should
  # always be a string that may include references to the matched data from
  # the rule.
  def plural(rule, replacement)
    @uncountables.delete(rule) if rule.is_a?(String)
    @uncountables.delete(replacement)
    @plurals.unshift([rule, replacement])
  end

  # Specifies a new singularization rule and its replacement. The rule can
  # either be a string or a regular expression. The replacement should
  # always be a string that may include references to the matched data from
  # the rule.
  def singular(rule, replacement)
    @uncountables.delete(rule) if rule.is_a?(String)
    @uncountables.delete(replacement)
    @singulars.unshift([rule, replacement])
  end

  # Specifies a new irregular that applies to both pluralization and
  # singularization at the same time. This can only be used for strings, not
  # regular expressions. You simply pass the irregular in singular and
  # plural form.
  def irregular(singular, plural)
    @uncountables.delete(singular)
    @uncountables.delete(plural)

    s0 = singular[0]
    srest = singular[1..-1]

    p0 = plural[0]
    prest = plural[1..-1]

    if s0.upcase == p0.upcase
      plural(/(#{s0})#{srest}$/i, '\1' + prest)
      plural(/(#{p0})#{prest}$/i, '\1' + prest)

      singular(/(#{s0})#{srest}$/i, '\1' + srest)
      singular(/(#{p0})#{prest}$/i, '\1' + srest)
    else
      plural(/#{s0.upcase}(?i)#{srest}$/,   p0.upcase   + prest)
      plural(/#{s0.downcase}(?i)#{srest}$/, p0.downcase + prest)
      plural(/#{p0.upcase}(?i)#{prest}$/,   p0.upcase   + prest)
      plural(/#{p0.downcase}(?i)#{prest}$/, p0.downcase + prest)

      singular(/#{s0.upcase}(?i)#{srest}$/,   s0.upcase   + srest)
      singular(/#{s0.downcase}(?i)#{srest}$/, s0.downcase + srest)
      singular(/#{p0.upcase}(?i)#{prest}$/,   s0.upcase   + srest)
      singular(/#{p0.downcase}(?i)#{prest}$/, s0.downcase + srest)
    end
  end

  # Add uncountable words that shouldn't be attempted inflected.
  def uncountable(*words)
    @uncountables += words.flatten.map(&:downcase)
  end

  # Specifies a humanized form of a string by a regular expression rule or
  # by a string mapping. When using a regular expression based replacement,
  # the normal humanize formatting is called after the replacement. When a
  # string is used, the human form should be specified as desired (example:
  # 'The name', not 'the_name').
  def human(rule, replacement)
    @humans.unshift([rule, replacement])
  end

  # Clears the loaded inflections within a given scope (default is
  # <tt>:all</tt>). Give the scope as a symbol of the inflection type, the
  # options are: <tt>:plurals</tt>, <tt>:singulars</tt>, <tt>:uncountables</tt>,
  # <tt>:humans</tt>.
  def clear(scope = :all)
    case scope
      when :all
        @plurals, @singulars, @uncountables, @humans = [], [], [], []
      else
        instance_variable_set "@#{scope}", []
    end
  end

  ## methods

  class << self

    # Yields a singleton instance of Inflections so you can specify
    # additional Inflections rules. If passed an optional locale, rules for other
    # languages can be specified. If not specified, defaults to <tt>:en</tt>.
    # Only rules for English are provided.
    def inflections(locale = :en)
      if block_given?
        yield self.instance(locale)
      else
        self.instance(locale)
      end
    end

    # Returns the plural form of the word in the string.
    #
    # If passed an optional +locale+ parameter, the word will be
    # pluralized using rules defined for that language. By default,
    # this parameter is set to <tt>:en</tt>.
    def pluralize(word, locale = :en)
      apply_inflections(word, inflections(locale).plurals)
    end

    # The reverse of +pluralize+, returns the singular form of a word in a
    # string.
    #
    # If passed an optional +locale+ parameter, the word will be
    # singularized using rules defined for that language. By default,
    # this parameter is set to <tt>:en</tt>.

    def singularize(word, locale = :en)
      apply_inflections(word, inflections(locale).singulars)
    end

    # By default, +camelize+ converts strings to UpperCamelCase. If the argument
    # to +camelize+ is set to <tt>:lower</tt> then +camelize+ produces
    # lowerCamelCase.
    #
    # +camelize+ will also convert '/' to '::' which is useful for converting
    # paths to namespaces.

    def camelize(term, uppercase_first_letter = true)
      string = term.to_s
      if uppercase_first_letter
        string = string.sub(/^[a-z\d]*/) { inflections.acronyms[$&] || $&.capitalize }
      else
        string = string.sub(/^(?:#{inflections.acronym_regex}(?=\b|[A-Z_])|\w)/) { $&.downcase }
      end
      string.gsub!(/(?:_|(\/))([a-z\d]*)/i) { "#{$1}#{inflections.acronyms[$2] || $2.capitalize}" }
      string.gsub!('/', '::')
      string
    end

    # Makes an underscored, lowercase form from the expression in the string.
    #
    # Changes '::' to '/' to convert namespaces to paths.
    #
    #   'ActiveModel'.underscore         # => "active_model"
    #   'ActiveModel::Errors'.underscore # => "active_model/errors"
    #
    # As a rule of thumb you can think of +underscore+ as the inverse of
    # +camelize+, though there are cases where that does not hold:
    #
    #   'SSLError'.underscore.camelize # => "SslError"
    def underscore(camel_cased_word)
      return camel_cased_word unless camel_cased_word =~ /[A-Z-]|::/
      word = camel_cased_word.to_s.gsub('::', '/')
      word.gsub!(/(?:([A-Za-z\d])|^)(#{inflections.acronym_regex})(?=\b|[^a-z])/) { "#{$1}#{$1 && '_'}#{$2.downcase}" }
      word.gsub!(/([A-Z\d]+)([A-Z][a-z])/,'\1_\2')
      word.gsub!(/([a-z\d])([A-Z])/,'\1_\2')
      word.tr!("-", "_")
      word.downcase!
      word
    end

    # Tweaks an attribute name for display to end users.
    #
    # Specifically, +humanize+ performs these transformations:
    #
    #   * Applies human inflection rules to the argument.
    #   * Deletes leading underscores, if any.
    #   * Removes a "_id" suffix if present.
    #   * Replaces underscores with spaces, if any.
    #   * Downcases all words except acronyms.
    #   * Capitalizes the first word.
    #
    # The capitalization of the first word can be turned off by setting the
    # +:capitalize+ option to false (default is true).
    def humanize(lower_case_and_underscored_word, options = {})
      result = lower_case_and_underscored_word.to_s.dup

      inflections.humans.each { |(rule, replacement)| break if result.sub!(rule, replacement) }

      result.sub!(/\A_+/, '')
      result.sub!(/_id\z/, '')
      result.tr!('_', ' ')

      result.gsub!(/([a-z\d]*)/i) do |match|
        "#{inflections.acronyms[match] || match.downcase}"
      end

      if options.fetch(:capitalize, true)
        result.sub!(/\A\w/) { |match| match.upcase }
      end

      result
    end

    # Capitalizes all the words and replaces some characters in the string to
    # create a nicer looking title. +titleize+ is meant for creating pretty
    # output. It is not used in the Rails internals.
    #
    # +titleize+ is also aliased as +titlecase+.
    def titleize(word)
      humanize(underscore(word)).gsub(/\b(?<!['`])[a-z]/) { $&.capitalize }
    end

    # Create the name of a table like Rails does for models to table names. This
    # method uses the +pluralize+ method on the last word in the string.
    def tableize(class_name)
      pluralize(underscore(class_name))
    end


    # Create a class name from a plural table name like Rails does for table
    # names to models. Note that this returns a string and not a Class (To
    # convert to an actual class follow +classify+ with +constantize+).
    def classify(table_name)
      # strip out any leading schema name
      camelize(singularize(table_name.to_s.sub(/.*\./, '')))
    end

    # Replaces underscores with dashes in the string.
    def dasherize(underscored_word)
      underscored_word.tr('_', '-')
    end

    # Removes the module part from the expression in the string.
    def demodulize(path)
      path = path.to_s
      if i = path.rindex('::')
        path[(i+2)..-1]
      else
        path
      end
    end

    # Removes the rightmost segment from the constant expression in the string.
    # See also +demodulize+.
    def deconstantize(path)
      path.to_s[0, path.rindex('::') || 0] # implementation based on the one in facets' Module#spacename
    end

    # Creates a foreign key name from a class name.
    # +separate_class_name_and_id_with_underscore+ sets whether
    # the method should put '_' between the name and 'id'.
    def foreign_key(class_name, separate_class_name_and_id_with_underscore = true)
      underscore(demodulize(class_name)) + (separate_class_name_and_id_with_underscore ? "_id" : "id")
    end

    # Tries to find a constant with the name specified in the argument string.
    def constantize(camel_cased_word)
      names = camel_cased_word.split('::')

      # Trigger a built-in NameError exception including the ill-formed constant in the message.
      Object.const_get(camel_cased_word) if names.empty?

      # Remove the first blank element in case of '::ClassName' notation.
      names.shift if names.size > 1 && names.first.empty?

      names.inject(Object) do |constant, name|
        if constant == Object
          constant.const_get(name)
        else
          candidate = constant.const_get(name)
          next candidate if constant.const_defined?(name, false)
          next candidate unless Object.const_defined?(name)

          # Go down the ancestors to check if it is owned directly. The check
          # stops when we reach Object or the end of ancestors tree.
          constant = constant.ancestors.inject do |const, ancestor|
            break const    if ancestor == Object
            break ancestor if ancestor.const_defined?(name, false)
            const
          end

          # owner is in Object, so raise
          constant.const_get(name, false)
        end
      end
    end

    # Tries to find a constant with the name specified in the argument string.
    def safe_constantize(camel_cased_word)
      constantize(camel_cased_word)
    rescue NameError => e
      raise unless e.message =~ /(uninitialized constant|wrong constant name) #{const_regexp(camel_cased_word)}$/ ||
        e.name.to_s == camel_cased_word.to_s
    rescue ArgumentError => e
      raise unless e.message =~ /not missing constant #{const_regexp(camel_cased_word)}\!$/
    end


    # Returns the suffix that should be added to a number to denote the position
    # in an ordered sequence such as 1st, 2nd, 3rd, 4th.
    def ordinal(number)
      abs_number = number.to_i.abs

      if (11..13).include?(abs_number % 100)
        "th"
      else
        case abs_number % 10
          when 1; "st"
          when 2; "nd"
          when 3; "rd"
          else    "th"
        end
      end
    end

    # Turns a number into an ordinal string used to denote the position in an
    # ordered sequence such as 1st, 2nd, 3rd, 4th.
    def ordinalize(number)
      "#{number}#{ordinal(number)}"
    end
    
  private

    # Mounts a regular expression, returned as a string to ease interpolation,
    # that will match part by part the given constant.
    def const_regexp(camel_cased_word) #:nodoc:
      parts = camel_cased_word.split("::")

      return Regexp.escape(camel_cased_word) if parts.blank?

      last  = parts.pop

      parts.reverse.inject(last) do |acc, part|
        part.empty? ? acc : "#{part}(::#{acc})?"
      end
    end

    # Applies inflection rules for +singularize+ and +pluralize+.
    def apply_inflections(word, rules)
      result = word.to_s.dup

      if word.empty? || inflections.uncountables.include?(result.downcase[/\b\w+\Z/])
        result
      else
        rules.each { |(rule, replacement)| break if result.sub!(rule, replacement) }
        result
      end
    end
  end
end

class String

  # Returns true if string contains numeric value
  def numeric?
    Float(self) != nil rescue false
  end

  # Returns true if string contains integer value
  def int?
    Integer(self) != nil rescue false
  end

  # Returns the plural form of the word in the string.
  #
  # If the optional parameter +count+ is specified,
  # the singular form will be returned if <tt>count == 1</tt>.
  # For any other value of +count+ the plural will be returned.
  def pluralize(count = nil, locale = :en)
    locale = count if count.is_a?(Symbol)
    if count == 1
      self.dup
    else
      Inflections.pluralize(self, locale)
    end
  end

  # The reverse of +pluralize+, returns the singular form of a word in a string.
  #
  # If the optional parameter +locale+ is specified,
  # the word will be singularized as a word of that language.
  # By default, this parameter is set to <tt>:en</tt>.
  # You must define your own inflection rules for languages other than English.
  def singularize(locale = :en)
    Inflections.singularize(self, locale)
  end


  # +constantize+ tries to find a declared constant with the name specified
  # in the string. It raises a NameError when the name is not in CamelCase
  # or is not initialized.  
  def constantize
    Inflections.constantize(self)
  end

  # +safe_constantize+ tries to find a declared constant with the name specified
  # in the string. It returns nil when the name is not in CamelCase
  # or is not initialized.  
  def safe_constantize
    Inflections.safe_constantize(self)
  end

  # By default, +camelize+ converts strings to UpperCamelCase. If the argument to camelize
  # is set to <tt>:lower</tt> then camelize produces lowerCamelCase.
  def camelize(first_letter = :upper)
    case first_letter
    when :upper
      Inflections.camelize(self, true)
    when :lower
      Inflections.camelize(self, false)
    end
  end
  alias_method :camelcase, :camelize

  # Capitalizes all the words and replaces some characters in the string to create
  # a nicer looking title. +titleize+ is meant for creating pretty output. 
  def titleize
    Inflections.titleize(self)
  end
  alias_method :titlecase, :titleize

  # The reverse of +camelize+. Makes an underscored, lowercase form from the expression in the string.
  #
  # +underscore+ will also change '::' to '/' to convert namespaces to paths.
  def underscore
    Inflections.underscore(self)
  end

  # Replaces underscores with dashes in the string.
  def dasherize
    Inflections.dasherize(self)
  end

  # Removes the module part from the constant expression in the string.
  def demodulize
    Inflections.demodulize(self)
  end

  # Removes the rightmost segment from the constant expression in the string.
  # See also +demodulize+.
  def deconstantize
    Inflections.deconstantize(self)
  end

  # Creates the name of a table like Rails does for models to table names. This method
  # uses the +pluralize+ method on the last word in the string.
  def tableize
    Inflections.tableize(self)
  end

  # Create a class name from a plural table name like Rails does for table names to models.
  # Note that this returns a string and not a class. (To convert to an actual class
  # follow +classify+ with +constantize+.)
  def classify
    Inflections.classify(self)
  end

  # Capitalizes the first word, turns underscores into spaces, and strips a
  # trailing '_id' if present.
  # Like +titleize+, this is meant for creating pretty output.
  #
  # The capitalization of the first word can be turned off by setting the
  # optional parameter +capitalize+ to false.
  # By default, this parameter is true.
  def humanize(options = {})
    Inflections.humanize(self, options)
  end

  # Creates a foreign key name from a class name.
  # +separate_class_name_and_id_with_underscore+ sets whether
  # the method should put '_' between the name and 'id'.
  def foreign_key(separate_class_name_and_id_with_underscore = true)
    Inflections.foreign_key(self, separate_class_name_and_id_with_underscore)
  end

  def capitalize_first
    str = to_s
    empty? ? str : str[0..0].upcase << str[1..-1]
  end

  # Converts a string to camelcase. This method leaves the first character
  # as given. This allows other methods to be used first, such as #uppercase
  # and #lowercase.
  def camel_case(*separators)
    case separators.first
      when Symbol, TrueClass, FalseClass, NilClass
        first_letter = separators.shift
    end

    separators = ['_', '\s'] if separators.empty?

    str = self.dup

    separators.each do |s|
      str = str.gsub(/(?:#{s}+)([a-z])/){ $1.upcase }
    end

    case first_letter
      when :upper, true
        str = str.gsub(/(\A|\s)([a-z])/){ $1 + $2.upcase }
      when :lower, false
        str = str.gsub(/(\A|\s)([A-Z])/){ $1 + $2.downcase }
    end

    str
  end

  # Converts a string to module name representation.
  #
  # This is essentially #camelcase, but it also converts
  # '/' to '::' which is useful for converting paths to
  # namespaces.
  def modulize
    #gsub('__','/'). # why was this ever here?
    gsub(/__(.?)/){ "::#{$1.upcase}" }.
        gsub(/\/(.?)/){ "::#{$1.upcase}" }.
        gsub(/(?:_+|-+)([a-z])/){ $1.upcase }.
        gsub(/(\A|\s)([a-z])/){ $1 + $2.upcase }
  end

  # # Underscore a string such that camelcase, dashes and spaces are
  # # replaced by underscores. This is the reverse of {#camelcase},
  # # albeit not an exact inverse.
  # def underscore
  #   #gsub(/::/, '/').
  #   gsub(/([A-Z]+)([A-Z][a-z])/,'\1_\2').
  #       gsub(/([a-z\d])([A-Z])/,'\1_\2').
  #       tr('-', '_').
  #       gsub(/\s/, '_').
  #       gsub(/__+/, '_').
  #       downcase
  # end

  # def titleize
  #   underscore.humanize.gsub(/\b('?[a-z])/) { $1.capitalize }
  # end

  def hard_wrap(width = 80)
    gsub(/(.{1,#{width}})(\s+|$)/, "\\1\n").strip
  end

  # from natcmp gem
  # Natural comaration based on numbers in strings
  def self.natcmp(str1, str2, ignoreCase=true)
    strArrays = [str1, str2].collect do |str|
      str = str.downcase if ignoreCase
      str.tr(" \t\r\n", '').split(/(\d+)/)
    end

    minSize = strArrays.min_by { |arr| arr.size }.size

    1.step(minSize-1, 2) do |i|
      unless strArrays.any? { |arr| arr[i] =~ /^0/ }
        strArrays.each { |arr| arr[i] = arr[i].to_i }
      end
    end

    strArrays[0] <=> strArrays[1]
  end

  def primary
    self.blue
  end

  def success
    self.green
  end

  def info
    self.cyan
  end

  def warning
    self.magenta
  end

  def alarm
    self.red
  end

  def black; self end
  def red; self end
  def green; self end
  def brown; self end
  def blue; self end
  def magenta; self end
  def cyan; self end
  def gray; self end
  def bg_black; self end
  def bg_red; self end
  def bg_green; self end
  def bg_brown; self end
  def bg_blue; self end
  def bg_magenta; self end
  def bg_cyan; self end
  def bg_gray; self end
  def bold; self end
  def underline; self end
  def reverse_color; self end

  # table of helper functions
  {  
    :black           => ["\033[30m","\033[0m"],
    :red             => ["\033[31m","\033[0m"],
    :green           => ["\033[32m","\033[0m"],
    :brown           => ["\033[33m","\033[0m"],
    :blue            => ["\033[34m","\033[0m"],
    :magenta         => ["\033[35m","\033[0m"],
    :cyan            => ["\033[36m","\033[0m"],
    :gray            => ["\033[37m","\033[0m"],
    :bg_black        => ["\033[40m","\0330m"],
    :bg_red          => ["\033[41m","\033[0m"],
    :bg_green        => ["\033[42m","\033[0m"],
    :bg_brown        => ["\033[43m","\033[0m"],
    :bg_blue         => ["\033[44m","\033[0m"],
    :bg_magenta      => ["\033[45m","\033[0m"],
    :bg_cyan         => ["\033[46m","\033[0m"],
    :bg_gray         => ["\033[47m","\033[0m"],
    :bold            => ["\033[1m","\033[22m"],
    :underline       => ["\033[4m","\033[24m"],
    :reverse_color   => ["\033[7m","\033[27m"]
  }.each do |name, format|
    # use colorizer only for TTY for non windows systems
    is_win = OS.windows?
    if STDOUT.tty? && !is_win
      send :define_method, name do
        [format[0], self, format[1]].join
      end
    end
  end
end

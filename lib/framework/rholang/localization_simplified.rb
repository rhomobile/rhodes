# LocalizationSimplified (L10n-simplified)
# Really simple localization for Rails
# By Jesper Rønn-Jensen ( http://justaddwater.dk/ )
# Plugin available at http://rubyforge.org/projects/l10n-simplified/
#

module LocalizationSimplified
  @@ignore = "\xFF\xFF\xFF\xFF" # %% == Literal "%" character
  @@cur_locale = 'en'
  # substitute all daynames and monthnames with localized names
  # from RUtils plugin
  def self.localize_strftime(date='%d.%m.%Y', time='')
    date.gsub!('%%', @@ignore)
    date.gsub!('%a', LocalizationSimplified::DateHelper::AbbrDaynames[time.wday])
    date.gsub!('%A', LocalizationSimplified::DateHelper::Daynames[time.wday])
    date.gsub!('%b', LocalizationSimplified::DateHelper::AbbrMonthnames[time.mon])
    date.gsub!('%B', LocalizationSimplified::DateHelper::Monthnames[time.mon])
    date.gsub!(@@ignore, '%%')
  end
  
  def self.get_cur_locale
    @@cur_locale  
  end
  
  def self.requre_loc(file,check_exist)
      curLocale = System::get_locale()
      puts 'Current locale: ' + curLocale.to_s
      @@cur_locale = curLocale
      
      if check_exist
        if Rho::file_exist?(file + curLocale + '.iseq')
            require file + curLocale
        else    
            puts 'Could not find resources for locale: ' + curLocale.to_s
            if curLocale != 'en' && Rho::file_exist?(file + 'en.iseq')
                puts 'Load english resources.'
                require file + 'en'
            end    
        end
      else
        begin
            require file + curLocale
        rescue Exception => e
            puts 'Could not load locale: ' + curLocale.to_s + '. Load english.'
            require file + 'en' unless curLocale == 'en'
        end
      end
  end
end

LocalizationSimplified.requre_loc('rholang/lang_',false)

class Hash
    def reverse_merge(other_hash)
        other_hash.merge(self)
    end
    
    def reverse_merge!(other_hash)
        replace(reverse_merge(other_hash))
    end

    def stringify_keys
      options = {}
      
      each_pair do |key, value|
        options[key.to_s] = value
      end
      
      options
    end    
end #hash

module Rho
# Give default settings to number_to_currency()
module NumberHelper
  #alias_method :orig_number_to_currency, :number_to_currency
  #modify number_to_currency to accept :order option
  
  def NumberHelper.number_to_currency(number, options = {})
    # Blend default options with localized currency options
    options.reverse_merge!(LocalizationSimplified::NumberHelper::CurrencyOptions)
    options[:order] ||= [:unit, :number]
    
    options = options.stringify_keys
    precision, unit, separator, delimiter = options.delete("precision") { 2 }, options.delete("unit") { "$" }, options.delete("separator") { "." }, options.delete("delimiter") { "," }
    separator = "" unless precision > 0

    #add leading space before trailing unit
    unit = " " + unit if options["order"] == [:number, :unit]
    output = ''
    
    opt2 = {:precision=>precision, :separator=>separator, :delimiter=>delimiter}
    begin
      options["order"].each do |param|
        case param
          when :unit
            output << unit
          when :number
            parts = number_with_precision(number, opt2 ).split(separator)
            output << number_with_delimiter(parts[0], opt2 ) + separator + parts[1].to_s
        end
      end
    rescue
      output = number
    end
    output
  end
  
    def NumberHelper.number_with_precision(number, options)
      precision = (options[:precision])
      separator = (options[:separator])
      delimiter = (options[:delimiter])

      begin
        rounded_number = (Float(number) * (10 ** precision)).round.to_f / 10 ** precision
        number_with_delimiter("%01.#{precision}f" % rounded_number,
          :separator => separator,
          :delimiter => delimiter)
      rescue
        number
      end
    end

    def NumberHelper.number_with_delimiter(number, options)
      delimiter = (options[:delimiter])
      separator = (options[:separator])

      begin
        parts = number.to_s.split('.')
        parts[0].gsub!(/(\d)(?=(\d\d\d)+(?!\d))/, "\\1#{delimiter}")
        parts.join(separator)
      rescue
        number
      end
    end  

end# module NumberHelper
end# rho
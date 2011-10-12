# LocalizationSimplified (L10n-simplified)
# Really simple localization for Rails
# By Jesper Rønn-Jensen ( http://justaddwater.dk/ )
# Plugin available at http://rubyforge.org/projects/l10n-simplified/
#

module LocalizationSimplified
  @@ignore = "\xFF\xFF\xFF\xFF" # %% == Literal "%" character
  @@cur_locale = nil #'en'
  @@cur_country = nil #'en'
  # substitute all daynames and monthnames with localized names
  # from RUtils plugin
  def self.localize_strftime(date='%d.%m.%Y', time='')
    #date.gsub!('%%', @@ignore)
    date.gsub!('%a', LocalizationSimplified::DateHelper::AbbrDaynames[time.wday])
    date.gsub!('%A', LocalizationSimplified::DateHelper::Daynames[time.wday])
    date.gsub!('%b', LocalizationSimplified::DateHelper::AbbrMonthnames[time.mon])
    date.gsub!('%B', LocalizationSimplified::DateHelper::Monthnames[time.mon])
    #date.gsub!(@@ignore, '%%')
  end
  
  def self.get_cur_locale
    @@cur_locale  
  end
  
  def self.set_cur_locale(loc, country)
    @@cur_locale = loc
    @@cur_country = country  
  end
  
    def self.init_current_locale
        return if @@cur_locale
        
        @@cur_locale = System::get_locale()
        @@cur_locale = @@cur_locale.downcase if @@cur_locale
        @@cur_locale = 'en' unless @@cur_locale
        @@cur_country = System::get_property("country")
        @@cur_country = @@cur_country.downcase if @@cur_country
        
        puts "Current locale: #{@@cur_locale}; Country code: #{@@cur_country}"
                
    end
    
    def self.requre_loc(file,check_exist)
        init_current_locale()

        unless check_exist
if defined?( RHODES_EMULATOR )        
            file = File.join( __rhoGetRhodesDir(), 'lib/framework', file)
else
            file = File.join( __rhoGetCurrentDir(), 'lib', file)            
end            
            puts "file: #{file}"
        end
      
      puts "Current locale: #{@@cur_locale}; Country code: #{@@cur_country}"
      
        if @@cur_country && @@cur_country.length() > 0 && Rho::file_exist?(file + @@cur_locale + '_' + @@cur_country + RHO_RB_EXT) 
            require file + @@cur_locale + '_' + @@cur_country
        elsif Rho::file_exist?(file + @@cur_locale + RHO_RB_EXT)
            require file + @@cur_locale
        else    
            puts 'Could not find resources for locale: ' + @@cur_locale.to_s + ";file: #{file}" if @@cur_locale != 'en'
            if @@cur_locale != 'en' && Rho::file_exist?(file + 'en' + RHO_RB_EXT)
                puts 'Load english resources.'
                require file + 'en'
            end    
        end
    end
  
end

#LocalizationSimplified.requre_loc('rholang/lang_',false)

module System
    def self.set_locale(locale_code, country_code = nil)
        LocalizationSimplified::set_cur_locale(locale_code, country_code)
        
        LocalizationSimplified.requre_loc('rholang/lang_',false)
        LocalizationSimplified.requre_loc(Rho::RhoFSConnector::get_app_path('app') + 'lang/lang_',true)
        LocalizationSimplified.requre_loc('rholang/rhoerror_',false)
        LocalizationSimplified.requre_loc('rholang/rhomsg_',false)    
    end
end

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
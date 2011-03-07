# lang_chef.rb
# Swedish Chef language file for Ruby on Rails
# Translation by Jesper Rønn-Jensen ( http://justaddwater.dk/ ), via web based translator 


module LocalizationSimplified
  About = {
    :lang => "chef",
    :updated => "2006-09-07"
  }

  class ActiveRecord
    # ErrorMessages to override default messages in 
    # +ActiveRecord::Errors::@@default_error_messages+
    # This plugin also replaces hardcoded 3 text messages 
    # :error_translation is inflected using the Rails 
    # inflector. 
    #
    # Remember to modify the Inflector with your localized translation
    # of "error" and "errors" in the bottom of this file
    # 
    ErrorMessages = {
      :inclusion           => "is nut inclooded in zee leest",
      :exclusion           => "is reserfed",
      :invalid             => "is infeleed",
      :confirmation        => "duesn't metch cunffurmeshun",
      :accepted            => "moost be-a eccepted",
      :empty               => "cun't be-a impty",
      :blank               => "ees reeequired",# alternate formulation: "is required"
      :too_long            => "is tuu lung (mexeemoom is %d cherecters)",
      :too_short           => "is tuu shurt (meenimoom is %d cherecters)",
      :wrong_length        => "is zee vrung lengt (shuoold be-a %d cherecters)",
      :taken               => "hes elreedy beee tekee",
      :not_a_number        => "is nut a noomber",
      #Jespers additions:
      :error_translation   => "irrur",
      :error_header        => "%s pruheebited thees %s frum beeeng sefed. Børk! Børk! Børk!",
      :error_subheader     => "Zeere-a vere-a prublems veet zee fullooeeng feeelds:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "less thun %d secunds",
      :half_a_minute       => "helff a meenoote-a",
      :less_than_a_minute  => "less thun a meenoote-a",
      :one_minute          => "one-a meenoote-a",
      :x_minutes           => "%d meenootes",
      :one_hour            => "ebuoot one-a huoor",
      :x_hours             => "ebuoot %d huoors",
      :one_day             => "one-a dey",
      :x_days              => "%d deys",
      :one_month           => "one-a munt",
      :x_months            => "%d munts",
      :one_year            => "one-a yeer",
      :x_years             => "%d yeers"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Junooery Febrooery Merch Epreel Mey Joone-a Jooly Oogoost Seeptembooor Ooctuber Nufember Deezember}
    AbbrMonthnames = [nil] + %w{Jun Feb Mer Epr Mey Joon Jool Oog Sep Ooct Nuf Deez}
    Daynames       = %w{Soondey Mundey Tooesdey Vednesdey Thoorsdey Freedey Setoordey}
    AbbrDaynames   = %w{Soon Mun Tooe-a Ved Thoo Free Set}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%Y-%m-%d",
      :short    => "%b %e",
      :long     => "%B %e, %Y"
    }

    TimeFormats = {
      :default  => "%a, %d %b %Y %H:%M:%S %z",
      :short    => "%d %b %H:%M",
      :long     => "%B %d, %Y %H:%M"
    }
    # Set the order of +date_select+ and +datetime_select+ boxes
    # Note that at present, the current Rails version only supports ordering of date_select boxes
    DateSelectOrder = {
      :order    => [:year, :month, :day] #default Rails is US ordered: :order => [:year, :month, :day]
    }
  end

  class NumberHelper
    # CurrencyOptions are used as default for +Number#to_currency()+
    # http://api.rubyonrails.org/classes/ActionView/Helpers/NumberHelper.html#M000449
    CurrencyOptions = {
      :unit      => "$",
      :separator => ".",             #unit separator (between integer part and fraction part)
      :delimiter => ",",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'eend',
      :skip_last_comma => false
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
# Inflector.inflections do |inflect|
#   inflect.plural /^(ox)$/i, '\1en'
#   inflect.singular /^(ox)en/i, '\1'
#   inflect.irregular 'person people'
#   inflect.uncountable %w( information )
# end

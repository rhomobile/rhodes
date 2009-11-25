# lang_fi.rb
# Finnish translation file. 
# Translation by Jarkko Laine ( http://jlaine.net/ )


module LocalizationSimplified
  About = {
    :lang => "fi",
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
      :inclusion           => "ei löydy listalta",
      :exclusion           => "on varattu",
      :invalid             => "on virheellinen",
      :confirmation        => "ei vastaa vahvistusta",
      :accepted            => "on hyväksyttävä",
      :empty               => "ei voi olla tyhjä",
      :blank               => "ei voi olla tyhjä",# alternate formulation: "is required"
      :too_long            => "on liian pitkä (maksimi on %d merkkiä)",
      :too_short           => "on liian lyhyt (minimi on %d merkkiä)",
      :wrong_length        => "on väärän pituinen (oikea pituus %d merkkiä)",
      :taken               => "on jo varattu",
      :not_a_number        => "ei ole numero",
      #Jespers additions:
      :error_translation   => "virhe",
      :error_header        => "%s esti tämän %s tallentamisen",
      :error_subheader     => "Seuraavat kentät aiheuttivat ongelmia:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "alle %d sekuntia",
      :half_a_minute       => "puoli minuuttia",
      :less_than_a_minute  => "alle minuutti",
      :one_minute          => "1 minuutti",
      :x_minutes           => "%d minuuttia",
      :one_hour            => "noin tunti",
      :x_hours             => "noin %d tuntia",
      :one_day             => "1 päivä",
      :x_days              => "%d päivää",
      :one_month           => "1 month",
      :x_months            => "%d months",
      :one_year            => "1 year",
      :x_years             => "%d years"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{tammikuu helmikuu maaliskuu huhtikuu toukokuu kesäkuu heinäkuu elokuu syyskuu lokakuu marraskuu joulukuu}
    AbbrMonthnames = [nil] + %w{tammi helmi maalis huhti touko kesä heinä elo syys loka marras joulu}
    Daynames       = %w{sunnuntai maanantai tiistai keskiviikko torstai perjantai lauantai}
    AbbrDaynames   = %w{su ma ti ke to pe la}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )    
    DateFormats = {
      :default  => "%e.%m.%Y",
      :short    => "%d.%m.",
      :long     => "%e. %Bta %Y"
    }

    TimeFormats = {
      :default  => "%a %Bn %e. %H:%M:%S %Z %Y",
      :short    => "%d.%m.%Y %H:%M",
      :long     => "%a %e. %Bta %Y %T"
    }
    # Set the order of +date_select+ and +datetime_select+ boxes
    # Note that at present, the current Rails version only supports ordering of date_select boxes
    DateSelectOrder = {
      :order    => [:day, :month, :year] #default Rails is US ordered: :order => [:year, :month, :day]
    }
  end

  class NumberHelper
    # CurrencyOptions are used as default for +Number#to_currency()+
    # http://api.rubyonrails.org/classes/ActionView/Helpers/NumberHelper.html#M000449
    CurrencyOptions = {
      :unit      => "€",
      :separator => " ",             #unit separator (between integer part and fraction part)
      :delimiter => ",",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'ja',
      :skip_last_comma => true
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

# lang_sr.rb
# Serbian translation / Srpski prevod
# author: Slobodan Kovačević (basti #at# arraystudio.com)



module LocalizationSimplified
  About = {
    :lang => "sr",
    :updated => "2007-04-26"
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
      :inclusion           => "nije uključen u listu",
      :exclusion           => "je rezervisan",
      :invalid             => "je neispravan",
      :confirmation        => "nije isti kao potvrda", #nesrećan prevod, treba da bude "doesn't match confirmation"
      :accepted            => "mora biti prihvaćen",
      :empty               => "ne može biti prazan",
      :blank               => "je obavezan",# alternate formulation: "is required"
      :too_long            => "je previše dug (maksimum je %d znakova)",
      :too_short           => "je suviše kratak (minimum je %d znakova)",
      :wrong_length        => "je pogrešne dužine (trebalo bi da bude %d znakova)",
      :taken               => "je već zauzet",
      :not_a_number        => "nije broj",
      #Jespers additions:
      :error_translation   => "greška",
      :error_header        => "%s je sprečila sačuvavanje", # original: "%s prohibited this %s from being saved"
      :error_subheader     => "Greške su se pojavile kod sledećih polja:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
    #NAPOMENA: postoji problem sa svim vremenskim odrednicama zbog nepravilnosti, na primer:
    # 1 godina, 2 godine, ..., ali 5 godina, 6 godina
    # 1 sat, 2 sata, ..., ali 5 sati, 6 sati
      :less_than_x_seconds => "manje od %d sekundi",
      :half_a_minute       => "pola minute",
      :less_than_a_minute  => "manje od minuta",
      :one_minute          => "1 minut",
      :x_minutes           => "%d minuta",
      :one_hour            => "oko 1 sat",
      :x_hours             => "oko %d sati",
      :one_day             => "1 dan",
      :x_days              => "%d dana",
      :one_month           => "1 mesec",
      :x_months            => "%d meseci",
      :one_year            => "1 godina",
      :x_years             => "%d godina"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Januar Februar Mart April Maj Jun Jul Avgust Septembar Oktobar Novembar Decembar}
    AbbrMonthnames = [nil] + %w{Jan Feb Mar Apr Maj Jun Jul Aug Sep Okt Nov Dec}
    Daynames       = %w{Nedelja Ponedeljak Utorak Sreda Četvrtak Petak Subota}
    AbbrDaynames   = %w{Ned Pon Uto Sre Čet Pet Sub}
    
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
      :unit      => "din.",
      :separator => ".",             #unit separator (between integer part and fraction part)
      :delimiter => ",",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:number, :unit] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'i',
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
Inflector.inflections do |inflect|
#   inflect.plural /^(ox)$/i, '\1en'
#   inflect.singular /^(ox)en/i, '\1'
  inflect.irregular 'greška', 'greške'
#   inflect.uncountable %w( information )
end

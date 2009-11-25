# lang_fr.rb
# Traduction française des messages d'erruer. A compléter ou corriger.
# Translation by Frédéric Cavazza ( www.fredcavazza.net )


module LocalizationSimplified
  About = {
    :lang => "fr",
    :updated => "2007-06-17"
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
      :inclusion           => "n'est pas inclus dans la liste",
      :exclusion           => "est réservé",
      :invalid             => "est invalide",
      :confirmation        => "ne correspond pas à la confirmation",
      :accepted            => "doit être accepté",
      :empty               => "ne peut pas être vide",
      :blank               => "ne peut pas être vierge",# alternate formulation: "is required"
      :too_long            => "est trop long (%d caractères maximum)",
      :too_short           => "est trop court (%d caractères minimum)",
      :wrong_length        => "n'est pas de la bonne longueur (devrait être de %d caractères)",
      :taken               => "est déjà pris",
      :not_a_number        => "n'est pas le nombre",
      #Jespers additions:
      :error_translation   => "erreur",
      :error_header        => "%s interdit ce %s d'être sauvegardé",
      :error_subheader     => "Il y a des problèmes avec les champs suivants :"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "moins de %d secondes",
      :half_a_minute       => "une demi-minute",
      :less_than_a_minute  => "moins d'une minute",
      :one_minute          => "1 minute",
      :x_minutes           => "%d minutes",
      :one_hour            => "à peu près 1 heure",
      :x_hours             => "à peu près %d heures",
      :one_day             => "1 jour",
      :x_days              => "%d jours",
      :one_month           => "1 mois",
      :x_months            => "%d mois",
      :one_year            => "1 an",
      :x_years             => "%d ans"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Janvier Février Mars Avril Mai Juin Juillet Août Septembre Octobre Novembre Decembre}
    AbbrMonthnames = [nil] + %w{Jan Fév Mar Avr Mai Jui Jul Aoû Sep Oct Nov Déc}
    Daynames       = %w{Dimanche Lundi Mardi Mercredi Jeudi Vendredi Samedi}
    AbbrDaynames   = %w{Dim Lun Mar Mer Jeu Ven Sam}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%d-%m-%Y",
      :short    => "%e %b",
      :long     => "%e %B %Y"
    }
    TimeFormats = {
      :default  => "%a, %d %b %Y %H:%M:%S %z",
      :short    => "%d %b %H:%M",
      :long     => "%d %B %Y %H:%M"
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
      :separator => ",",             #unit separator (between integer part and fraction part)
      :delimiter => " ",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:number, :unit] #order is at present unsupported in Rails
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'et',
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

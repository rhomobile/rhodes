# LocalizationSimplified (L10n-simplified)
# Really simple localization for Rails
# By Jesper Rønn-Jensen ( http://justaddwater.dk/ )
# Plugin available at http://rubyforge.org/projects/l10n-simplified/
#

curLocale = System::get_locale()
puts 'Current locale: ' + curLocale.to_s

#if File.exist?('rholang/lang_' + curLocale + '.iseq')
#    require 'rholang/lang_' + curLocale
#elsif curLocale != 'en'
#    puts 'Could not load locale: ' + curLocale.to_s + '. Load english.'
#
#    require 'rholang/lang_en'
#end
begin
    require 'rholang/lang_' + curLocale
rescue Exception => e
    puts 'Could not load locale: ' + curLocale.to_s + '. Load english.'
    require 'rholang/lang_en' unless curLocale == 'en'
end

module LocalizationSimplified
  @@ignore = "\xFF\xFF\xFF\xFF" # %% == Literal "%" character
  # substitute all daynames and monthnames with localized names
  # from RUtils plugin
  def self.localize_strftime(date='%d.%m.%Y', time='')
    date.gsub!(/%%/, @@ignore)
    date.gsub!(/%a/, LocalizationSimplified::DateHelper::AbbrDaynames[time.wday])
    date.gsub!(/%A/, LocalizationSimplified::DateHelper::Daynames[time.wday])
    date.gsub!(/%b/, LocalizationSimplified::DateHelper::AbbrMonthnames[time.mon])
    date.gsub!(/%B/, LocalizationSimplified::DateHelper::Monthnames[time.mon])
    date.gsub!(@@ignore, '%%')
  end
end

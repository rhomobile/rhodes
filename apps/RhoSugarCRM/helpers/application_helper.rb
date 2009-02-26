module ApplicationHelper
  def strip_braces(str=nil)
    str ? str.gsub(/\{/,"").gsub(/\}/,"") : nil
  end
  
  def strike_if(str, condition=true)
    condition ? "<s>#{str}</s>" : str
  end
  
  def display_blanks(value)
    if blank?(value)
      "---"
    else
      value
    end
  end
  
  def blank?(value)
    value.nil? || value == "" || value.length==0
  end
  
  def display_blankstr(value)
    if blank?(value)
      " "
    else
      value
    end
  end
  
  def display_newline()
    "<br>"
  end
  
  def display_space()
    " "
  end
  
  def display_dollars(value)
    if blank?(value)
      " "
    else
      "$" + sprintf("%.2f", value)
    end   
  end
  
  def display_number(value)
    if blank?(value)
      " "
    else
      sprintf("%.2f", value)
    end   
  end
  
  def both_items_present?(value1, value2)
    !blank?(value1) && !blank?(value2)
  end
  
  def replace_newlines(value)
    if blank?(value)
      " "
    else
      value.gsub('\n', ' ')
    end       
  end
  
  def format_address_for_mapping(street, city, state, zip, tagforurl)
    if !tagforurl
      result = street + ", " + city + ", " + state + " " + zip
    else
      # need to URL encode data too
      result = "&street=" + street + "&city=" + city + "&state=" + state + "&zip=" + zip
    end
    result
  end
  
  def has_valid_mapping_address(street, city, state, zip)
    # we allow an empty zip for now
    !street.nil? && (street.length > 0) && !city.nil? && (city.length > 0) && !state.nil? && (state.length > 0)
  end
  
end
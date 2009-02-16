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
  
end
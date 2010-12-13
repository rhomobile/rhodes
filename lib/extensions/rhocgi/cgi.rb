class CGI
  TABLE_FOR_ESCAPE_HTML__ = {
    '&' => '&amp;',
    '"' => '&quot;',
    '<' => '&lt;',
    '>' => '&gt;',
  }

  # Escape special characters in HTML, namely &\"<>
  #   CGI::escapeHTML('Usage: foo "bar" <baz>')
  #      # => "Usage: foo &quot;bar&quot; &lt;baz&gt;"
  def CGI::escapeHTML(string)
    string.gsub(/[&\"<>]/, TABLE_FOR_ESCAPE_HTML__)
  end

end

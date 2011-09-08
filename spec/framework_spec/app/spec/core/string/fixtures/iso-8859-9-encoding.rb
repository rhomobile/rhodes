
module StringSpecs
  class ISO88599Encoding
    def source_encoding; __ENCODING__; end
    def x_escape; "\xDF"; end
    def ascii_only; "glark"; end
    def cedilla; "Ş"; end
  end
end

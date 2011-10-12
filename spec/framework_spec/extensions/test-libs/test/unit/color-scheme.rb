require 'test/unit/color'

module Test
  module Unit
    class ColorScheme
      include Enumerable

      class << self
        def default
          if available_colors == 256
            default_for_256_colors
          else
            default_for_8_colors
          end
        end

        @@default_for_8_colors = nil
        def default_for_8_colors
          @@default_for_8_colors ||=
            new("pass" => Color.new("green", :foreground => false) +
                          Color.new("white", :bold => true),
                "failure" => Color.new("red", :foreground => false) +
                             Color.new("white", :bold => true),
                "pending" => Color.new("magenta", :bold => true),
                "omission" => Color.new("blue", :bold => true),
                "notification" => Color.new("cyan", :bold => true),
                "error" => Color.new("yellow", :bold => true) +
                           Color.new("black", :foreground => false),
                "case" => Color.new("white", :bold => true) +
                          Color.new("blue", :foreground => false),
                "suite" => Color.new("white", :bold => true) +
                           Color.new("green", :foreground => false),
                "diff-inserted-tag" => Color.new("red", :bold => true),
                "diff-deleted-tag" => Color.new("green", :bold => true),
                "diff-difference-tag" => Color.new("cyan", :bold => true),
                "diff-inserted" => Color.new("red", :foreground => false) +
                                   Color.new("white", :bold => true),
                "diff-deleted" =>  Color.new("green", :foreground => false) +
                                   Color.new("white", :bold => true))
        end

        @@default_for_256_colors = nil
        def default_for_256_colors
          @@default_for_256_colors ||=
            new("pass" => Color.new("030", :foreground => false) +
                          Color.new("555", :bold => true),
                "failure" => Color.new("300", :foreground => false) +
                             Color.new("555", :bold => true),
                "pending" => Color.new("303", :foreground => false) +
                              Color.new("555", :bold => true),
                "omission" => Color.new("001", :foreground => false) +
                              Color.new("555", :bold => true),
                "notification" => Color.new("011", :foreground => false) +
                                  Color.new("555", :bold => true),
                "error" => Color.new("550", :bold => true) +
                           Color.new("000", :foreground => false),
                "case" => Color.new("220", :foreground => false) +
                          Color.new("555", :bold => true),
                "suite" => Color.new("110", :foreground => false) +
                           Color.new("555", :bold => true),
                "diff-inserted-tag" => Color.new("500", :foreground => false) +
                                       Color.new("000", :bold => true),
                "diff-deleted-tag" => Color.new("050", :foreground => false) +
                                      Color.new("000", :bold => true),
                "diff-difference-tag" => Color.new("005", :foreground => false) +
                                         Color.new("555", :bold => true),
                "diff-inserted" => Color.new("300", :foreground => false) +
                                   Color.new("555", :bold => true),
                "diff-deleted" =>  Color.new("030", :foreground => false) +
                                   Color.new("555", :bold => true))
        end

        @@schemes = {}
        def all
          @@schemes.merge("default" => default)
        end

        def [](id)
          @@schemes[id.to_s]
        end

        def []=(id, scheme_or_spec)
          if scheme_or_spec.is_a?(self)
            scheme = scheme_or_spec
          else
            scheme = new(scheme_or_spec)
          end
          @@schemes[id.to_s] = scheme
        end

        def available_colors
          case ENV["COLORTERM"]
          when "gnome-terminal"
            256
          else
            case ENV["TERM"]
            when "xterm-256color"
              256
            else
              8
            end
          end
        end
      end

      def initialize(scheme_spec)
        @scheme = {}
        scheme_spec.each do |key, color_spec|
          self[key] = color_spec
        end
      end

      def [](name)
        @scheme[name.to_s]
      end

      def []=(name, color_spec)
        @scheme[name.to_s] = make_color(color_spec)
      end

      def each(&block)
        @scheme.each(&block)
      end

      def to_hash
        hash = {}
        @scheme.each do |key, color|
          hash[key] = color
        end
        hash
      end

      private
      def make_color(color_spec)
        if color_spec.is_a?(Color) or color_spec.is_a?(MixColor)
          color_spec
        else
          color_name = nil
          normalized_color_spec = {}
          color_spec.each do |key, value|
            key = key.to_sym
            if key == :name
              color_name = value
            else
              normalized_color_spec[key] = value
            end
          end
          Color.new(color_name, normalized_color_spec)
        end
      end
    end
  end
end

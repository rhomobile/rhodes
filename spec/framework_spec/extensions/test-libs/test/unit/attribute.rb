module Test
  module Unit
    module Attribute
      class StringifyKeyHash < Hash
        class << self
          def stringify(object)
            object.to_s
          end
        end

        def [](key)
          super(self.class.stringify(key))
        end

        def []=(key, value)
          super(self.class.stringify(key), value)
        end
      end

      class << self
        def included(base)
          base.extend(BaseClassMethods)
          base.extend(ClassMethods)
        end
      end

      module BaseClassMethods
        def attributes_table
          {}
        end
      end

      module ClassMethods
        def method_added(name)
          super
          return unless defined?(@current_attributes)

          attributes = {}
          kept_attributes = StringifyKeyHash.new
          @current_attributes.each do |attribute_name, attribute|
            attributes[attribute_name] = attribute[:value]
            kept_attributes[attribute_name] = attribute if attribute[:keep]
          end
          set_attributes(name, attributes)
          @current_attributes = kept_attributes
        end

        def attribute(name, value, options={}, *method_names)
          unless options.is_a?(Hash)
            method_names << options
            options = {}
          end
          if method_names.empty?
            current_attributes[name] = options.merge(:value => value)
          else
            method_names.each do |method_name|
              set_attributes(method_name, {name => value})
            end
          end
        end

        def current_attributes
          @current_attributes ||= StringifyKeyHash.new
        end

        def current_attribute(name)
          current_attributes[name] || StringifyKeyHash.new
        end

        def attributes_table
          @attributes_table ||= StringifyKeyHash.new
          super.merge(@attributes_table)
        end

        def set_attributes(method_name, new_attributes)
          return if new_attributes.empty?
          @attributes_table ||= StringifyKeyHash.new
          @attributes_table[method_name] ||= StringifyKeyHash.new
          current_attributes = @attributes_table[method_name]
          new_attributes.each do |key, value|
            observers = attribute_observers(key) || []
            observers.each do |observer|
              observer.call(self,
                            StringifyKeyHash.stringify(key),
                            (attributes(method_name) || {})[key],
                            value,
                            method_name)
            end
            current_attributes[key] = value
          end
        end

        def attributes(method_name)
          attributes = attributes_table[method_name]
          ancestors[1..-1].each do |ancestor|
            if ancestor.is_a?(Class) and ancestor < Test::Unit::Attribute
              parent_attributes = ancestor.attributes(method_name)
              if attributes
                attributes = (parent_attributes || {}).merge(attributes)
              else
                attributes = parent_attributes
              end
              break
            end
          end
          attributes || StringifyKeyHash.new
        end

        @@attribute_observers = StringifyKeyHash.new
        def register_attribute_observer(attribute_name, observer=Proc.new)
          @@attribute_observers[attribute_name] ||= []
          @@attribute_observers[attribute_name] << observer
        end

        def attribute_observers(attribute_name)
          @@attribute_observers[attribute_name]
        end
      end

      def attributes
        self.class.attributes(@method_name) || StringifyKeyHash.new
      end

      def [](name)
        attributes[name]
      end
    end
  end
end

module Test
  module Unit
    module Data
      class << self
        def included(base)
          base.extend(ClassMethods)
        end
      end

      module ClassMethods
        # TODO: WRITE ME.
        def data(*arguments, &block)
          n_arguments = arguments.size
          case n_arguments
          when 0
            data_set = block
          when 1
            data_set = arguments[0]
          when 2
            data_set = {arguments[0] => arguments[1]}
          else
            message = "wrong number arguments(#{n_arguments} for 1..2)"
            raise ArgumentError, message
          end
          current_data = current_attribute(:data)[:value] || []
          attribute(:data, current_data + [data_set])
        end

        # TODO: WRITE ME.
        def load_data(file_name)
          case file_name
          when /\.csv/i
            loader = CSVDataLoader.new(self)
            loader.load(file_name)
          else
            raise ArgumentError, "unsupported file format: <#{file_name}>"
          end
        end

        class CSVDataLoader
          def initialize(test_case)
            @test_case = test_case
          end

          def load(file_name)
            require 'csv'
            header = nil
            CSV.foreach(file_name) do |row|
              if header.nil?
                header = row
                next
              end
              label = nil
              data = {}
              header.each_with_index do |key, i|
                if key == "label"
                  label = row[i]
                else
                  data[key] = normalize_value(row[i])
                end
              end
              @test_case.data(label, data)
            end
          end

          private
          def normalize_value(value)
            Integer(value)
          rescue ArgumentError
            begin
              Float(value)
            rescue ArgumentError
              value
            end
          end
        end
      end
    end
  end
end

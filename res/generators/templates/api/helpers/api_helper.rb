class CppGen
  class << self
    def simple_native_type_or_string(gen_type)
      RhogenCore::SIMPLE_TYPES.include?(gen_type.type) ? native_type(gen_type) : "rho::String"
    end

    def native_type(gen_type)
      case gen_type.type
        when RhogenCore::TYPE_INT
          res = "int"
        when RhogenCore::TYPE_BOOL
          res = "bool"
        when RhogenCore::TYPE_DOUBLE
          res = "double"
        when RhogenCore::TYPE_STRING
          res = "rho::String"
        when RhogenCore::TYPE_ARRAY
          case gen_type.api_style
            # always and only strings
            when RhogenCore::API_STYLE_LEGACY
              val_type = "rho::String" #simple_native_type_or_string(gen_type.value)
            when RhogenCore::API_STYLE_NESTED
              val_type = native_type(gen_type.value)
          end

          res = "rho::Vector<#{val_type}>".gsub('>>', '> >')
        when RhogenCore::TYPE_HASH
          case gen_type.api_style
            # always and only strings
            when RhogenCore::API_STYLE_LEGACY
              key_type = "rho::String" #simple_native_type_or_string(gen_type.key)
              val_type = "rho::String" #simple_native_type_or_string(gen_type.value)
            when RhogenCore::API_STYLE_NESTED
              key_type = native_type(gen_type.key)
              val_type = native_type(gen_type.value)
          end

          res = "rho::Hashtable<#{key_type}, #{val_type}>".gsub('>>', '> >')
        else
          #Module name, pass ID of object
          res = "rho::String"
        #raise "Unknown parameter type: #{gen_type}"
      end
      res
    end

    def templatify(fn_name, datum)
      "#{fn_name}" #"<#{datum.map{|x| native_type(x)}.join(', ')}>"
    end

    def result_converter(gen_type)
      result = ''
      case gen_type.type
        when RhogenCore::TYPE_ARRAY
          if gen_type.simple_value?
            datum = [gen_type.value]
            result = templatify('rho_value_to_typed_array', datum)
          elsif gen_type.value.simple_value?
            if gen_type.value_type == RhogenCore::TYPE_ARRAY
              datum = [gen_type.value.value]
              result = templatify('rho_value_to_typed_array_array', datum)
            elsif gen_type.value_type == RhogenCore::TYPE_HASH
              datum = [gen_type.value.key, gen_type.value.value]
              result = templatify('rho_value_to_typed_array_hash', datum)
            else
              raise "invalid parameter 1 #{gen_type.name}"
            end

          else
            raise "invalid parameter 2 #{gen_type.name}"
          end
        when RhogenCore::TYPE_HASH
          if gen_type.simple_value?
            datum = [gen_type.key, gen_type.value]
            result = templatify('rho_value_to_typed_hash', datum)
          elsif gen_type.value.simple_value?
            if gen_type.value_type == RhogenCore::TYPE_ARRAY
              datum = [gen_type.key, gen_type.value.value]
              result = templatify('rho_value_to_typed_hash_array', datum)
            elsif gen_type.value_type == RhogenCore::TYPE_HASH
              datum = [gen_type.key, gen_type.value.key, gen_type.value.value]
              result = templatify('rho_value_to_typed_hash_hash', datum)
            else
              raise "invalid parameter 3 #{gen_type.name}"
            end

          else
            raise "invalid parameter 4 #{gen_type.name}"
          end

        else
          raise "invalid parameter 5 #{gen_type.name}"
      end


      result
    end

    def native_type_arg(gen_type)
      type = native_type(gen_type)
      # non POD types
      if !['int', 'bool', 'double'].include?(type)
        type = "const #{type}&"
      end

      type
    end

    def native_result_type(gen_type)
      case gen_type.type
        when RhogenCore::TYPE_STRING
          res = "CMethodResult::eString"
        when RhogenCore::TYPE_ARRAY
          res = "CMethodResult::eStringArray"
        when RhogenCore::TYPE_HASH
          res = "CMethodResult::eStringHash"
        when RhogenCore::TYPE_INT
          res = "CMethodResult::eInt"
        when RhogenCore::TYPE_BOOL
          res = "CMethodResult::eBool"
        when RhogenCore::TYPE_DOUBLE
          res = "CMethodResult::eDouble"
        else
          res = "CMethodResult::eNone"
      end

      res
    end

    def make_namespace(parents)
      namespace = ""
      parents.each do |parent|
        namespace += parent.downcase() + "::"
      end

      namespace
    end
  end
end

class JNIGen
  class << self
    def hash_table_element_type(sub_params)
      return "rho::String" if !sub_params || sub_params.empty?

      res = nil
      sub_params.each do |name, param|
        type = native_type(param)
        if res == nil
          res = type
        else
          return "rho::String" unless res == type
        end
      end

      res
    end

    def native_type(param)
      res = "rho::String"
      return res unless param

      if param.type == RhogenCore::TYPE_STRING
        res = "rho::String"
      elsif param.type == RhogenCore::TYPE_INT
        res = "int"
      elsif param.type == RhogenCore::TYPE_BOOL
        res = "bool"
      elsif param.type == RhogenCore::TYPE_DOUBLE
        res = "double"
      elsif param.type == RhogenCore::TYPE_ARRAY
        res = "rho::Vector<#{native_type(param.sub_param)}>"
      elsif param.type == RhogenCore::TYPE_HASH
        res = "rho::Hashtable<rho::String, rho::String>"
      else
        res = "rho::String" # object id
      end

      res
    end

    def native_type_arg(param)

      if param.type == RhogenCore::TYPE_STRING
        res = "const rho::String&"
      elsif param.type == RhogenCore::TYPE_INT
        res = "int"
      elsif param.type == RhogenCore::TYPE_BOOL
        res = "bool"
      elsif param.type == RhogenCore::TYPE_DOUBLE
        res = "double"
      elsif param.type == RhogenCore::TYPE_ARRAY
        res = "const rho::Vector<#{native_type(param.sub_param)}>&"
      elsif param.type == RhogenCore::TYPE_HASH
        res = "const rho::Hashtable<rho::String, #{hash_table_element_type(param.sub_params)}&>"
      else
        res = "const rho::String&" # object id
      end

      res
    end

    def JNI_type(gen_type)
      res = "jstring"
      case gen_type.type
        when RhogenCore::TYPE_STRING
          res = "jstring"
        when RhogenCore::TYPE_ARRAY
          res = "jobject"
        when RhogenCore::TYPE_HASH
          res = "jobject"
        when RhogenCore::TYPE_INT
          res = "jint"
        when RhogenCore::TYPE_BOOL
          res = "jboolean"
        when RhogenCore::TYPE_DOUBLE
          res = "jdouble"
        else
          #raise "Unknown parameter type: #{gen_type}"
      end

      res
    end
  end
end

class JavaGen
  class << self
    def map_element_type(sub_params, parent)
      if !sub_params || sub_params.empty?
        return "String"
      end

      res = nil
      sub_params.each do |param|
        type = native_type(param, parent)
        if res == nil
          res = type
        else
          return "Object" unless res == type
        end
      end

      res
    end

    def make_simple_native_type(gen_type)

      case gen_type.type
        when RhogenCore::TYPE_STRING
          res = "String"
        when RhogenCore::TYPE_ARRAY
          res = "List<String>"
        when RhogenCore::TYPE_HASH
          res = "Map<String, String>"
        when RhogenCore::TYPE_INT
          res = "int"
        when RhogenCore::TYPE_BOOL
          res = "boolean"
        when RhogenCore::TYPE_DOUBLE
          res = "double"
        else
          res = "String"
      end

      res
    end

    def native_type(param, parent=nil)
      unless param
        puts "Default java type: String"
        return "String"
      end

      if param.type == RhogenCore::TYPE_STRING
        res = "String"
      elsif param.type == RhogenCore::TYPE_ARRAY
        subtype = "String"
        subtype = native_type(param.value, param) if param.value
        res = "List<#{subtype}>"
      elsif param.type == RhogenCore::TYPE_HASH
        subtype = "String"
        subtype = map_element_type(param.fields, param) if param.fields
        res = "Map<String, #{subtype}>"
      elsif param.type == RhogenCore::TYPE_INT
        if parent
          res = "Integer"
        else
          res = "int"
        end
      elsif param.type == RhogenCore::TYPE_BOOL
        if parent
          res = "Boolean"
        else
          res = "boolean"
        end
      elsif param.type == RhogenCore::TYPE_DOUBLE
        if parent
          res = "Double"
        else
          res = "double"
        end
      else
        res = "String"
      end

      res
    end

    def make_native_type_signature(gen_type)

      if gen_type == RhogenCore::TYPE_STRING
        res = "Ljava/lang/String;"
      elsif gen_type == RhogenCore::TYPE_ARRAY
        res = "Ljava/util/List;"
      elsif gen_type == RhogenCore::TYPE_HASH
        res = "Ljava/util/Map;"
      elsif gen_type == RhogenCore::TYPE_INT
        res = "I"
      elsif gen_type == RhogenCore::TYPE_BOOL
        res = "Z"
      elsif gen_type == RhogenCore::TYPE_DOUBLE
        res = "D"
      else
        res = "Ljava/lang/String;"
      end

      res
    end

    def make_package_name(cur_module)
      res = "com."
      cur_module.parents.each do |parent|
        res += parent.downcase() + "."
      end
      res += cur_module.name.downcase

      res
    end

    def make_package_path(cur_module)
      res = "com/"
      cur_module.parents.each do |parent|
        res += parent.downcase() + "/"
      end
      res += cur_module.name.downcase

      res
    end
  end
end


def api_generator_cli_conversion(gen_type, var_name)

  if gen_type == RhogenCore::TYPE_ARRAY
    res = "Windows::Foundation::Collections::IVectorView<Platform::String^>^ _#{var_name} = rho::common::convertArrayToWP8(#{var_name});\n"
  elsif gen_type == RhogenCore::TYPE_HASH
    res = "Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ _#{var_name} = rho::common::convertHashToWP8(#{var_name});\n"
  elsif gen_type == RhogenCore::TYPE_INT
    res = ''
  elsif gen_type == RhogenCore::TYPE_BOOL
    res = ''
  elsif gen_type == RhogenCore::TYPE_DOUBLE
    res = ''
  else # elif gen_type == RhogenCore::TYPE_STRING
    res = "Platform::String^ _#{var_name} = rho::common::convertStringToWP8(#{var_name});\n"
  end

  res
end

def api_generator_cli_makeNativeTypeArg(gen_type)

  if gen_type == RhogenCore::TYPE_STRING
    res = "Platform::String^"
  elsif gen_type == RhogenCore::TYPE_ARRAY
    res = "Windows::Foundation::Collections::IVectorView<Platform::String^>^"
  elsif gen_type == RhogenCore::TYPE_HASH
    res = "Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^"
  elsif gen_type == RhogenCore::TYPE_INT
    res = "int"
  elsif gen_type == RhogenCore::TYPE_BOOL
    res = "bool"
  elsif gen_type == RhogenCore::TYPE_DOUBLE
    res = "double"
  else
    res = "Platform::String^"
  end

  res
end

def api_generator_cs_makeNativeTypeArg(gen_type)

  if gen_type == RhogenCore::TYPE_STRING
    res = "string"
  elsif gen_type == RhogenCore::TYPE_ARRAY
    res = "IReadOnlyList<string>"
  elsif gen_type == RhogenCore::TYPE_HASH
    res = "IReadOnlyDictionary<string, string>"
  elsif gen_type == RhogenCore::TYPE_INT
    res = "int"
  elsif gen_type == RhogenCore::TYPE_BOOL
    res = "bool"
  elsif gen_type == RhogenCore::TYPE_DOUBLE
    res = "double"
  else
    res = "string"
  end

  res
end

def api_generator_isApiObjectParam(param)
  return false unless param

  if param.type == RhogenCore::TYPE_ARRAY
    array_type = param.kind_of?(RhogenCore::MethodResult) ? param.sub_param : param.value

    if array_type
      return api_generator_isApiObjectParam(array_type)
    else
      return false
    end

  elsif param.type == RhogenCore::TYPE_HASH
    hash_fields = param.kind_of?(RhogenCore::MethodResult) ? param.sub_params : param.fields.map{|f| [f.name,f]}
    if hash_fields
      hash_fields.each do |name, param|
        isObject = api_generator_isApiObjectParam(param)
        return false unless isObject
      end
      return true
    else
      return false
    end
  else
    if param.type != RhogenCore::TYPE_STRING and param.type != RhogenCore::TYPE_INT and param.type != RhogenCore::TYPE_BOOL and param.type != RhogenCore::TYPE_DOUBLE
      return true
    else
      return false
    end
  end
end

def api_generator_ruby_makeApiObjectTypeName(param, cur_module)
  if param.nil? || param.type == RhogenCore::TYPE_CLASS
    return api_generator_getRubyModuleFullName2(param.self_type)
  elsif param.type == RhogenCore::TYPE_ARRAY
    return api_generator_ruby_makeApiObjectTypeName(param.sub_param, cur_module)
  elsif param.type == RhogenCore::TYPE_HASH
    return api_generator_ruby_makeApiObjectTypeName(param.sub_params.first, cur_module)
  else
    return param.type.gsub('.', '::')
  end
end

def api_generator_CreateSimpleRubyType(gen_type, value)

  res = ""
  if gen_type == RhogenCore::TYPE_STRING
    res = "rb_str_new2(\"#{value}\")"
  elsif gen_type == RhogenCore::TYPE_INT
    res = "rb_ll2inum(#{value})"
  elsif gen_type == RhogenCore::TYPE_BOOL
    res = "strcmp(\"#{value}\", \"true\") = 0 ? Qtrue : Qfalse"
  elsif gen_type == RhogenCore::TYPE_DOUBLE
    res = "rb_float_new(#{value})"
  else
    raise "Type is not simple: #{gen_type}"
  end

  res
end

def api_generator_MakeRubyMethodDecl(module_name, module_method, is_static)

  method_name = 'rb_'
  method_name += 's_' if is_static
  method_name += module_name + "_" + module_method.native_name

  params = ''
  if is_static
    params = 'int argc, VALUE *argv'
  else
    params = 'int argc, VALUE *argv, VALUE obj'
  end

  "VALUE #{method_name}(#{params})"
end

def api_generator_getRubyModuleFullName2(src)
  src.gsub('.', '::')
end

def api_generator_getRubyModuleFullName(cur_module)
  module_name = cur_module.parents.join('.')
  module_name += '.' if module_name && module_name.length() > 0
  module_name += cur_module.name

  module_name.gsub('.', '::')
end

def api_generator_getJSModuleName(module_name)
  module_name.gsub('::', '.')
end

def api_generator_MakeJSMethodDecl(module_name, module_method_name, is_static)

  method_name = 'js_'
  method_name += 's_' if is_static
  method_name += module_name + "_" + module_method_name

  params = 'const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam'

  "rho::String #{method_name}(#{params})"
end

def api_generator_isSelfModule(cur_module, type)
  return false unless type

  api_generator_getRubyModuleFullName(cur_module) == type.gsub('.', '::')
end
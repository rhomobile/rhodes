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

class CppGen
  class << self
    def native_type(gen_type)
      puts "=======\n"
      pp(gen_type)
      case gen_type.type
        when RhogenCore::TYPE_STRING
          res = "rho::String"
        when RhogenCore::TYPE_INT
          res = "int"
        when RhogenCore::TYPE_BOOL
          res = "bool"
        when RhogenCore::TYPE_DOUBLE
          res = "double"
        when RhogenCore::TYPE_ARRAY
          val_type = native_type(gen_type.value)
          res = "rho::Vector<#{val_type}>"
        when RhogenCore::TYPE_HASH
          key_type = native_type(gen_type.key)
          val_type = native_type(gen_type.value)
          res = "rho::Hashtable<#{key_type}, #{val_type}>"
        else
          #Module name, pass ID of object
          res = "rho::String"
        #raise "Unknown parameter type: #{gen_type}"
      end
      res
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

def api_generator_jni_makeHashTableElementType(sub_params)
    return "rho::String" unless sub_params

    res = nil
    sub_params.each do |name, param|
        type = api_generator_jni_makeNativeType(param)
        if res == nil
            res = type
        else
            return "rho::String" unless res == type
        end
    end
    
    res
end

def api_generator_jni_makeNativeType(param)
    res = "rho::String"
    return res unless param
    
    if param.type == RhogenCore::TYPE_STRING
        res = "rho::String"
    elsif param.type == RhogenCore::TYPE_ARRAY
        res = "rho::Vector<#{api_generator_jni_makeNativeType(param.sub_param)}>"
    elsif param.type == RhogenCore::TYPE_HASH
        res = "rho::Hashtable<rho::String, rho::String>"
    elsif param.type == RhogenCore::TYPE_INT
        res = "int"
    elsif param.type == RhogenCore::TYPE_BOOL
        res = "bool"
    elsif param.type == RhogenCore::TYPE_DOUBLE
        res = "double"
    else
        res = "rho::String" # object id
    end
    
    res
end

def api_generator_jni_makeNativeTypeArg(param)
    
    if param.type == RhogenCore::TYPE_STRING
        res = "const rho::String&"
    elsif param.type == RhogenCore::TYPE_ARRAY
        res = "const rho::Vector<#{api_generator_jni_makeNativeType(param.sub_param)}>&"
    elsif param.type == RhogenCore::TYPE_HASH
        res = "const rho::Hashtable<rho::String, #{api_generator_jni_makeHashTableElementType(param.sub_params)}&>"
    elsif param.type == RhogenCore::TYPE_INT
        res = "int"
    elsif param.type == RhogenCore::TYPE_BOOL
        res = "bool"
    elsif param.type == RhogenCore::TYPE_DOUBLE
        res = "double"
    else
        res = "const rho::String&" # object id
    end
    
    res
end

def api_generator_java_makeMapElementType(sub_params, parent)
    unless sub_params
        puts "Hash sub_params is nil"
        return "String"
    end

    res = nil
    sub_params.each do |param|
        type = api_generator_java_makeNativeType(param, parent)
        if res == nil
            res = type
        else
            return "Object" unless res == type
        end
    end
    
    res
end

def api_generator_java_makeSimpleNativeType(type)

    if type == RhogenCore::TYPE_STRING
        res = "String"
    elsif type == RhogenCore::TYPE_ARRAY
        res = "List<String>"
    elsif type == RhogenCore::TYPE_HASH
        res = "Map<String, String>"
    elsif type == RhogenCore::TYPE_INT
        res = "int"
    elsif type == RhogenCore::TYPE_BOOL
        res = "boolean"
    elsif type == RhogenCore::TYPE_DOUBLE
        res = "double"
    else
        res = "String"
    end
    
    res
end

def api_generator_java_makeNativeType(param, parent=nil)
    unless param
        puts "Default java type: String"
        return "String" 
    end

    if param.type == RhogenCore::TYPE_STRING
        res = "String"
    elsif param.type == RhogenCore::TYPE_ARRAY
        subtype = "String"
        subtype = api_generator_java_makeNativeType(param.sub_param, param) if param.sub_param
        res = "List<#{subtype}>"
    elsif param.type == RhogenCore::TYPE_HASH
        subtype = "String"
        subtype = api_generator_java_makeMapElementType(param.sub_params, param) if param.sub_params
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

def api_generator_java_makeNativeTypeSignature(gen_type)
    
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

def api_generator_jni_makeJNIType(gen_type)
    if gen_type == RhogenCore::TYPE_STRING
        res = "jstring"
    elsif gen_type == RhogenCore::TYPE_ARRAY
        res = "jobject"
    elsif gen_type == RhogenCore::TYPE_HASH
        res = "jobject"
    elsif gen_type == RhogenCore::TYPE_INT
        res = "jint"
    elsif gen_type == RhogenCore::TYPE_BOOL
        res = "jboolean"
    elsif gen_type == RhogenCore::TYPE_DOUBLE
        res = "jdouble"
    else
        #raise "Unknown parameter type: #{gen_type}"
    end
    
    res
end

def api_generator_isApiObjectParam(param)
    return false unless param

    if param.type == RhogenCore::TYPE_ARRAY
        if param.sub_param
            return api_generator_isApiObjectParam(param.sub_param)
        else
            return false
        end
    elsif param.type == RhogenCore::TYPE_HASH
        if param.sub_params
            param.sub_params.each do |name, param|
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
    if param.nil? || param.type == RhogenCore::TYPE_SELF
        return api_generator_getRubyModuleFullName(cur_module)
    elsif param.type == RhogenCore::TYPE_ARRAY
        return api_generator_ruby_makeApiObjectTypeName(param.sub_param, cur_module)
    elsif param.type == RhogenCore::TYPE_HASH
        return api_generator_ruby_makeApiObjectTypeName(param.sub_params.first, cur_module)
    else
        return param.type.gsub('.', '::')
    end
end

def api_generator_MakeJSMethodDecl(module_name, module_method_name, is_static)

    method_name = 'js_'
    method_name += 's_' if is_static
    method_name += module_name + "_" + module_method_name

    params = 'const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam'

    "rho::String #{method_name}(#{params})"
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


def api_generator_getRubyModuleFullName(cur_module)
    module_name = cur_module.parents.join('.')
    module_name += '.' if module_name && module_name.length() > 0
    module_name += cur_module.name

    module_name.gsub('.', '::')
end

def api_generator_getJSModuleName(module_name)
    module_name.gsub('::', '.')    
end

def api_generator_isSelfModule(cur_module, type)
    return false unless type
    return true if type == RhogenCore::TYPE_SELF
    
    api_generator_getRubyModuleFullName(cur_module) == type.gsub('.', '::')
end

def api_generator_java_makePackageName(cur_module)
    res = "com."
    cur_module.parents.each do |parent|
        res += parent.downcase() + "."
    end
    res += cur_module.name.downcase
    
    res
end

def api_generator_java_makePackagePath(cur_module)
    res = "com/"
    cur_module.parents.each do |parent|
        res += parent.downcase() + "/"
    end
    res += cur_module.name.downcase
    
    res
end
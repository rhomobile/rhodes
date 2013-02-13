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

def api_generator_cpp_makeNativeType(gen_type)
    
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "rho::StringW"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_ARRAY
        res = "rho::Vector<rho::StringW>"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_HASH
        res = "rho::Hashtable<rho::StringW, rho::StringW>"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "int64"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "bool"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "double"
    else
        raise "Unknown parameter type: #{gen_type}"     
    end
    
    res
end

def api_generator_cpp_makeNativeTypeArg(gen_type)
    
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "const rho::StringW&"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_ARRAY
        res = "const rho::Vector<rho::StringW>&"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_HASH
        res = "const rho::Hashtable<rho::StringW, rho::StringW>&"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "int64"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "bool"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "double"
    else
        raise "Unknown parameter type: #{gen_type}"     
    end
    
    res
end

def api_generator_jni_makeNativeType(gen_type)
    
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "rho::String"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_ARRAY
        res = "rho::Vector<rho::String>"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_HASH
        res = "rho::Hashtable<rho::String, rho::String>"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "int64"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "bool"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "double"
    else
        #raise "Unknown parameter type: #{gen_type}"
    end
    
    res
end

def api_generator_java_makeNativeType(gen_type)
    
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "String"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_ARRAY
        res = "List<String>"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_HASH
        res = "Map<String, String>"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "int"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "boolean"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "double"
    else
        raise "Unknown parameter type: #{gen_type}"     
    end
    
    res
end

def api_generator_java_makeNativeTypeSignature(gen_type)
    
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "Ljava/lang/String;"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_ARRAY
        res = "Ljava/util/List;"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_HASH
        res = "Ljava/util/Map;"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "I"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "Z"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "D"
    else
        raise "Unknown parameter type: #{gen_type}"     
    end
    
    res
end

def api_generator_jni_makeJNIType(gen_type)
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "jstring"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_ARRAY
        res = "jobject"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_HASH
        res = "jobject"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "jint"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "jboolean"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "jdouble"
    else
        #raise "Unknown parameter type: #{gen_type}"
    end
    
    res
end

def api_generator_MakeJSMethodDecl(module_name, module_method_name, is_static)

    method_name = 'js_'
    method_name += 's_' if is_static
    method_name += module_name + "_" + module_method_name

    params = 'rho::json::CJSONArray& argv, const rho::String& strObjID'

    "rho::String #{method_name}(#{params})"
end

def api_generator_CreateSimpleRubyType(gen_type, value)

    res = ""
    if gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_STRING
        res = "rb_str_new2(\"#{value}\")"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_INT
        res = "rb_ll2inum(#{value})"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_BOOL
        res = "strcmp(\"#{value}\", \"true\") = 0 ? Qtrue : Qfalse"
    elsif gen_type == Rhogen::ApiGenerator::MethodParam::TYPE_DOUBLE
        res = "rb_float_new(#{value})"
    else    
        raise "Type is not simple: #{gen_type}"     
    end
    
    res
end

def api_generator_cpp_MakeNamespace(parents)
    namespace = ""
    parents.each do |parent|
        namespace += parent.downcase() + "::"
    end
    
    namespace
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
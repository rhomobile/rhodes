def api_generator_MakeRubyMethodDecl(module_name, module_method, is_static)

    method_name = 'rb_'
    method_name += 's_' if is_static
    method_name += module_name + "_" + module_method.name

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
    else
        raise "Unknown parameter type: #{gen_type}"     
    end
    
    res
end

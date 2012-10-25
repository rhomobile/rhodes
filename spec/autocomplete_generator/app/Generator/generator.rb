$rbExt = ".rb"

def undecorateConstantName(name)
  name = name.to_s
  name = name[0, name.size]
  return name
end

def printTitleInfo(file)
  if file.nil?
    return
  end 

  file.puts " "
  file.puts "#"
  file.puts "# It is auto-generated content."
  file.puts "# Do not do required for this file in your application."
  file.puts "#"
  file.puts " "
end

def decoratePattern(pattern, level, name, params)
  if params.nil?
    return decorateOffset(level, pattern + name.to_s + "\n") + decorateOffset(level, "end") + decorateOffset(level, "")
  else    
    paramsDeclaration = String.new
    params.each { |param| 
      paramsDeclaration += param.to_s 
      paramsDeclaration += ","
    }

    if !paramsDeclaration.empty?
      paramsDeclaration = paramsDeclaration[0..-2]
      funcDef = decorateOffset(level, pattern + name.to_s + "(" + paramsDeclaration + ")\n")
    else
      funcDef = decorateOffset(level, pattern + name.to_s + "\n")
    end

    return funcDef + decorateOffset(level, "end") + decorateOffset(level, "")
  end
end

def decorateInstanceMethod(level, name, params)
  decoratePattern("def ", level, name, params)
end

def decorateMethod(level, name, params)
  decoratePattern("def self.", level, name, params)
end

def decorateOffset(columnNum, printLine)
  return "  " * columnNum  + printLine
end

def prepareUserDefinedSubclass(constNames)
  autoGenerateConsts  = [ "Instance", "Class", "Base", "Object", "BasicObject", "Kernel", 
	"NilClass", "NIL", "Data", "TrueClass", "TRUE", "FalseClass", "FALSE", "Encoding", "Comparable", "Enumerable", 
	"String", "Symbol", "Exception", "SystemExit", "SignalException", "Interrupt", "StandardError", 
	"TypeError", "ArgumentError", "IndexError", "KeyError", "RangeError", "ScriptError", "SyntaxError", 
	"LoadError", "NotImplementedError", "NameError", "NoMethodError", "RuntimeError", 
	"SecurityError", "NoMemoryError", "EncodingError", "SystemCallError", "Errno", "ZeroDivisionError", 
	"FloatDomainError", "Numeric", "Integer", "Fixnum", "Float", "Bignum", "Array", "Hash", 
	"ENV", "Struct", "RegexpError", "Regexp", "MatchData", "Marshal", "Range", "IOError", "EOFError", "IO", 
	"STDIN", "STDOUT", "STDERR", "ARGF", "FileTest", "Random", "Signal", 
	"Process", "Proc", "LocalJumpError", "SystemStackError", "Method", "UnboundMethod", "Binding", 
        "Math", "GC", "ObjectSpace", "Enumerator", "StopIteration", "RubyVM", "TOPLEVEL_BINDING", 
	"ThreadGroup", "Mutex", "ThreadError", "Fiber", "FiberError", "Rational", "Complex", "RUBY_VERSION", 
	"RUBY_RELEASE_DATE", "RUBY_PLATFORM", "RUBY_PATCHLEVEL", "RUBY_REVISION", "RUBY_DESCRIPTION", 
	"RUBY_COPYRIGHT", "RUBY_ENGINE", "ARGV", "Gem", "RbConfig", "Config", "CROSS_COMPILING", "Module",
        "TMP_RUBY_PREFIX", "RHO_FRAMEWORK" ]

  constNames.delete_if{ |item| autoGenerateConsts.include?(undecorateConstantName(item)) }
  return constNames
end

$recursiveLevel = 2

def printClassInfo(cls, baseClass, level, moduleFile)

  if level >= $recursiveLevel
    return
  end

  begin
    if baseClass.nil?
      puts cls.to_s
      clsObj = Object.const_get(undecorateConstantName(cls))
    else
      puts baseClass.to_s + ":" + cls.to_s
      clsObj = baseClass.const_get(undecorateConstantName(cls))
    end

    if clsObj.nil?
      return
    end
  rescue
    p 'wrong name - ' + baseClass.to_s + undecorateConstantName(cls)
    return
  end

  begin
    p '1-' + clsObj.class.to_s + " - " + cls.to_s
  rescue
    p 'extception 1'
  end

  begin
    case clsObj.class.to_s
      when "String"
        puts cls.to_s + ' - String const' 
        if clsObj == "\\"
          moduleFile.puts decorateOffset(level, cls.to_s + " = \"\\\\\"")
        else
          moduleFile.puts decorateOffset(level, cls.to_s + " = \"" + clsObj + "\"")
        end
        moduleFile.puts ''
        return
      when "Integer"
        puts cls.to_s + ' - Integer const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + clsObj.to_s)
        moduleFile.puts ''
        return
      when "Fixnum"
        puts cls.to_s + ' - Fixnum const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + clsObj.to_s)
        moduleFile.puts ''
        return
      when "Hash"
        puts cls.to_s + ' - Hash const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + "{}")
        moduleFile.puts ''
        return
      when "Array"
        puts cls.to_s + ' - Array const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + "[]")
        moduleFile.puts ''
        return
      when "Mutex"
        puts cls.to_s + ' - Mutex const' 
        return
      when "FalseClass"
        puts cls.to_s + ' - FalseClass const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + "false")
        moduleFile.puts ''
        return
      when "TrueClass"
        puts cls.to_s + ' - TrueClass const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + "true")
        moduleFile.puts ''
        return
      when "Complex"
        puts cls.to_s + ' - Complex const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + "1")
        moduleFile.puts ''
        return
      when "Float"
        puts cls.to_s + ' - Float const' 
        moduleFile.puts decorateOffset(level, cls.to_s + " = " + clsObj.to_s)
        moduleFile.puts ''
        return
      else
        moduleFile.puts decorateOffset(level, clsObj.class.to_s.downcase + " " + cls.to_s)

        if !clsObj.constants.nil?
          p '2-' + clsObj.constants.to_s
    
          subConstants = prepareUserDefinedSubclass(clsObj.constants)

          subConstants.each { |constant|
          printClassInfo(constant, clsObj, level + 1, moduleFile)
        }
        end
    end

  rescue
    p 'extception 2'
  end
  

  begin
    if !clsObj.methods(false).nil?
      p '3-' + clsObj.methods(false).to_s

      methods = clsObj.methods(false)

      methods.each { |method|  
          p '3.1- ' + method.to_s + " - " + clsObj.method(method.to_s).parameters.to_s #map(&:last).map(&:to_s).to_s
          moduleFile.puts decorateMethod(level + 1, method, clsObj.method(method.to_s).parameters.map(&:last).map(&:to_s))
          p '3.2- '
      }
    end
  rescue
    p 'extception 3'
  end

  begin
    if !clsObj.instance_methods.nil?
      p '4-' + clsObj.instance_methods(false).to_s

      instance_methods = clsObj.instance_methods(false)

      instance_methods.each { |method|      
        p '4.1- ' + method.to_s + " - " + clsObj.instance_method(method.to_s).parameters.to_s #map(&:last).map(&:to_s).to_s
        moduleFile.puts decorateInstanceMethod(level + 1, method, clsObj.instance_method(method.to_s).parameters.map(&:last).map(&:to_s))
        p '4.2- '
      }
    end
  rescue
    p 'extception 4'
  end

  moduleFile.puts decorateOffset(level, "end")
  moduleFile.puts ''
end

def generate
  t1 = Thread.new do
    base = Array.new
    base << Module.constants
    base = base[0]

    puts '***********************************************'

    classes = prepareUserDefinedSubclass(base)
    p classes

    classes.each { |cls|
      puts cls    
      moduleFile = File.new(cls.to_s + $rbExt , "w+")

      printTitleInfo(moduleFile)
      printClassInfo(cls, nil, 0, moduleFile)
      moduleFile.close
    }
  end
end
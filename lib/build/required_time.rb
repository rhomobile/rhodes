# helper class from https://gist.github.com/defunkt/278994

module RequiredTime
  class << self
    def hook
      Kernel.class_eval do
        alias_method :__require_benchmarking_old_require, :require
        def require(path, *args)
          RequiredTime.benchmark_require(path, caller) { __require_benchmarking_old_require(path, *args) }
        end
      end
      @hooked = true
    end

    def hooked?
      @hooked
    end

    def benchmark_require(path, full_backtrace, &block)
      output = nil
      backtrace = full_backtrace.reject {|x| x =~ /require|dependencies/ }
      caller = File.expand_path(backtrace[0].split(":")[0])
      parent = required_files.find {|f| f[:fullpath] == caller }
      unless parent
        parent = {
            :index => required_files.size,
            :fullpath => caller,
            :parent => nil,
            :is_root => true
        }
        required_files << parent
      end
      fullpath = find_file(path)
      expanded_path = path; expanded_path = File.expand_path(path) if path =~ /^\//
      new_file = {
          :index => required_files.size,
          :xpath => path,
          :path => expanded_path,
          :fullpath => fullpath,
          :backtrace => full_backtrace,
          :parent => parent,
          :is_root => false
      }
      # add this before the required is required so that anything that is required
      # within the required that's about to be required already has a parent present
      required_files << new_file
      start = Time.now

      output = yield # do the require here

      new_file[:time] = Time.now.to_f - start.to_f
      output
    end


    def generate_benchmark_report
      @printed_files = []
      generate_benchmark_report_level(@required_files.select {|file| file[:is_root] })
    end
    private

    def required_files
      @required_files ||= []
    end

    def printed_files
      @printed_files ||= []
    end

    def proj_dir
      @proj_dir ||= File.expand_path(File.dirname(__FILE__) + "/..")
    end

    def find_file(path)
      return File.expand_path(path) if path =~ /^\//
      expanded_path = nil
# Try to find the path in the ActiveSupport load paths and then the built-in load paths
      catch :found_path do
        %w(rb bundle so).each do |ext|
          path_suffix = path; path_suffix = "#{path}.#{ext}" unless path_suffix =~ /\.#{ext}$/
          ($:).each do |path_prefix|
            possible_path = File.join(path_prefix, path_suffix)
            if File.file? possible_path
              expanded_path = File.expand_path(possible_path)
              throw :found_path
            end
          end
          expanded_path
        end
      end
      expanded_path
    end

    def generate_benchmark_report_level(files, printing_all=false)
      report = []

      if printing_all
        files = files.sort {|a,b| b[:time] <=> a[:time] }
      else
        files = files.sort_by {|f| [(f[:parent] ? 1 : 0), -(f[:time] || 0), f[:index]] }
      end
      for required in files
        already_printed = printed_files.include?(required[:fullpath])
        # don't print this required if it's already been printed,
        # or it will have been printed
        next if already_printed
        if required[:parent] && !printing_all
          next if required[:index] < required[:parent][:index]
        end

        path = required[:fullpath] ? format_path(required[:fullpath]) : required[:path]

        out = "#{required[:index]+1}) "
        if required[:time] && !already_printed
          #if required[:time] >= 0.5
          # out << "%s: %.4f s" % [path, required[:time]]
          #else
          ms = required[:time].to_f * 1000
          out << "%s: %.1f ms" % [path, ms]
          #end
        else
          out << path
        end
        if required[:is_root] && required[:parent]
          out << " (required by #{required[:parent][:fullpath]})"
        end
        unless required[:parent]
          out << " (already loaded)"
        end
        if already_printed
          out << " (already printed)"
        end
        report << out

        unless already_printed
          printed_files << required[:fullpath]
          unless printing_all
            children = @required_files.select {|f| !f[:is_root] && f[:parent] && f[:parent][:fullpath] == required[:fullpath] }
            if children.any?
              report.concat(generate_benchmark_report_level(children).map{|s| '  ' + s })
            end
          end
        end
      end

      report
    end

    def format_path(path)
      path.sub(proj_dir, "*")
    end
  end
end
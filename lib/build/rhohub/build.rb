class Rhohub::Build < Rhohub::Base

  def self.create(options,data)
    catch_all do
      raise Exception.new "missing target_device" unless data[:build]['target_device']
      raise Exception.new "missing version_tag" unless data[:build]['version_tag']
      raise Exception.new "missing rhodes_version"  unless data[:build]['rhodes_version']
      raise Exception.new "missing app_id" unless options[:app_id]
      super(options,data,'builds')
    end
  end

  # options = {:id => build_id}
  def self.delete(options)
    catch_all do
      raise Exception.new "id must be passed to show ex: {:id => build_id}" unless options[:id]
      raise Exception.new "app_id must be passed to show ex: {:app_id => app_id}" unless options[:app_id]
      super(options,'builds')
    end
  end

  # options = {:app_id => app_id}
  def self.list(options)
    catch_all do
      raise Exception.new "app_id must be passed to show ex: {:app_id => app_id}" unless options[:app_id]
      super(options,'builds')
    end
  end

  # options = {:id => build_id}
  def self.show(options)
    catch_all do
      raise Exception.new "id must be passed to show ex: {:id => build_id}" unless options[:id]
      raise Exception.new "app_id must be passed to show ex: {:app_id => app_id}" unless options[:app_id]
      super(options,'builds')
    end
  end

  def self.platforms
    catch_all do
      super({},'platforms')
    end
  end
end

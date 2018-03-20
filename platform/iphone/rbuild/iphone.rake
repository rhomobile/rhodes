#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------

require File.expand_path(File.join(File.dirname(__FILE__), 'iphonecommon'))
require File.dirname(__FILE__) + '/../../../lib/build/BuildConfig'


$out_file_buf_enable = false
$out_file_buf_path = 'rhobuildlog.txt'
$out_file_buf = []

puts 'iphone.rake execute' if USE_TRACES
puts 'ENV["RHO_BUNDLE_BUILD_LOG_FILE"] = '+ENV["RHO_BUNDLE_BUILD_LOG_FILE"].to_s if USE_TRACES
if (ENV["RHO_BUNDLE_BUILD_LOG_FILE"] != nil)
    $out_file_buf_path = ENV["RHO_BUNDLE_BUILD_LOG_FILE"]
    $out_file_buf_enable = true
    load File.expand_path(File.join(File.dirname(__FILE__), 'putsOverride.rake'))
end



def save_out_file
    if $out_file_buf_enable
        f = File.new($out_file_buf_path,"w")
        $out_file_buf.each do |line|
            f.write(line)
            f.write("\n")
        end
        f.close
    end
end





def load_plist(fname)
  require 'cfpropertylist'

  plist = CFPropertyList::List.new(:file => fname)
  data = CFPropertyList.native_types(plist.value)

  data
end

def save_plist(fname, hash_data = {})
  require 'cfpropertylist'

  plist = CFPropertyList::List.new
  plist.value = CFPropertyList.guess(hash_data)
  plist.save(fname, CFPropertyList::List::FORMAT_XML,{:formatted=>true})
end

def update_plist_block(fname)
  hash = load_plist(fname)

  if block_given?
    yield hash
  end

  save_plist(fname, hash)

  hash
end

def set_app_plist_options(fname, appname, bundle_identifier, version, url_scheme)
  update_plist_block(fname) do |hash|
    hash['CFBundleDisplayName'] = appname
    hash['CFBundleIdentifier'] = bundle_identifier

    if hash['CFBundleURLTypes'].empty?
      hash['CFBundleURLTypes'] = {'CFBundleURLName' => bundle_identifier, 'CFBundleURLSchemes' => [url_scheme] }
    else
      elem = hash['CFBundleURLTypes'].first

      elem['CFBundleURLName'] = bundle_identifier
      elem['CFBundleURLSchemes'] = [url_scheme] unless url_scheme.nil?
    end

    unless version.nil?
      hash['CFBundleVersion'] = version
      hash['CFBundleShortVersionString'] = version
    end

    if block_given?
      yield hash
    end
  end
end

def get_ext_plist_changes(ext_path_to_cfg_map)
  changed_value = {}
  extension_name = {}

  ext_path_to_cfg_map.each do |ext, conf|
    plist_addons = BuildConfig.find_elem(conf, 'iphone/plist_changes')

    unless plist_addons.nil?
      full_patj = File.join(ext, plist_addons.to_s)

      if File.exist?(full_patj)
        hash = load_plist(full_patj)

        hash.each do |k, v|
          if extension_name.has_key?(k)
            BuildOutput.error(["Extension #{ext} overrides key #{k} that was set by #{extension_name[k]}"])
          end

          extension_name[k] = ext
          changed_value[k] = v
        end
      end
    end
  end
  return extension_name, changed_value
end


#
# def set_ui_prerendered_icon(val)
#     add = (val=~(/(true|t|yes|y|1)$/i))?true:false
#
#     ret_value = nil
#
#     #fname = $config["build"]["iphonepath"] + "/Info.plist"
#     fname = $app_path + "/project/iphone/Info.plist"
#     nextline = false
#     replaced = false
#     dictcnt = 0
#     buf = ""
#     File.new(fname,"r").read.each_line do |line|
#         matches = (line =~ /UIPrerenderedIcon/)?true:false
#         if nextline and not replaced
#             ret_value = true
#             return ret_value if add
#
#             replaced = true
#         else
#             if (line=~/<\/dict>/)
#                 if add and (dictcnt==1)
#                     buf << "<key>UIPrerenderedIcon</key>\n"
#                     buf << "<true/>\n"
#                 end
#                 dictcnt = dictcnt-1
#             elsif (line=~/<dict>/)
#                 dictcnt = dictcnt+1
#             end
#
#             buf << line unless ( matches and not add )
#         end
#         nextline = matches
#     end
#
#     File.open(fname,"w") { |f| f.write(buf) }
#     return ret_value
# end

def recursive_replace_bool(value)
    if value.kind_of?(Hash)
        value.each_key do |nkey|
            value[nkey] = recursive_replace_bool(value[nkey])
        end
    elsif value.kind_of?(Array)
        new_array = []
        value.each do |el|
            new_array << recursive_replace_bool(el)
        end
    else
        if (value.to_s.downcase == 'true') || (value.to_s.downcase == 'yes')
            value = true
        end
        if (value.to_s.downcase == 'false') || (value.to_s.downcase == 'no')
            value = false
        end
    end
    return value
end

def recursive_merge_hash(hash, key, value)
    old_value = hash[key]
    if old_value.nil?
        hash[key] = value
    elsif value.kind_of?(Array)
        if old_value.kind_of?(Array)
            value.each do |element|
                if !old_value.include?(element)
                    old_value << element
                end
            end
        else
            hash[key] = value
        end
    elsif value.kind_of?(Hash)
        if old_value.kind_of?(Hash)
            value.each do |nkey, nvalue|
                recursive_merge_hash(old_value, nkey, nvalue)
            end
        else
            hash[key] = value
        end
    else
        hash[key] = value
    end
end


def recursive_remove_hash(hash, key)
    if key.kind_of?(Array)
        key.each do |element|
            recursive_remove_hash(hash, element)
        end
    elsif key.kind_of?(Hash)
        key.each do |keykey, keyvalue|
            if hash.has_key? keykey
                oldkey = hash[keykey]
                recursive_remove_hash(oldkey, keyvalue)
            end
        end
    elsif key.kind_of?(String)
        #if hash.has_key? key
            hash.delete(key)
        #end
    end
end



def update_plist_procedure
      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

      vendor = $app_config['vendor'] ? $app_config['vendor'] : "rhomobile"
      bundle_identifier = "com.#{vendor}.#{appname}"
      bundle_identifier = $app_config["iphone"]["BundleIdentifier"] unless $app_config["iphone"]["BundleIdentifier"].nil?

      on_suspend = $app_config["iphone"]["UIApplicationExitsOnSuspend"]
      on_suspend_value = false

      if on_suspend.nil?
        puts "UIApplicationExitsOnSuspend not configured, using default of false"
      elsif on_suspend.to_s.downcase == "true" || on_suspend.to_s == "1"
        on_suspend_value = true
      elsif on_suspend.to_s.downcase == "false" || on_suspend.to_s == "0"
        on_suspend_value = false
      else
        raise "UIApplicationExitsOnSuspend is not set to a valid value. Current value: '#{$app_config["iphone"]["UIApplicationExitsOnSuspend"]}'"
      end

      init_extensions( nil, "get_ext_xml_paths")

      ext_name, changed_value = get_ext_plist_changes($app_extension_cfg)

      set_app_plist_options($app_path + "/project/iphone/Info.plist", appname, bundle_identifier, $app_config["version"], $app_config["iphone"]["BundleURLScheme"]) do |hash|
         hash['UIApplicationExitsOnSuspend'] = on_suspend_value

        changed_value.each do |k, v|
          puts "Info.plist: Setting key #{k} = #{v} from #{File.basename(ext_name[k])}"
          hash[k] = v
        end

        #setup GPS access
        gps_request_text = nil
        if $app_config["capabilities"].index("gps") != nil
          gps_request_text = 'application tracks your position'
        end
        if !$app_config["iphone"].nil?
          if !$app_config["iphone"]["capabilities"].nil?
            if $app_config["iphone"]["capabilities"].index("gps") != nil
              gps_request_text = 'application tracks your position'
            end
          end
        end
        if gps_request_text != nil
          if hash['NSLocationWhenInUseUsageDescription'] == nil
            puts "Info.plist: added key [NSLocationWhenInUseUsageDescription]"
            hash['NSLocationWhenInUseUsageDescription'] = gps_request_text
          end
        end

        #setup Camera access
        camera_request_text = nil
        if $app_config["capabilities"].index("camera") != nil
          camera_request_text = 'application wants to use camera'
        end
        if !$app_config["iphone"].nil?
          if !$app_config["iphone"]["capabilities"].nil?
            if $app_config["iphone"]["capabilities"].index("camera") != nil
              camera_request_text = 'application wants to use camera'
            end
          end
        end
        if camera_request_text != nil
          if hash['NSCameraUsageDescription'] == nil
            puts "Info.plist: added key [NSCameraUsageDescription]"
            hash['NSCameraUsageDescription'] = camera_request_text
          end
        end


        #LSApplicationQueriesSchemes
        if $app_config["iphone"].has_key?("ApplicationQueriesSchemes")
          arr_app_queries_schemes = $app_config["iphone"]["ApplicationQueriesSchemes"]
          if arr_app_queries_schemes.kind_of?(Array)
            hash['LSApplicationQueriesSchemes'] = arr_app_queries_schemes
          else
            hash['LSApplicationQueriesSchemes'] = []
          end
        end

        #http_connection_domains
        if $app_config["iphone"].has_key?("http_connection_domains")
          http_connection_domains = $app_config["iphone"]["http_connection_domains"]
          if http_connection_domains.kind_of?(Array)
              if !hash.has_key?("NSAppTransportSecurity")
                  hash['NSAppTransportSecurity'] = {}
              end
              hash['NSAppTransportSecurity']['NSExceptionDomains'] = {}
              http_connection_domains.each do |domain|
                  domain_hash = {}
                  domain_hash['NSIncludesSubdomains'] = true
                  domain_hash['NSTemporaryExceptionAllowsInsecureHTTPLoads'] = true
                  domain_hash['NSTemporaryExceptionMinimumTLSVersion'] = 'TLSv1.0'
                  domain_hash['NSExceptionAllowsInsecureHTTPLoads'] = true
                  domain_hash['NSExceptionMinimumTLSVersion'] = 'TLSv1.0'
                  hash['NSAppTransportSecurity']['NSExceptionDomains'][domain.to_s] = domain_hash
              end
          end
        end

        # add custom data
        if $app_config["iphone"].has_key?("info_plist_data_remove")
            info_plist_data_remove = $app_config["iphone"]["info_plist_data_remove"]
            if info_plist_data_remove.kind_of?(Array)
                info_plist_data_remove.each do |key|
                    recursive_remove_hash(hash, key)
                end
            end
        end
        if $app_config["iphone"].has_key?("info_plist_data")
            info_plist_data = $app_config["iphone"]["info_plist_data"]
            if info_plist_data.kind_of?(Hash)
                info_plist_data.each do |key, value|
                    value = recursive_replace_bool(value)
                    recursive_merge_hash(hash, key, value)
                end
            end
        end


         set_app_icon()
         set_default_images(false, hash)
      end
end

def set_signing_identity(identity,profile,entitlements,provisioning_style,development_team)

  appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
  appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| (w.capitalize) }.join("")

  #fname = $config["build"]["iphonepath"] + "/rhorunner.xcodeproj/project.pbxproj"
  fname = $app_path + "/project/iphone" + "/" + appname_fixed + ".xcodeproj/project.pbxproj"
  buf = ""
  File.new(fname,"r").read.each_line do |line|
    if entitlements != nil
       line.gsub!(/CODE_SIGN_ENTITLEMENTS = .*;/,"CODE_SIGN_ENTITLEMENTS = \"#{entitlements}\";")
    end
    if provisioning_style != nil
       line.gsub!(/ProvisioningStyle = .*;/,"ProvisioningStyle = \"#{provisioning_style}\";")
    end
    if development_team != nil
       line.gsub!(/DevelopmentTeam = .*;/,"DevelopmentTeam = \"#{development_team}\";")
       line.gsub!(/DEVELOPMENT_TEAM = .*;/,"DEVELOPMENT_TEAM = \"#{development_team}\";")
    end
    line.gsub!(/CODE_SIGN_IDENTITY = .*;/,"CODE_SIGN_IDENTITY = \"#{identity}\";")
    line.gsub!(/"CODE_SIGN_IDENTITY\[sdk=iphoneos\*\]" = .*;/,"\"CODE_SIGN_IDENTITY[sdk=iphoneos*]\" = \"#{identity}\";")
    if profile and profile.to_s != ""
      line.gsub!(/PROVISIONING_PROFILE = .*;/,"PROVISIONING_PROFILE = \"#{profile}\";")
      line.gsub!(/"PROVISIONING_PROFILE\[sdk=iphoneos\*\]" = .*;/,"\"PROVISIONING_PROFILE[sdk=iphoneos*]\" = \"#{profile}\";")
    end

    puts line if line =~ /CODE_SIGN/
    buf << line
  end

  File.open(fname,"w") { |f| f.write(buf) }
end



def make_app_info
  fname = File.join($app_path, 'bin', 'target', 'iOS', $sdk, $configuration, 'app_info.txt')
  buf = ""
  urlscheme = 'rhodes'
  urlscheme = $app_config["name"] unless $app_config["name"].nil?
  urlscheme = $app_config["iphone"]["BundleURLScheme"] unless $app_config["iphone"]["BundleURLScheme"].nil?

  urlscheme = urlscheme.split(/[^a-zA-Z0-9\_\-]/).map{|w| w}.join("_")

  buf << urlscheme
  File.open(fname,"w") { |f| f.write(buf) }
end


def prepare_production_ipa (app_path, app_name)
  puts 'Preparing *.IPA file ...'

  tmp_dir = File.join(app_path, "tmp_ipa")
  mkdir_p tmp_dir
  payload_dir = File.join(tmp_dir, "Payload")
  mkdir_p payload_dir
  app_file = File.join(app_path, app_name + ".app")
  app_in_payload = File.join(payload_dir,  app_name + ".app")

  mprovision_in_app = File.join(app_file,  "embedded.mobileprovision")
  mprovision_in_payload = File.join(payload_dir,  app_name + ".mobileprovision")

  cp_r app_file, app_in_payload

  executable_file =  File.join(app_in_payload, 'rhorunner')
  if !File.executable? executable_file
     begin
         File.chmod 0700, executable_file
         puts 'executable attribute was writed for : '+executable_file
     rescue Exception => e
         puts 'ERROR: can not change attribute for executable in application package ! Try to run build command with sudo: prefix.'
     end
  end


  #cp mprovision_in_app, mprovision_in_payload

  # now iTunesArtwork should be placed into application bundle !
  #cp itunes_artwork, itunes_artwork_dst

  currentdir = Dir.pwd()
  chdir tmp_dir
  sh %{zip -r -y temporary_archive.zip .}
  ipa_file_path = File.join(app_path, app_name + ".ipa")
  rm_rf ipa_file_path
  cp 'temporary_archive.zip', ipa_file_path
  Dir.chdir currentdir
  rm_rf tmp_dir

  return ipa_file_path
end


#TODO: support assets !
def copy_all_png_from_icon_folder_to_product(app_path)
#   rm_rf File.join(app_path, "*.png")
#
#   app_icon_folder = File.join($app_path, 'resources', 'ios')
#   if File.exists? app_icon_folder
#       # NEW resources
#       Dir.glob(File.join(app_icon_folder, "icon*.png")).each do |icon_file|
#         cp icon_file, app_path
#       end
#   else
#       app_icon_folder = File.join($app_path, 'icon')
#       Dir.glob(File.join(app_icon_folder, "*.png")).each do |icon_file|
#         cp icon_file, app_path
#       end
#   end
end

def prepare_production_plist (app_path, app_name)
    puts 'Preparing application production plist ...'

    $plist_title = app_name
    $plist_subtitle = app_name
    $plist_icon_url = "http://example.com/icon57.png"
    $plist_ipa_url = "http://example.com/"+app_name+".ipa"

    appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
    vendor = $app_config['vendor'] ? $app_config['vendor'] : "rhomobile"
    $plist_bundle_id = "com.#{vendor}.#{appname}"
    $plist_bundle_id = $app_config["iphone"]["BundleIdentifier"] unless $app_config["iphone"]["BundleIdentifier"].nil?

    $plist_bundle_version = "1.0"
    $plist_bundle_version = $app_config["version"] unless $app_config["version"].nil?

    if !$app_config["iphone"].nil?
      if !$app_config["iphone"]["production"].nil?
        $plist_title = $app_config["iphone"]["production"]["app_plist_title"] unless $app_config["iphone"]["production"]["app_plist_title"].nil?
        $plist_subtitle = $app_config["iphone"]["production"]["app_plist_subtitle"] unless $app_config["iphone"]["production"]["app_plist_subtitle"].nil?
        $plist_icon_url = $app_config["iphone"]["production"]["app_plist_icon_url"] unless $app_config["iphone"]["production"]["app_plist_icon_url"].nil?
        $plist_ipa_url = $app_config["iphone"]["production"]["app_plist_ipa_url"] unless $app_config["iphone"]["production"]["app_plist_ipa_url"].nil?
      end
    end

    rbText = ERB.new( IO.read(File.join($config["build"]["iphonepath"], "rbuild", "ApplicationPlist.erb")) ).result
	fAlx = File.new(File.join(app_path, app_name + ".plist"), "w")
    fAlx.write(rbText)
    fAlx.close()
end


ICONS = [
'icon20.png',
'icon29.png',
'icon40.png',
'icon50.png',
'icon57.png',
'icon58.png',
'icon60.png',
'icon72.png',
'icon76.png',
'icon80.png',
'icon87.png',
'icon100.png',
'icon114.png',
'icon120.png',
'icon144.png',
'icon152.png',
'icon167.png',
'icon180.png',
'icon1024.png']



def set_app_icon
  puts "set icon"
  #ipath = $config["build"]["iphonepath"]

  begin
    ICONS.each do |icon|
      name = icon
      ipath = $app_path + "/project/iphone/Media.xcassets/AppIcon.appiconset"
      icon = File.join(ipath, name)
      appicon_old = File.join($app_path, 'icon', name)
      appicon = appicon_old
      appicon_new = File.join($app_path, 'resources', 'ios', name)
      if File.exists? appicon_new
          appicon = appicon_new
      end
      if File.exists? appicon
          if File.exists? ipath
            rm_f ipath
          end
         cp appicon, ipath
      else
         #puts "WARNING: application should have next icon file : "+ name + '.png !!!'
         BuildOutput.warning("Can not found next icon file : "+ name + ' , Use default Rhodes image !!!' )
      end
    end
  rescue => e
    puts "WARNING!!! Can not change icon: #{e.to_s}"
  end
end



LOADINGIMAGES = [
'Default.png',
'Default@2x.png',
'Default-568h@2x.png',
'Default-667h@2x.png',
'Default-736h@3x.png',
'Default-812h@3x.png',
'Default-Portrait.png',
'Default-Portrait@2x.png',
'Default-Landscape.png',
'Default-Landscape@2x.png',
'Default-Landscape-736h@3x.png',
'Default-Landscape-812h@3x.png'
]

def remove_lines_from_xcode_project(array_with_substrings)
    appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
    appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| (w.capitalize) }.join("")

    fname = $app_path + "/project/iphone" + "/" + appname_fixed + ".xcodeproj/project.pbxproj"
    buf = ""
    File.new(fname,"r").read.each_line do |line|
        is_remove = false
        array_with_substrings.each do |rimg|
            if line.include?(rimg)
               is_remove = true
            end
        end
        if !is_remove
           buf << line
        end
    end
    File.open(fname,"w") { |f| f.write(buf) }
end

def set_default_images(make_bak, plist_hash)
  puts "set_default_images"
  ipath = $app_path + "/project/iphone/Media.xcassets/LaunchImage.launchimage"
  begin
    LOADINGIMAGES.each do |name|
      oldname = name.sub('Default', 'loading')
      imag = File.join(ipath, name)

      appimage = File.join($app_path, 'app', oldname)
      name_ios_ext = oldname.sub('.png', '.iphone.png')
      appsimage = File.join($app_path, 'app', name_ios_ext)
      resourcesiamge = File.join($app_path, 'resources', 'ios', name)
      if File.exists? appsimage
         appimage =  appsimage
      end
      if File.exists? resourcesiamge
         appimage =  resourcesiamge
      end

      #bundlei = File.join($srcdir, defname + '.png')
      if File.exist? appimage
          if File.exists? imag
            rm_f imag
          end
        cp appimage, imag
      else
          BuildOutput.warning("Can not found next default file : "+ name + ' , Use default Rhodes image !!!' )
      end
    end
  rescue => e
    puts "WARNING!!! Can not change default image: #{e.to_s}"
  end

end


def update_xcode_project_files_by_capabilities
    info_plist = $app_path + "/project/iphone/Info.plist"
    dev_ent = $app_path + "/project/iphone/rhorunner_development.entitlements"
    prd_ent = $app_path + "/project/iphone/rhorunner_production.entitlements"

    hash_info_plist = load_plist(info_plist)
    hash_dev_ent = load_plist(dev_ent)
    hash_prd_ent = load_plist(prd_ent)

    #bluetooth
    bt_capability = false
    if $app_config['capabilities'] != nil
        if $app_config['capabilities'].index('bluetooth')
            bt_capability = true
        end
    end
    if $app_config['iphone'] != nil
        if $app_config['iphone']['capabilities'] != nil
            if $app_config['iphone']['capabilities'].index('bluetooth')
                bt_capability = true
            end
        end
    end
    if bt_capability
        if hash_info_plist['UIRequiredDeviceCapabilities'] == nil
            hash_info_plist['UIRequiredDeviceCapabilities'] = []
        end
        hash_info_plist['UIRequiredDeviceCapabilities'] << "gamekit"
    else
        remove_lines_from_xcode_project(['GameKit.framework'])
        if hash_info_plist['UIRequiredDeviceCapabilities'] != nil
            hash_info_plist['UIRequiredDeviceCapabilities'].delete("gamekit")
        end
    end

    #external_accessory
    zebra_printing_ext = false
    if $app_config['extensions'] != nil
        if $app_config['extensions'].index('printing_zebra')
            zebra_printing_ext = true
        end
    end
    if $app_config['iphone'] != nil
        if $app_config['iphone']['extensions'] != nil
            if $app_config['iphone']['extensions'].index('printing_zebra')
                zebra_printing_ext = true
            end
        end
    end
    if zebra_printing_ext
        if $app_config['capabilities'] == nil
            $app_config['capabilities'] = []
        end
        if $app_config['capabilities'].index('external_accessory')
        else
            $app_config['capabilities'] << "external_accessory"
        end
    end
    ea_capability = false
    if $app_config['capabilities'] != nil
        if $app_config['capabilities'].index('external_accessory')
            ea_capability = true
        end
    end
    if $app_config['iphone'] != nil
        if $app_config['iphone']['capabilities'] != nil
            if $app_config['iphone']['capabilities'].index('external_accessory')
                ea_capability = true
            end
        end
    end
    if ea_capability
        hash_dev_ent['com.apple.external-accessory.wireless-configuration'] = true
        hash_prd_ent['com.apple.external-accessory.wireless-configuration'] = true
    else
        hash_dev_ent.delete('com.apple.external-accessory.wireless-configuration')
        hash_prd_ent.delete('com.apple.external-accessory.wireless-configuration')
        remove_lines_from_xcode_project(['ExternalAccessory.framework', 'com.apple.BackgroundModes = {enabled = 1;};'])
    end

    #push
    push_capability = false
    if $app_config['capabilities'] != nil
        if $app_config['capabilities'].index('push')
            push_capability = true
        end
    end
    if $app_config['iphone'] != nil
        if $app_config['iphone']['capabilities'] != nil
            if $app_config['iphone']['capabilities'].index('push')
                push_capability = true
            end
        end
    end
    if push_capability
        hash_dev_ent['aps-environment'] = 'development'
        hash_prd_ent['aps-environment'] = 'production'
    else
        hash_dev_ent.delete('aps-environment')
        hash_prd_ent.delete('aps-environment')
        remove_lines_from_xcode_project(['com.apple.Push = {enabled = 1;};'])
    end

    #keychain access
    enable_keychain = false
    if $app_config['capabilities'] != nil
        if $app_config['capabilities'].index('keychain')
            enable_keychain = true
        end
    end
    if $app_config['iphone'] != nil
        if $app_config['iphone']['capabilities'] != nil
            if $app_config['iphone']['capabilities'].index('keychain')
                enable_keychain = true
            end
        end
    end
    # required for database encryption
    if $app_config['encrypt_database'] != nil
        if $app_config['encrypt_database'] == '1'
            enable_keychain = true
        end
    end
    if enable_keychain
    else
        remove_lines_from_xcode_project(['com.apple.Keychain = {enabled = 1;};'])
    end

    save_plist(info_plist, hash_info_plist)
    save_plist(dev_ent, hash_dev_ent)
    save_plist(prd_ent, hash_prd_ent)
end

def copy_entitlements_file_from_app
  #enti_rho_name = File.join($config["build"]["iphonepath"], "Entitlements.plist")
  enti_rho_name = File.join($app_path + "/project/iphone", "Entitlements.plist")

  enti_app_name = File.join($app_path, "Entitlements.plist")

  if !$app_config["iphone"].nil?
    if !$app_config["iphone"]["entitlements_file"].nil?
      enti_test_name = $app_config["iphone"]["entitlements_file"]
      if File.exists? enti_test_name
        enti_app_name = enti_test_name
      else
        enti_test_name = File.join($app_path, $app_config["iphone"]["entitlements_file"])
        if File.exists? enti_test_name
          enti_app_name = enti_test_name
        else
          enti_app_name = $app_config["iphone"]["entitlements_file"]
        end
      end
    end
  end

  #if File.exists? enti_app_name
  #  puts 'Copy Entitlements.plist from application ...'
  #  cp enti_rho_name, (enti_rho_name + '_bak')
  #  rm_f enti_rho_name
  #  cp enti_app_name,enti_rho_name
  #end
end

def restore_entitlements_file
  #enti_rho_name = File.join($config["build"]["iphonepath"], "Entitlements.plist")
  enti_rho_name = File.join($app_path + "/project/iphone", "Entitlements.plist")
  if File.exists?(enti_rho_name + '_bak')
    puts 'restore Entitlements.plist ...'
    rm_f enti_rho_name
    cp (enti_rho_name + '_bak'), enti_rho_name
    rm_f (enti_rho_name + '_bak')
  end
end


def basedir
  File.join(File.dirname(__FILE__),'..','..','..')
end

def app_expanded_path(appname)
  File.expand_path(File.join(basedir,'spec',appname))
end

def check_sdk(sdkname)

      puts 'Check SDK :'
      args = ['-version', '-sdk', sdkname]

      puts Jake.run($xcodebuild,args)
      ret = $?

      chdir $startdir

      unless ret == 0
        puts ""
        puts "ERROR: invalid SDK in BUILD.YML !"
        puts sdkname+' is NOT installed on this computer !'
        puts ""

        puts "See all installed SDKs on this computer :"
        args = ['-showsdks']
        Jake.run($xcodebuild,args)

        exit 1
      end
end


def get_xcode_version
  info_path = '/Applications/XCode.app/Contents/version.plist'
  ret_value = '0.0'
  if File.exists? info_path
    hash = load_plist(info_path)
    ret_value = hash['CFBundleShortVersionString'] if hash.has_key?('CFBundleShortVersionString')
  else
    puts '$$$ can not find XCode version file ['+info_path+']'
  end
  puts '$$$ XCode version is '+ret_value
  return ret_value
end

def kill_iphone_simulator
  puts 'kill "iPhone Simulator"'
  `killall -9 "iPhone Simulator"`
  `killall -9 "iOS Simulator"`
  `killall -9 iphonesim`
  `killall -9 iphonesim_43`
  `killall -9 iphonesim_51`
  `killall -9 iphonesim_6`
  `killall -9 iphonesim_7`
  `killall -9 iphonesim_8`
end

namespace "config" do

  namespace "iphone" do
    task :app_config do
      if $app_config['capabilities'].index('push')
        $app_config['extensions'] << 'applePush' unless $app_config['extensions'].index('applePush')
      end

      $file_map_name = "rhofilelist.txt"
    end
  end

  task :set_iphone_platform do
    $current_platform = "iphone"
  end

  task :iphone => [:set_iphone_platform, "config:common", "switch_app"] do

    $use_prebuild_data = false

    #$rubypath = "res/build-tools/RubyMac" #path to RubyMac
    if RUBY_PLATFORM =~ /(win|w)32$/
      $all_files_mask = "*.*"
      $rubypath = "res/build-tools/RhoRuby.exe"
    else
      $all_files_mask = "*"
      if RUBY_PLATFORM =~ /darwin/
        $rubypath = "res/build-tools/RubyMac"
      else
        $rubypath = "res/build-tools/rubylinux"
      end
    end
    $file_map_name = "rhofilelist.txt"

    iphonepath = $app_path + "/project/iphone" #$config["build"]["iphonepath"]
    $builddir = $config["build"]["iphonepath"] + "/rbuild" #iphonepath + "/rbuild"
    $bindir = File.join(iphonepath, "bin")#Jake.get_absolute(iphonepath) + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = $app_path + "/bin/target/iOS"
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']
    $tmpdir =  $bindir +"/tmp"

    if $devroot.nil?
        $devroot = $app_config["iphone"]["xcodepath"]
        if $devroot == nil
            $devroot = $config["env"]["paths"]["xcodepath"]
        end
        if $devroot != nil
            $devroot = File.join($devroot, 'Contents/Developer')
        end
    end

    $devroot = '/Applications/Xcode.app/Contents/Developer' if $devroot.nil?
    $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_51') if $iphonesim.nil?

    #check for XCode 6
    xcode_version = get_xcode_version
    if xcode_version[0].to_i >= 6
      $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_6')
    end
    if xcode_version[0].to_i >= 7
      $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_7')
    end
    if xcode_version[0].to_i >= 8
      $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_8')
    end


    $xcodebuild = $devroot + "/usr/bin/xcodebuild"
    if !File.exists? $xcodebuild
        $devroot = '/Developer'
        $xcodebuild = $devroot + "/usr/bin/xcodebuild"
        $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim')
    else
        #additional checking for iphonesimulator version
      if !File.exists? '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/Library/PrivateFrameworks/DVTiPhoneSimulatorRemoteClient.framework'
        #check for XCode 6
        xcode_version = get_xcode_version
        if xcode_version[0].to_i >= 8
          $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_8')
        elsif xcode_version[0].to_i >= 7
          $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_7')
        elsif xcode_version[0].to_i >= 6
          $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_6')
          if xcode_version[0].to_i >= 7
            $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_7')
          end
          if xcode_version[0].to_i >= 8
            $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_8')
          end
        else
          $iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim_43')
        end
      end
    end

    if (!File.exists? $xcodebuild) && (!$skip_checking_XCode)
        puts 'ERROR: can not found XCode command line tools'
        puts 'Install XCode to default location'
        puts 'For XCode from 4.3 and later - you should install Command Line Tools package ! Open XCode - Preferences... - Downloads - Components - Command Line Tools'
        exit 1
    end

    if !$skip_checking_XCode
      $homedir = ENV['HOME']
      $simdir = "#{$homedir}/Library/Application Support/iPhone Simulator/"
      $sim = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/Applications"
      $guid = `uuidgen`.strip
      $applog = File.join($homedir,$app_config["applog"]) if $app_config["applog"]
    else

      if RUBY_PLATFORM =~ /(win|w)32$/
        $homedir = ''
        $devroot = ''
        $sim = ''
        $guid = ''
        $applog = ''
      else
        if RUBY_PLATFORM =~ /darwin/
          $homedir = ENV['HOME']
          $simdir = ''
          $sim = ''
          $guid = ''
          $applog = ''
        else
          $homedir = ''
          $devroot = ''
          $sim = ''
          $guid = ''
          $applog = ''
        end
      end
    end

    $entitlements = nil
    if $app_config["iphone"].nil?
      $signidentity = $config["env"]["iphone"]["codesignidentity"]
      $provisionprofile = $config["env"]["iphone"]["provisionprofile"]
      $provisioning_style = $config["env"]["iphone"]["provisioning_style"]
      $development_team = $config["env"]["iphone"]["development_team"]
      $entitlements = $config["env"]["iphone"]["entitlements"]
      $configuration = $config["env"]["iphone"]["configuration"]
      $sdk = $config["env"]["iphone"]["sdk"]
      $emulatortarget = 'iphone'
    else
      $signidentity = $app_config["iphone"]["codesignidentity"]
      $provisionprofile = $app_config["iphone"]["provisionprofile"]
      $provisioning_style = $app_config["iphone"]["provisioning_style"]
      $development_team = $app_config["iphone"]["development_team"]
      $entitlements = $app_config["iphone"]["entitlements"]
      $configuration = $app_config["iphone"]["configuration"]
      $sdk = $app_config["iphone"]["sdk"]
      $emulatortarget = $app_config["iphone"]["emulatortarget"]
      if $emulatortarget == nil
         $emulatortarget = 'iphone'
      end
    end

    if $entitlements == ""
        $entitlements = nil
    end

    if $signidentity == nil
      $signidentity = 'iPhone Developer'
    end



    # find UUID for name of mobileprovision
    if (!$skip_checking_XCode) && ($provisionprofile != nil)
        $homedir = ENV['HOME']
        mp_folder = "#{$homedir}/Library/MobileDevice/Provisioning Profiles/"

        mp_latest_UUID = nil
        mp_latest_Time = nil

        if File.exists? mp_folder
            Dir.entries(mp_folder).select do |entry|
                path = File.join(mp_folder,entry)
                #puts '$$$ '+path.to_s
                if !(File.directory? path) and !(entry =='.' || entry == '..' || entry == '.DS_Store')
                    #puts '     $$$ '+path.to_s
                    plist_path = path
                    # make XML
                    xml_lines_arr = []
                    args = ['cms', '-D', '-i', plist_path]
                    Jake.run2('security',args,{:rootdir => $startdir, :hide_output => true}) do |line|
                        xml_lines_arr << line.to_s
                        true
                    end
                    xml_lines = xml_lines_arr.join.to_s
                    #puts '%%%%%%% '+xml_lines

                    plist_obj = CFPropertyList::List.new(:data => xml_lines)
                    mp_plist_hash = CFPropertyList.native_types(plist_obj.value)
                    #puts '     $$$ '+mp_plist_hash.to_s
                    mp_name = mp_plist_hash['Name']
                    mp_uuid = mp_plist_hash['UUID']
                    mp_creation_date = mp_plist_hash['CreationDate']

                    #puts '       '+mp_creation_date.class.to_s+'    '+mp_creation_date.to_s

                    #puts '$$$$$   MP: Name: "'+mp_name+'"   UUID: ['+mp_uuid+']'

                    if mp_name == $provisionprofile
                        puts 'Found MobileProvision Name: "'+mp_name+'"   UUID: ['+mp_uuid+']    Creation Time:   '+mp_creation_date.to_s+'    File: '+path.to_s
                        #$provisionprofile = mp_uuid
                        if mp_latest_UUID == nil
                            mp_latest_UUID = mp_uuid
                            mp_latest_Time = mp_creation_date
                        else
                            if mp_creation_date > mp_latest_Time
                                mp_latest_UUID = mp_uuid
                                mp_latest_Time = mp_creation_date
                            end
                        end
                    end
                end
            end
        end
        if mp_latest_UUID != nil
            $provisionprofile = mp_latest_UUID
        end
        puts 'Processed MobileProvision UUID = '+$provisionprofile.to_s
    end

    # process special SDK names: latest, latest_simulator, latest_device
    if ($sdk =~ /latest/) && (!$skip_checking_XCode)
        args = ['-showsdks']
        nullversion = Gem::Version.new('0.0')
        latestsimulator = Gem::Version.new('0.0')
        latestdevice = Gem::Version.new('0.0')
        simulatorsdkmask = /(.*)iphonesimulator([0-9]+).([0-9]+)(.*)/
        devicemask = /(.*)iphoneos([0-9]+).([0-9]+)(.*)/

        Jake.run2($xcodebuild,args,{:rootdir => $startdir, :hide_output => true}) do |line|
            #puts 'LINE = '+line.to_s
            if (simulatorsdkmask=~line) == 0
                parsed = line.scan(simulatorsdkmask)
                curver = Gem::Version.new(parsed[0][1].to_s+'.'+parsed[0][2].to_s)
                if curver > latestsimulator
                    latestsimulator = curver
                end
            end
            if (devicemask=~line) == 0
                parsed = line.scan(devicemask)
                curver = Gem::Version.new(parsed[0][1].to_s+'.'+parsed[0][2].to_s)
                if curver > latestdevice
                    latestdevice = curver
                end
            end
            true
        end
        puts 'detect latestsimulator sdk = '+latestsimulator.to_s
        puts 'detect latestdevice sdk = '+latestdevice.to_s
        retx = $?
        #puts '### +'+retx.to_s
        if retx == 0
            if $sdk.to_s.downcase == 'latest'
                if Rake.application.top_level_tasks.to_s =~ /run/
                  $sdk = 'latest_simulator'
                else
                  $sdk = 'latest_device'
                end
            end
            if $sdk.to_s.downcase == 'latest_simulator'
                if nullversion != latestsimulator
                    $sdk = 'iphonesimulator'+latestsimulator.to_s
                end
            end
            if $sdk.to_s.downcase == 'latest_device'
                if nullversion != latestsimulator
                    $sdk = 'iphoneos'+latestdevice.to_s
                end
            end
        else
            puts "ERROR: cannot run xcodebuild for get list of installed SDKs !"
        end
    end

    if $sdk !~ /iphone/
      if Rake.application.top_level_tasks.to_s =~ /run/
        $sdk = "iphonesimulator#{$sdk}"
      else
        $sdk = "iphoneos#{$sdk}"
      end
    end

    puts 'SDK = '+$sdk

    if !$skip_checking_XCode
      check_sdk($sdk)
    end

    if $sdk =~ /iphonesimulator/
      $sdkver = $sdk.gsub(/iphonesimulator/,"")
      $sdkroot = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" + $sdkver + ".sdk"
    else
      $sdkver = $sdk.gsub(/iphoneos/,"")
      $sdkroot = $devroot + "/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS" + $sdkver + ".sdk"
    end

    $emulator_version = nil
    plist = File.join($sdkroot, 'System/Library/CoreServices/SystemVersion.plist')
    if File.exists? plist
      File.open(plist, 'r') do |f|
        while line = f.gets
          next unless line =~ /<string>(#{$sdkver.gsub('.', '\.')}[^<]*)<\/string>/
          $emulator_version = $1
          break unless $emulator_version.nil?
        end
      end
    end

    if !$skip_checking_XCode
      unless File.exists? $homedir + "/.profile"
        File.open($homedir + "/.profile","w") {|f| f << "#" }
        chmod 0744, $homedir + "/.profile"
      end
    end

    #if $app_config["iphone"] and $app_config["iphone"]["extensions"]
    #  $app_config["extensions"] += $app_config["iphone"]["extensions"] if $app_config["extensions"]
    #  $app_config["iphone"]["extensions"] = nil
    #end

    # check environment variables setted by XCode (when we executed from XCode)
    #xcode_sdk_name = ENV['SDK_NAME']
    #$sdk = xcode_sdk_name if not xcode_sdk_name.nil?
    #$xcode_sdk_dir = ENV['SDK_DIR']
    #xcode_configuration = ENV['CONFIGURATION']
    #$configuration = xcode_configuration if not xcode_configuration.nil?

    #make_project_bakup
  end
end

namespace "build" do
  namespace "iphone" do
#    desc "Build iphone rhobundle"
    # [build:iphone:rhobundle]
    task :rhobundle => ["config:iphone"] do
      print_timestamp('build:iphone:rhobundle START')

      ENV["RHO_BUNDLE_ALREADY_BUILDED"] = "NO"

      #chdir 'platform/iphone'
      chdir File.join($app_path, 'project','iphone')
      rm_rf 'bin'
      #rm_rf 'build/Debug-*'
      #rm_rf 'build/Release-*'

      chdir $startdir

      if (ENV["SDK_NAME"] != nil) and (ENV["CONFIGURATION"] != nil)
         #we should override build.yml parameters by parameters from XCode !
         $sdk = ENV["SDK_NAME"]
         $configuration = ENV["CONFIGURATION"]
      end


      Rake::Task["build:bundle:noxruby"].execute

      copy_generated_sources_and_binaries

      if !$skip_build_extensions
        Rake::Task["build:iphone:extensions"].execute
      end

      ENV["RHO_BUNDLE_ALREADY_BUILDED"] = "YES"

      # Store hash
      File.open(File.join($srcdir, "hash"), "w") { |f| f.write(get_dir_hash($srcdir).hexdigest) }
      # Store app name
      File.open(File.join($srcdir, "name"), "w") { |f| f.write($app_config["name"]) }

      Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )

      print_timestamp('build:iphone:rhobundle FINISH')

      save_out_file

    end

    desc "build upgrade package with full bundle"
    task :upgrade_package => ["config:set_iphone_platform", "config:common"] do

        $skip_checking_XCode = true
        $skip_build_rhodes_main = true
        $skip_build_extensions = true
        $skip_build_xmls = true
        $use_prebuild_data = true

        Rake::Task['config:iphone'].invoke

        appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
        appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

        iphone_project = File.join($app_path, "project","iphone","#{appname_fixed}.xcodeproj")

        if !File.exist?(iphone_project)

          puts '$ make iphone XCode project for application'
          Rake::Task['build:iphone:make_xcode_project'].invoke

        end

        Rake::Task['build:iphone:rhobundle'].invoke

        #puts '$$$$$$$$$$$$$$$$$$'
        #puts 'targetdir = '+$targetdir.to_s
        #puts 'bindir = '+$bindir.to_s


        mkdir_p $targetdir if not File.exists? $targetdir
        zip_file_path = File.join($targetdir, "upgrade_bundle.zip")
        Jake.zip_upgrade_bundle( $bindir, zip_file_path)
    end

    task :check_update => ["config:iphone"] do
      mkdir_p File.join($app_path, 'development')
      RhoDevelopment.setup(File.join($app_path, 'development'), 'iphone')
      is_require_full_update = RhoDevelopment.is_require_full_update
      result = RhoDevelopment.check_changes_from_last_build(File.join($app_path, 'upgrade_package_add_files.txt'), File.join($app_path, 'upgrade_package_remove_files.txt'))
      puts '$$$ RhoDevelopment.is_require_full_update() = '+is_require_full_update.to_s
      puts '$$$ RhoDevelopment.check_changes_from_last_build() = '+result.class.to_s
    end

    desc "build upgrade package with part of bundle (changes configure by two text files - see documentation)"
    task :upgrade_package_partial => ["config:set_iphone_platform", "config:common"] do

        print_timestamp('build:iphone:upgrade_package_partial START')


        $skip_checking_XCode = true
        Rake::Task['config:iphone'].invoke


        #puts '$$$$$$$$$$$$$$$$$$'
        #puts 'targetdir = '+$targetdir.to_s
        #puts 'bindir = '+$bindir.to_s

        appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
        appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

        iphone_project = File.join($app_path, "project","iphone","#{appname_fixed}.xcodeproj")

        if !File.exist?(iphone_project)

          puts '$ make iphone XCode project for application'
          Rake::Task['build:iphone:make_xcode_project'].invoke

        end


        $skip_build_rhodes_main = true
        $skip_build_extensions = true
        $skip_build_xmls = true
        $use_prebuild_data = true

        Rake::Task['build:iphone:rhobundle'].invoke

        # process partial update

        add_list_full_name = File.join($app_path, 'upgrade_package_add_files.txt')
        remove_list_full_name = File.join($app_path, 'upgrade_package_remove_files.txt')

        src_folder = File.join($bindir, 'RhoBundle')
        src_folder = File.join(src_folder, 'apps')

        tmp_folder = $bindir + '_tmp_partial'
        rm_rf tmp_folder if File.exists? tmp_folder
        mkdir_p tmp_folder

        dst_tmp_folder = File.join(tmp_folder, 'RhoBundle')
        mkdir_p dst_tmp_folder
        # copy all
        cp_r src_folder, dst_tmp_folder

        dst_tmp_folder = File.join(dst_tmp_folder, 'apps')
        mkdir_p dst_tmp_folder

        add_files = []
        if File.exists? add_list_full_name
           File.open(add_list_full_name, "r") do |f|
              while line = f.gets
                 fixed_path = line.gsub('.rb', '.iseq').gsub('.erb', '_erb.iseq').chomp
                 add_files << fixed_path
                 puts '### ['+fixed_path+']'
              end
           end
        end

        remove_files = []
        if File.exists? remove_list_full_name
           File.open(remove_list_full_name, "r") do |f|
              while line = f.gets
                 fixed_path = line.gsub('.rb', '.iseq').gsub('.erb', '_erb.iseq').chomp
                 remove_files << fixed_path
                 #puts '### ['+fixed_path+']'
              end
           end
        end

        psize = dst_tmp_folder.size+1
        Dir.glob(File.join(dst_tmp_folder, '**','*')).sort.each do |f|
          relpath = f[psize..-1]

          if File.file?(f)
             #puts '$$$ ['+relpath+']'
             if (not add_files.include?(relpath)) && (relpath != 'rhofilelist.txt')
                 rm_rf f
             end
          end
        end

        Jake.build_file_map( dst_tmp_folder, "upgrade_package_add_files.txt" )

        #if File.exists? add_list_full_name
        #   File.open(File.join(dst_tmp_folder, 'upgrade_package_add_files.txt'), "w") do |f|
        #      add_files.each do |j|
        #         f.puts "#{j}\tfile\t0\t0"
        #      end
        #   end
        #end

        if File.exists? remove_list_full_name
           File.open(File.join(dst_tmp_folder, 'upgrade_package_remove_files.txt'), "w") do |f|
              remove_files.each do |j|
                 f.puts "#{j}"
                 #f.puts "#{j}\tfile\t0\t0"
              end
           end
        end

        mkdir_p $targetdir if not File.exists? $targetdir
        zip_file_path = File.join($targetdir, "upgrade_bundle_partial.zip")
        Jake.zip_upgrade_bundle( tmp_folder, zip_file_path)
        rm_rf tmp_folder
        print_timestamp('build:iphone:upgrade_package_partial FINISH')

    end



    def check_for_rebuild( result_lib_file, patternlist_file )

      if patternlist_file == nil
         return true
      end

      if !File.exist?(result_lib_file) || !File.exist?(patternlist_file)
          return true
      end

      patterns = []
      File.new(patternlist_file,"r").read.each_line do |line|
         if line.size > 0
            patterns << line.tr("\n","")
         end
      end

      #patterns << '../../../../../../platform/shared/common/app_build_capabilities.h'

      list_of_files = []

      fillist = FileList.new
      patterns.each do |p|
         fillist.include(p)
      end

      fillist.each do |fil|
         list_of_files << fil
      end

      list_of_files << File.join( $startdir ,'platform','shared','common','app_build_capabilities.h')

      if !FileUtils.uptodate?(result_lib_file, list_of_files)
          return true

      end
      return false
    end

    def is_options_was_changed(options_hash, options_file)
        puts "      is_options_was_changed( "+options_hash.to_s+", "+options_file.to_s+")"
      if !File.exist?(options_file)
         File.open(options_file,"w") {|f| f.write(YAML::dump(options_hash)) }
         return true
      else
         saved_options = YAML.load_file(options_file)
         if saved_options.to_a != options_hash.to_a
            File.open(options_file,"w") {|f| f.write(YAML::dump(options_hash)) }
            return true
         end
      end
      return false
    end




    def run_build_for_extension(extpath, xcodeproject, xcodetarget, depfile)
      # only depfile is optional parameter !

      currentdir = Dir.pwd()
      Dir.chdir File.join(extpath, "platform","iphone")

      xcodeproject = File.basename(xcodeproject)
      if depfile != nil
         depfile = File.basename(depfile)
      end

      targetdir = ENV['TARGET_TEMP_DIR']
      tempdir = ENV['TEMP_FILES_DIR']
      rootdir = ENV['RHO_ROOT']
      xcodebuild = ENV['XCODEBUILD']
      configuration = ENV['CONFIGURATION']
      sdk = ENV['SDK_NAME']
      bindir = ENV['PLATFORM_DEVELOPER_BIN_DIR']
      sdkroot = ENV['SDKROOT']
      arch = ENV['ARCHS']
      gccbin = bindir + '/gcc-4.2'
      arbin = bindir + '/ar'
      #xcode_version = ENV['XCODE_VERSION_ACTUAL']
      xcode_version = get_xcode_version

      iphone_path = '.'

      simulator = sdk =~ /iphonesimulator/

      if configuration == 'Distribution'
         configuration = 'Release'
      end

      build_config_folder = configuration + '-' + ( simulator ? "iphonesimulator" : "iphoneos")
      result_lib_folder = File.join(iphone_path, 'build', build_config_folder)

      FileUtils.mkdir(File.join(iphone_path, "build") ) unless File.exist? File.join(iphone_path, "build")
      FileUtils.mkdir(File.join(iphone_path, "build", build_config_folder) ) unless File.exist? File.join(iphone_path, "build", build_config_folder)

      result_lib = result_lib_folder + '/lib'+xcodetarget+'.a'
      target_lib = targetdir + '/lib'+xcodetarget+'.a'

      puts "BEGIN build xcode extension : ["+extpath+"]"
      puts "      result_lib : ["+result_lib+"]"
      puts "      target_lib : ["+target_lib+"]"
      puts "      depfile : ["+depfile.to_s+"]"
      puts "      configuration : ["+configuration+"]"
      puts "      sdk : ["+sdk+"]"

      rm_rf target_lib

      check_f_r = check_for_rebuild(result_lib, depfile)
      puts "      check_for_rebuild = "+check_f_r.to_s

      check_configuration = {
          "configuration" => configuration,
          "sdk" => sdk,
          "xcode_version" => xcode_version,
          "is_jsapp" => $js_application.to_s,
          "is_nodejsapp" => $nodejs_application.to_s
      }

      is_opt_c = is_options_was_changed( check_configuration, File.join(result_lib_folder, "lastbuildoptions.yml"))
      puts "      is_options_was_changed = "+is_opt_c.to_s

      result_lib_exist = File.exist?(result_lib)
      puts "      result_lib_exist('"+result_lib+"') = "+result_lib_exist.to_s


      if check_f_r || is_opt_c || (!result_lib_exist)

          rm_rf result_lib

          puts "      we should rebuild because previous builded library is not actual or not exist !"

          # library bot found ! We should build it !
          puts "      build xcode extension !"

          #rm_rf 'build'
          rm_rf result_lib_folder

          args = ['build', '-target', xcodetarget, '-configuration', configuration, '-sdk', sdk, '-project', xcodeproject]

          additional_string = ''
          if simulator
              #args << '-arch'
              #args << 'i386'
              additional_string = ' ARCHS="i386 x86_64"'
          end

          require   rootdir + '/lib/build/jake.rb'

          ret = IPhoneBuild.run_and_trace(xcodebuild,args,{:rootdir => rootdir, :string_for_add_to_command_line => additional_string})

          # save configuration
          is_options_was_changed( check_configuration, File.join(result_lib_folder, "lastbuildoptions.yml"))

      else
          puts "      ssskip rebuild because previous builded library is still actual !"
      end

      cp result_lib,target_lib

      Dir.chdir currentdir
      puts "END build xcode extension : ["+extpath+"]"


    end




    def build_extension_lib(extpath, sdk, target_dir, xcodeproject, xcodetarget, depfile)

      puts "build extension START :" + extpath


      ENV["SDK_NAME"] ||= sdk
      sdk = ENV["SDK_NAME"]
      #puts "xcodeproject = "+xcodeproject.to_s
      #puts "xcodetarget = "+xcodetarget.to_s

      if sdk =~ /iphonesimulator/
        $sdkver = sdk.gsub(/iphonesimulator/,"")
        $sdkroot = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" + $sdkver + ".sdk"
      else
        $sdkver = sdk.gsub(/iphoneos/,"")
        $sdkroot = $devroot + "/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS" + $sdkver + ".sdk"
      end

      ENV['RHO_PLATFORM'] = 'iphone'
      simulator = sdk =~ /iphonesimulator/
      ENV["PLATFORM_DEVELOPER_BIN_DIR"] ||= $devroot + "/Platforms/" + ( simulator ? "iPhoneSimulator" : "iPhoneOS" ) +
        ".platform/Developer/usr/bin"
      ENV["SDKROOT"] = $sdkroot

      #ENV["SDKROOT"] = $xcode_sdk_dir if not $xcode_sdk_dir.nil?


      ENV["BUILD_DIR"] ||= $startdir + "/platform/iphone/build"
      ENV["TARGET_TEMP_DIR"] ||= target_dir
      ENV["TEMP_FILES_DIR"] ||= ENV["TARGET_TEMP_DIR"]

      ENV["ARCHS"] ||= simulator ? "i386 x86_64" : "armv7 armv7s arm64"
      ENV["RHO_ROOT"] = $startdir

      # added by dmitrys
      ENV["XCODEBUILD"] = $xcodebuild
      ENV["CONFIGURATION"] ||= $configuration



      build_script = File.join(extpath, 'build')


      if (xcodeproject != nil) and (xcodetarget != nil)

          run_build_for_extension(extpath, xcodeproject, xcodetarget, depfile)

      else

        # modify executable attribute
        if File.exists? build_script
            if !File.executable? build_script
                 #puts 'change executable attribute for build script in extension : '+build_script
                 begin
                     #File.chmod 0700, build_script
                     #puts 'executable attribute was writed for : '+build_script
                 rescue Exception => e
                     puts 'ERROR: can not change attribute for build script in extension ! Try to run build command with sudo: prefix.'
                 end
            else
                 puts 'build script in extension already executable : '+build_script
            end
            #puts '$$$$$$$$$$$$$$$$$$     START'
            currentdir = Dir.pwd()
            Dir.chdir extpath
            sh %{$SHELL ./build}
            #Jake.run32("$SHELL "+build_script)
            Dir.chdir currentdir
            #puts '$$$$$$$$$$$$$$$$$$     FINISH'
            #if File.executable? build_script
                 #puts Jake.run('./build', [], extpath)
                 #exit 1 unless $? == 0
            #else
            #     puts 'ERROR: build script in extension is not executable !'
            #end
        else
            puts 'build script in extension not found => pure ruby extension'
        end

      end

      puts "build extension FINISH :" + extpath


    end


    def build_extension_libs(sdk, target_dir)

      puts "extpaths: #{$app_config["extpaths"].inspect.to_s}"
      $stdout.flush
      $app_extensions_list.each do |ext, commin_ext_path |
          if commin_ext_path != nil

            print_timestamp('process extension "'+ext+'" START')

            #puts '########################  ext='+ext.to_s+'        path='+commin_ext_path.to_s
            extpath = File.join( commin_ext_path, 'ext')

            prebuilt_copy = false

            xcodeproject = nil  #xcodeproject
            xcodetarget = nil   #xcodetarget
            depfile = nil       #rebuild_deplist

            extyml = File.join(commin_ext_path,"ext.yml")
            if File.file? extyml
              extconf = Jake.config(File.open(extyml))
              extconf_iphone = extconf['iphone']
              exttype = 'build'
              exttype = extconf_iphone['exttype'] if extconf_iphone and extconf_iphone['exttype']

              xcodeproject = extconf_iphone['xcodeproject'] if extconf_iphone and extconf_iphone['xcodeproject']
              xcodetarget = extconf_iphone['xcodetarget'] if extconf_iphone and extconf_iphone['xcodetarget']
              depfile = extconf_iphone['rebuild_deplist'] if extconf_iphone and extconf_iphone['rebuild_deplist']

              if exttype == 'prebuilt'
                prebuiltpath = Dir.glob(File.join(extpath, '**', 'iphone'))
                if prebuiltpath.count > 0
                  prebuiltpath = prebuiltpath.first
                else
                  raise "iphone:exttype is 'prebuilt' but prebuilt path is not found #{prebuiltpath.inspect}"
                end

                libes = extconf["libraries"]
                if libes != nil
                  libname = libes.first
                  libpath = File.join(prebuiltpath, "lib"+libname+".a")
                  if File.file? libpath
                     targetlibpath = File.join(target_dir, "lib"+libname+".a")
                     cp libpath, targetlibpath
                     prebuilt_copy = true
                  else
                    raise "iphone:exttype is 'prebuilt' but lib path is not found #{libpath.inspect}"
                  end

                end
              end


            end
            if ! prebuilt_copy
              if $use_prebuild_data && (ext == 'coreapi')
                prebuild_root = File.join($startdir, "platform/iphone/prebuild")

                prebuild_ruby = File.join(prebuild_root, "ruby")
                prebuild_noruby = File.join(prebuild_root, "noruby")

                coreapi_ruby = File.join(prebuild_ruby, "libCoreAPI.a")
                coreapi_noruby = File.join(prebuild_noruby, "libCoreAPI.a")

                coreapi_lib = coreapi_ruby

                if $js_application
                  coreapi_lib = coreapi_noruby
                end
                if File.exist?(coreapi_lib)
                   coreapi_lib_dst = File.join(target_dir, 'libCoreAPI.a')
                   cp coreapi_lib, coreapi_lib_dst
                else
                   build_extension_lib(extpath, sdk, target_dir, xcodeproject, xcodetarget, depfile)
                end
              else
                build_extension_lib(extpath, sdk, target_dir, xcodeproject, xcodetarget, depfile)
              end
            end

            print_timestamp('process extension "'+ext+'" FINISH')

          end
      end

    end

    #[build:iphone:extension_libs]
    task :extension_libs => ["config:iphone", "build:bundle:noxruby"] do
      #chdir 'platform/iphone'
      chdir File.join($app_path, 'project/iphone')
      rm_rf 'bin'
      rm_rf 'build/Debug-*'
      rm_rf 'build/Release-*'

      chdir $startdir

      if $sdk =~ /iphonesimulator/
        $sdkver = $sdk.gsub(/iphonesimulator/,"")
      else
        $sdkver = $sdk.gsub(/iphoneos/,"")
      end

      if $configuration.to_s.downcase == "release"
        $confdir = "release"
      else
        $confdir = "debug"
      end
      $app_builddir = File.join($targetdir, $confdir)

      simsdk = "iphonesimulator"+$sdkver
      devsdk = "iphoneos"+$sdkver

      sim_target_dir = $startdir + "/platform/iphone/build/rhorunner.build/#{$configuration}-" +
          "iphonesimulator" + "/rhorunner.build"
      dev_target_dir = $startdir + "/platform/iphone/build/rhorunner.build/#{$configuration}-" +
          "iphoneos" + "/rhorunner.build"


      puts "extpaths: #{$app_config["extpaths"].inspect.to_s}"
      $stdout.flush
      $app_extensions_list.each do |ext, commin_ext_path |
        if commin_ext_path != nil

          extpath = File.join( commin_ext_path, 'ext')
          extyml = File.join( commin_ext_path, "ext.yml")

          xcodeproject = nil  #xcodeproject
          xcodetarget = nil   #xcodetarget
          depfile = nil       #rebuild_deplist

          if File.file? extyml
            extconf = Jake.config(File.open(extyml))
            #extconf_iphone = extconf['iphone']
            exttype = 'build'
            extconf_iphone = extconf['iphone']
            exttype = extconf_iphone['exttype'] if extconf_iphone and extconf_iphone['exttype']

            xcodeproject = extconf_iphone['xcodeproject'] if extconf_iphone and extconf_iphone['xcodeproject']
            xcodetarget = extconf_iphone['xcodetarget'] if extconf_iphone and extconf_iphone['xcodetarget']
            depfile = extconf_iphone['rebuild_deplist'] if extconf_iphone and extconf_iphone['rebuild_deplist']

            libes = extconf["libraries"]
            if (libes != nil) && (exttype != 'prebuilt')
              libname = libes.first
              prebuiltpath = Dir.glob(File.join(extpath, '**', 'iphone'))
              if prebuiltpath != nil && prebuiltpath.count > 0

                print_timestamp('build extension "'+ext+'" START')


                 prebuiltpath = prebuiltpath.first

                 libpath = File.join(prebuiltpath, "lib"+libname+".a")
                 libsimpath = File.join(prebuiltpath, "lib"+libname+"386.a")
                 libdevpath = File.join(prebuiltpath, "lib"+libname+"ARM.a")
                 libbinpath = File.join($app_builddir, "extensions", ext, "lib", "lib"+libname+".a")

                 ENV["TARGET_TEMP_DIR"] = prebuiltpath
                 ENV["ARCHS"] = "i386 x86_64"
                 ENV["SDK_NAME"] = simsdk

                 build_extension_lib(extpath, simsdk, prebuiltpath, xcodeproject, xcodetarget, depfile)
                 cp libpath, libsimpath
                 rm_f libpath

                 ENV["ARCHS"] = nil
                 ENV["SDK_NAME"] = devsdk
                 build_extension_lib(extpath, devsdk, prebuiltpath, xcodeproject, xcodetarget, depfile)
                 cp libpath, libdevpath
                 rm_f libpath

                 args = []
                 args << "-create"
                 args << "-output"
                 args << libpath
                 args << libsimpath
                 args << libdevpath
                 IPhoneBuild.run_and_trace("lipo",args,{:rootdir => $startdir})

                 mkdir_p File.join($app_builddir, "extensions", ext, "lib")
                 cp libpath, libbinpath

                 rm_f libsimpath
                 rm_f libdevpath

                 # copy all files from extension folder to extension binary folder
                 Dir.glob(File.join(commin_ext_path,'*')).each do |artefact|
                    if (artefact != extpath) && (artefact != extyml) && ((File.file? artefact) || (File.directory? artefact))
                        cp_r artefact, File.join($app_builddir, "extensions", ext)
                    end
                 end
                print_timestamp('build extension "'+ext+'" FINISH')

              end
            end
          end
        end
      end


    end

    #[build:iphone:extensions]
    task :extensions => "config:iphone" do
      print_timestamp('build:iphone:extensions START')

      simulator = $sdk =~ /iphonesimulator/
      target_dir = ''
      if ENV["TARGET_TEMP_DIR"] and ENV["TARGET_TEMP_DIR"] != ""
         target_dir = ENV["TARGET_TEMP_DIR"]
      else
         target_dir = File.join($app_path, "/project/iphone") + "/build/rhorunner.build/#{$configuration}-" +
            ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
      end

      build_extension_libs($sdk, target_dir)

      print_timestamp('build:iphone:extensions FINISH')

    end




    def build_rhodes_lib(targetdir)
      # only depfile is optional parameter !


      rhodes_xcode_project_folder = File.join($startdir, '/platform/iphone/')

      currentdir = Dir.pwd()
      Dir.chdir File.join(rhodes_xcode_project_folder)

      xcodeproject = 'Rhodes.xcodeproj'
      xcodetarget = 'Rhodes'

      #tempdir = ENV['TEMP_FILES_DIR']
      #rootdir = ENV['RHO_ROOT']
      configuration = 'Release'
      sdk = $sdk
      #bindir = ENV['PLATFORM_DEVELOPER_BIN_DIR']
      #sdkroot = ENV['SDKROOT']
      #arch = ENV['ARCHS']
      #gccbin = bindir + '/gcc-4.2'
      #arbin = bindir + '/ar'

      iphone_path = '.'

      simulator = sdk =~ /iphonesimulator/

      if configuration == 'Distribution'
         configuration = 'Release'
      end

      result_lib = iphone_path + '/build/' + configuration + '-' + ( simulator ? "iphonesimulator" : "iphoneos") + '/lib'+xcodetarget+'.a'
      target_lib = targetdir + '/lib'+xcodetarget+'.a'

      #if check_for_rebuild(result_lib, depfile) || is_options_was_changed({"configuration" => configuration,"sdk" => sdk}, "lastbuildoptions.yml")

           rm_rf 'build'
           rm_rf target_lib

           args = ['build', '-target', xcodetarget, '-configuration', configuration, '-sdk', sdk, '-project', xcodeproject, '-quiet']

           additional_string = ''
           if simulator
           #    args << '-arch'
           #    args << 'i386 x86_64'
               additional_string = ' ARCHS="i386 x86_64"'
           end

           require   $startdir + '/lib/build/jake.rb'

           ret = IPhoneBuild.run_and_trace($xcodebuild,args,{:rootdir => $startdir, :string_for_add_to_command_line => additional_string})
      #else
      #
      #  puts "ssskip rebuild because previous builded library is still actual !"
      #  rm_rf target_lib
      #
      #end

      cp result_lib,target_lib

      Dir.chdir currentdir


    end






    #[build:iphone:make_prebuild_core]
    task :make_prebuild_core do

      currentdir = Dir.pwd()

      # remove prebuild folder
      prebuild_root = File.join($startdir, "platform/iphone/prebuild")

      rm_rf prebuild_root

      prebuild_ruby = File.join(prebuild_root, "ruby")
      prebuild_noruby = File.join(prebuild_root, "noruby")

      mkdir_p prebuild_ruby
      mkdir_p prebuild_noruby


      coreapi_ruby = File.join(prebuild_ruby, "libCoreAPI.a")
      coreapi_noruby = File.join(prebuild_noruby, "libCoreAPI.a")

      prebuild_base_app_folder = File.join($startdir, "res/prebuild_base_app");

      prebuild_base_app_build_yml = File.join(prebuild_base_app_folder, "build.yml")

      coreapi_root = File.join($startdir, "lib/commonAPI/coreapi")


      #$app_config = YAML::load_file(prebuild_base_app_build_yml)
      $app_config = Jake.config(File.open(prebuild_base_app_build_yml))

      $app_config_disable_reread = true

      puts "%%% $app_config[javascript_application] = "+$app_config["javascript_application"].to_s

      $app_path = File.expand_path(prebuild_base_app_folder)

      chdir File.join($startdir)

      $app_extensions_list = {}
      $app_extensions_list['coreapi'] = coreapi_root

      #ruby

      rm_rf File.join(prebuild_base_app_folder, 'project')
      Rake::Task['clean:iphone'].invoke

      #$app_config["javascript_application"] = "false"


      Rake::Task['config:common'].invoke
      Rake::Task['config:iphone'].invoke

      $js_application = false

      Rake::Task['build:bundle:prepare_native_generated_files'].invoke



      #coreAPI
      Rake::Task['build:iphone:extension_libs'].invoke

      #Rhodes

      build_rhodes_lib(prebuild_ruby)



      #copy

      cp File.join(prebuild_base_app_folder, "bin/target/iOS/release/extensions/coreapi/lib/libCoreAPI.a"), coreapi_ruby

      #Rhodes





      #noruby
      rm_rf File.join(prebuild_base_app_folder, 'project')
      $app_config['javascript_application'] = 'true'

      puts "%%% $app_config[javascript_application] = "+$app_config["javascript_application"].to_s


      Rake::Task['clean:iphone'].reenable
      Rake::Task['config:common'].reenable
      Rake::Task['config:iphone'].reenable
      Rake::Task['build:bundle:prepare_native_generated_files'].reenable
      Rake::Task['build:iphone:extension_libs'].reenable

      #Rake::Task['clean:iphone'].invoke

      #Rake::Task['config:common'].invoke

      #Rake::Task['config:iphone'].invoke

      $js_application = true


      Rake::Task['build:bundle:prepare_native_generated_files'].invoke

      #coreAPI

      Rake::Task['build:iphone:extension_libs'].invoke


      #Rhodes

      build_rhodes_lib(prebuild_noruby)

      #copy
      #copy

      cp File.join(prebuild_base_app_folder, "bin/target/iOS/release/extensions/coreapi/lib/libCoreAPI.a"), coreapi_noruby




      Dir.chdir currentdir

    end



    def copy_generated_sources_and_binaries
      #copy sources
      extensions_src = File.join($startdir, 'platform','shared','ruby','ext','rho','extensions.c')
      extensions_dst = File.join($app_path, 'project','iphone','Rhodes','extensions.c')

      #rm_rf extensions_dst
      #cp extensions_src, extensions_dst
      Jake.copyIfNeeded extensions_src, extensions_dst

      properties_src = File.join($startdir, 'platform','shared','common','app_build_configs.c')
      properties_dst = File.join($app_path, 'project','iphone','Rhodes','app_build_configs.c')

      #rm_rf properties_dst
      #cp properties_src, properties_dst
      Jake.copyIfNeeded properties_src, properties_dst


      # old code for use prebuild libraries:

      #copy libCoreAPI.a and Rhodes.a
      #prebuild_root = File.join($startdir, "platform","iphone","prebuild")

      #prebuild_ruby = File.join(prebuild_root, "ruby")
      #prebuild_noruby = File.join(prebuild_root, "noruby")

      #coreapi_ruby = File.join(prebuild_ruby, "libCoreAPI.a")
      #coreapi_noruby = File.join(prebuild_noruby, "libCoreAPI.a")

      #rhodes_ruby = File.join(prebuild_ruby, "libRhodes.a")
      #rhodes_noruby = File.join(prebuild_noruby, "libRhodes.a")

      #coreapi_lib = coreapi_ruby
      #rhodes_lib = rhodes_ruby

      #if $js_application
    #    coreapi_lib = coreapi_noruby
    #    rhodes_lib = rhodes_noruby
     # end
      #if File.exist?(coreapi_lib)
      #   coreapi_lib_dst = File.join($app_path, 'project/iphone/Rhodes/libCoreAPI.a')
      #   rm_rf coreapi_lib_dst
      #   cp coreapi_lib, coreapi_lib_dst
      #end
      #if File.exist?(rhodes_lib)
    #     rhodes_lib_dst = File.join($app_path, 'project/iphone/Rhodes/libRhodes.a')
    #     rm_rf rhodes_lib_dst
    #     cp rhodes_lib, rhodes_lib_dst
     # end

    end


    #[build:iphone:setup_xcode_project]
    desc "make/change generated XCode project for build application"
    task :setup_xcode_project => ["config:iphone"] do
      print_timestamp('build:iphone:setup_xcode_project START')
      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

      iphone_project = File.join($app_path, "project","iphone","#{appname_fixed}.xcodeproj")

      xcode_project_checker = GeneralTimeChecker.new
      xcode_project_checker.init($startdir, $app_path, "xcode_project")

      if !File.exist?(iphone_project)

        puts '$ make iphone XCode project for application'
        Rake::Task['build:iphone:make_xcode_project'].invoke

      else

        chdir $app_path

        build_yml_filepath = File.join($app_path, "build.yml")
        build_yml_updated = xcode_project_checker.check(build_yml_filepath)

        if build_yml_updated

            puts '$ prepare iphone XCode project for application'
            rhogenpath = File.join($startdir, 'bin', 'rhogen')
            Jake.run32("\"#{rhogenpath}\" iphone_project #{appname_fixed} \"#{$startdir}\"")

            Rake::Task['build:iphone:update_plist'].invoke

            if !$skip_build_xmls
              Rake::Task['build:bundle:prepare_native_generated_files'].invoke
              rm_rf File.join('project','iphone','toremoved')
              rm_rf File.join('project','iphone','toremovef')
            end

            update_xcode_project_files_by_capabilities

        else
            puts "$ build.yml not changed after last XCode project generation !"
        end


      end

      copy_generated_sources_and_binaries

      xcode_project_checker.update

      print_timestamp('build:iphone:setup_xcode_project FINISH')

    end


    #[build:iphone:update_plist]
    task :update_plist => ["config:iphone"] do

      chdir $app_path

      puts 'update info.plist'

      update_plist_procedure

      set_signing_identity($signidentity,$provisionprofile,$entitlements,$provisioning_style,$development_team) #if $signidentity.to_s != ""
    end

    #[build:iphone:make_xcode_project]
    task :make_xcode_project => ["config:iphone"] do

      print_timestamp('build:iphone:make_xcode_project START')
      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

      chdir $app_path

      rm_rf File.join('project','iphone')

      puts 'prepare iphone XCode project for application'
      rhogenpath = File.join($startdir, 'bin', 'rhogen')
      if $use_prebuild_data
        Jake.run32("\"#{rhogenpath}\" iphone_project_prebuild #{appname_fixed} \"#{$startdir}\"")
      else
        Jake.run32("\"#{rhogenpath}\" iphone_project #{appname_fixed} \"#{$startdir}\"")
      end
      #make_project_bakup

      #restore_project_from_bak

      #fix issue with Application Base generated file - hardcoded !!!
      #xml_path = File.join($startdir, "/lib/commonAPI/coreapi/ext/Application.xml")
      #Jake.run3("\"#{$startdir}/bin/rhogen\" api \"#{xml_path}\"")

      update_plist_procedure


      if $entitlements == ""
          if $configuration == "Distribution"
              $entitlements = "Entitlements.plist"
          end
      end

      set_signing_identity($signidentity,$provisionprofile,$entitlements,$provisioning_style,$development_team) #if $signidentity.to_s != ""
      copy_entitlements_file_from_app

      Rake::Task['build:bundle:prepare_native_generated_files'].invoke


      rm_rf File.join('project','iphone','toremoved')
      rm_rf File.join('project','iphone','toremovef')

      update_xcode_project_files_by_capabilities

      copy_generated_sources_and_binaries

      print_timestamp('build:iphone:make_xcode_project FINISH')

    end

    #[build:iphone:rhodes]
    task :rhodes => "config:iphone" do

       appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
       appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

       Rake::Task['build:iphone:setup_xcode_project'].invoke

       Rake::Task['build:iphone:rhodes_old'].invoke

    end

    #[build:iphone:rhodes_old]
#    desc "Build rhodes"
    task :rhodes_old => ["config:iphone", "build:iphone:rhobundle"] do
      print_timestamp('build:iphone:rhodes START')
      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")
      appname_project = appname_fixed.slice(0, 1).capitalize + appname_fixed.slice(1..-1) + ".xcodeproj"

      #saved_name = ''
      #saved_version = ''
      #saved_identifier = ''
      #saved_url_scheme = ''
      #saved_url_name = ''

      #saved_name = set_app_name($app_config["name"]) unless $app_config["name"].nil?
      #saved_version = set_app_version($app_config["version"]) unless $app_config["version"].nil?


      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      #vendor = $app_config['vendor'] ? $app_config['vendor'] : "rhomobile"
      #bundle_identifier = "com.#{vendor}.#{appname}"
      #bundle_identifier = $app_config["iphone"]["BundleIdentifier"] unless $app_config["iphone"]["BundleIdentifier"].nil?
      #saved_identifier = set_app_bundle_identifier(bundle_identifier)
      # Set UIApplicationExitsOnSuspend.
      on_suspend = $app_config["iphone"]["UIApplicationExitsOnSuspend"]
      on_suspend_value = false

      if on_suspend.nil?
        puts "UIApplicationExitsOnSuspend not configured, using default of false"
      elsif on_suspend.to_s.downcase == "true" || on_suspend.to_s == "1"
        on_suspend_value = true
      elsif on_suspend.to_s.downcase == "false" || on_suspend.to_s == "0"
        on_suspend_value = false
      else
        raise "UIApplicationExitsOnSuspend is not set to a valid value. Current value: '#{$app_config["iphone"]["UIApplicationExitsOnSuspend"]}'"
      end

      update_plist_block($app_path + "/project/iphone/Info.plist") do |hash|
        hash['UIApplicationExitsOnSuspend'] = on_suspend_value
      end

      #icon_has_gloss_effect = $app_config["iphone"]["IconHasGlossEffect"] unless $app_config["iphone"]["IconHasGlossEffect"].nil?
      #icon_has_gloss_effect = set_ui_prerendered_icon(icon_has_gloss_effect)

      #saved_url_scheme = set_app_url_scheme($app_config["iphone"]["BundleURLScheme"]) unless $app_config["iphone"]["BundleURLScheme"].nil?
      #saved_url_name = set_app_url_name(bundle_identifier)

      #set_app_icon(true)
      #set_default_images(true)

      if $entitlements == ""
          if $configuration == "Distribution"
              $entitlements = "Entitlements.plist"
          end
      end

      #set_signing_identity($signidentity,$provisionprofile,$entitlements.to_s) if $signidentity.to_s != ""
      #copy_entitlements_file_from_app

      #chdir $config["build"]["iphonepath"]
      chdir File.join($app_path, "project","iphone")

      args = ['build', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk, '-project', appname_project, '-quiet']

      additional_string = ''
      if $sdk =~ /iphonesimulator/
      #   args << '-arch'
      #   args << 'i386'
        additional_string = ' ARCHS="i386 x86_64"'
      end

      ret = 0

      if !$skip_build_rhodes_main
        ret = IPhoneBuild.run_and_trace($xcodebuild,args,{:rootdir => $startdir, :string_for_add_to_command_line => additional_string})
      end


      ENV["RHO_BUNDLE_ALREADY_BUILDED"] = "NO"

      chdir $startdir

      #set_app_name(saved_name) unless $app_config["name"].nil?
      #set_app_version(saved_version) unless $app_config["version"].nil?
      #set_app_bundle_identifier(saved_identifier) unless $app_config["iphone"]["BundleIdentifier"].nil?
      #set_app_url_scheme(saved_url_scheme) unless $app_config["iphone"]["BundleURLScheme"].nil?
      #set_app_url_name(saved_url_name) unless $app_config["iphone"]["BundleIdentifier"].nil?
      #set_ui_prerendered_icon(icon_has_gloss_effect)

      # Set UIApplicationExitsOnSuspend.
      #if $app_config["iphone"]["UIApplicationExitsOnSuspend"].nil?
      #  puts "UIApplicationExitsOnSuspend not configured, using default of false"
      #  set_app_exit_on_suspend(false)
      #elsif $app_config["iphone"]["UIApplicationExitsOnSuspend"].to_s.downcase == "true" || $app_config["iphone"]["UIApplicationExitsOnSuspend"].to_s == "1"
      #  set_app_exit_on_suspend(true)
      #elsif $app_config["iphone"]["UIApplicationExitsOnSuspend"].to_s.downcase == "false" || $app_config["iphone"]["UIApplicationExitsOnSuspend"].to_s == "0"
      #  set_app_exit_on_suspend(false)
      #else
      #  raise "UIApplicationExitsOnSuspend is not set to a valid value. Current value: '#{$app_config["iphone"]["UIApplicationExitsOnSuspend"]}'"
      #end

      #restore_entitlements_file
      #restore_default_images
      #restore_app_icon

      unless ret == 0
        puts '************************************'
        puts "ERROR during building by XCode !"
        puts 'XCode return next error code = '+ret.to_s
        exit 1
      end

      print_timestamp('build:iphone:rhodes FINISH')

    end

  end
end

namespace "run" do
  namespace "iphone" do
    task :build => 'run:buildsim'
    task :debug => :run

    #[run:iphone]
    task :run => 'config:iphone' do

      print_timestamp('run:iphone:run START')

      mkdir_p $tmpdir
      log_name  =   File.join($app_path, 'rholog.txt')
      File.delete(log_name) if File.exist?(log_name)

      rhorunner = File.join(File.join($app_path, "/project/iphone"),"build/#{$configuration}-iphonesimulator/rhorunner.app")
      commandis = $iphonesim + ' launch "' + rhorunner + '" ' + $sdkver.gsub(/([0-9]\.[0-9]).*/,'\1') + ' ' + $emulatortarget + ' "' +log_name+'"'

      kill_iphone_simulator


      $ios_run_completed = false


      #thr = Thread.new do
      #end
      #Thread.new {
      thr = Thread.new do
        puts 'start thread with execution of application'
        #if ($emulatortarget != 'iphone') && ($emulatortarget != 'ipad')
        #  puts  'use old execution way - just open iPhone Simulator'
        #  system("open \"#{$sim}/iPhone Simulator.app\"")
        #  $ios_run_completed = true
        #  sleep(1000)
        #else
          puts 'use iphonesim tool - open iPhone Simulator and execute our application, also support device family (iphone/ipad)'
          puts 'Execute command: '+commandis
          system(commandis)
          $ios_run_completed = true
          sleep(1000)
        #end
        #}
      end

      print_timestamp('application was launched in simulator')
      #if ($emulatortarget != 'iphone') && ($emulatortarget != 'ipad')
       #    thr.join
      #else
        puts 'start waiting for run application in Simulator'
        while (!File.exist?(log_name)) && (!$ios_run_completed)
          puts ' ... still waiting'
          sleep(1)
        end
        puts 'stop waiting - application started'
        #sleep(1000)
        thr.kill
        #thr.join
        puts 'application is started in Simulator'
        exit
      #end
      print_timestamp('run:iphone FINISH')
      puts "end build iphone app"
      exit
    end

    #[run:iphone:spec]
    task :spec => ["clean:iphone"] do
    #task :spec do
      is_timeout = false
      is_correct_stop = false
      Jake.decorate_spec do
          Rake::Task['run:buildsim'].invoke

          # Run local http server
          $iphonespec = true
          #httpserver = false
          #httpserver = true if File.exist? "#{$app_path}/app/spec/library/net/http/http/fixtures/http_server.rb"
          #if httpserver
          #  require "#{$app_path}/app/spec/library/net/http/http/fixtures/http_server"
          #  NetHTTPSpecs.start_server
          #end

          $dont_exit_on_failure = false

          Jake.before_run_spec
          kill_iphone_simulator

          mkdir_p $tmpdir
          #log_name  =   File.join($tmpdir, 'logout')
          log_name = File.join($app_path, 'rholog.txt')
          File.delete(log_name) if File.exist?(log_name)

          $iphone_end_spec = false
          Thread.new {
            # run spec
            rhorunner = File.join(File.join($app_path, "project","iphone"),"build","#{$configuration}-iphonesimulator","rhorunner.app")
            #iphonesim = File.join($startdir, 'res/build-tools/iphonesim/build/Release/iphonesim')
            commandis = $iphonesim + ' launch "' + rhorunner + '" ' + $sdkver.gsub(/([0-9]\.[0-9]).*/,'\1') + ' ' + $emulatortarget + ' "' +log_name+'"'
            puts 'use iphonesim tool - open iPhone Simulator and execute our application, also support device family (iphone/ipad) '
            puts 'execute command : ' + commandis
            system(commandis)
            #$iphone_end_spec = true
          }

          start = Time.now
          puts "Waiting for iphone app log ..."
          while (!File.exist?(log_name)) && (!$iphone_end_spec)
            sleep(1)
          end

          timeout_in_seconds = 30*60

          log_lines = []
          last_spec_line = ""
          last_spec_iseq_line = ""


          start_logging = Time.now

          puts "Start reading log ..."
          File.open(log_name, 'r:UTF-8') do |io|
            while !$iphone_end_spec do
              io.each do |line|
                line = line.force_encoding('ASCII-8BIT')
                $logger.debug line
                log_lines << line
                if line.class.method_defined? "valid_encoding?"
                  $iphone_end_spec = !Jake.process_spec_output(line) if line.valid_encoding?
                else
                  $iphone_end_spec = !Jake.process_spec_output(line)
                end
                # FIXME: Workaround to avoid endless loop in the case of System.exit
                # seg. fault: (SEGV received in SEGV handler)
                # Looking at log end marker from mspec runner
                is_mspec_stop = line =~ /MSpec runner stopped/
                is_terminated = line =~ /\| \*\*\*Terminated\s+(.*)/

                is_correct_stop = true if is_mspec_stop || is_terminated

                $iphone_end_spec = true if is_mspec_stop


                last_spec_line = line if line =~ /_spec/
                last_spec_iseq_line = line if line =~ /_spec.iseq/

                #check for timeout
                if (Time.now.to_i - start_logging.to_i) > timeout_in_seconds
                    $iphone_end_spec = true
                    is_timeout = true
                end
                if $iphone_end_spec
                    puts "%%% stop spec by this line : ["+line.to_s+"]"
                end
                break if $iphone_end_spec
              end
              sleep(3) unless $iphone_end_spec
            end
          end

          puts "Processing spec results ..."
          Jake.process_spec_results(start)
          if is_timeout || !is_correct_stop
              puts "Tests has issues : is_timeout["+is_timeout.to_s+"], timeout["+timeout_in_seconds.to_s+" sec], not_correct_terminated_line["+(!is_correct_stop).to_s+"] !"
              puts "Tests stoped by timeout ( "+timeout_in_seconds.to_s+" sec ) !"
              puts "last_spec_line = ["+last_spec_line.to_s+"]"
              puts "last_spec_iseq_line = ["+last_spec_iseq_line.to_s+"]"
              puts "last spec executed = ["+$latest_test_line.to_s+"]"
              puts "This is last 1024 lines from log :"
              idx = log_lines.size-1024
              if idx < 0
                  idx = 0
              end
              while idx < log_lines.size
                  puts "line ["+idx.to_s+"]: "+log_lines[idx]
                  idx = idx + 1
              end
          end

          #File.delete(log_name) if File.exist?(log_name)
          # kill_iphone_simulator
          $stdout.flush
      end

      unless $dont_exit_on_failure
        exit 1 if is_timeout
        exit 1 if $total.to_i==0
        exit 1 if !is_correct_stop
        exit $failed.to_i
      end
    end

    # Alias for above task
    task "emulator:spec" => :spec

    task :spec_old => ["clean:iphone",:buildsim] do

      sdkroot = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" +
                $sdk.gsub(/iphonesimulator/,"") + ".sdk"

      old_user_home = ENV["CFFIXED_USER_HOME"]
      old_dyld_root = ENV["DYLD_ROOT_PATH"]
      old_dyld_framework = ENV["DYLD_FRAMEWORK_PATH"]
      old_iphone_simulator = ENV["IPHONE_SIMULATOR_ROOT"]

      ENV["CFFIXED_USER_HOME"] = $simrhodes
      ENV["DYLD_ROOT_PATH"] = sdkroot
      ENV["DYLD_FRAMEWORK_PATH"] = sdkroot + "/System/Library/Frameworks"
      ENV["IPHONE_SIMULATOR_ROOT"] = sdkroot

      command = '"' + $simrhodes + '/rhorunner.app/rhorunner"' + " -RegisterForSystemEvents"

      #if someone runs against the wrong app, kill after 120 seconds
      Thread.new {
        sleep 300
        `killall -9 rhorunner`
      }

      `killall -9 rhorunner`

      # Run local http server
      $iphonespec = true
      httpserver = false
      httpserver = true if File.exist? "#{$app_path}/app/spec/library/net/http/http/fixtures/http_server.rb"

      if httpserver
        require "#{$app_path}/app/spec/library/net/http/http/fixtures/http_server"
        NetHTTPSpecs.start_server
      end

      Jake.before_run_spec

      start = Time.now
      io = IO.popen(command)
      io.each do |line|
        Jake.process_spec_output(line)
      end

      Jake.process_spec_results(start)

      $stdout.flush

      NetHTTPSpecs.stop_server if httpserver

      ENV["CFFIXED_USER_HOME"] = old_user_home
      ENV["DYLD_ROOT_PATH"] = old_dyld_root
      ENV["DYLD_FRAMEWORK_PATH"] = old_dyld_framework
      ENV["IPHONE_SIMULATOR_ROOT"] = old_iphone_simulator
      unless $dont_exit_on_failure
        exit 1 if $total.to_i==0
        exit $failed.to_i
      end
    end

    rhosim_task = lambda do |name, &block|
      task name => ["config:set_iphone_platform", "config:common"] do
        $rhosim_config = "platform='iphone'\r\n"
        block.()
      end
    end

    desc "Run application on RhoSimulator"
    rhosim_task.(:rhosimulator) { Rake::Task["run:rhosimulator"].invoke }
    namespace :rhosimulator do
      rhosim_task.(:build) { Rake::Task["run:rhosimulator:build"].invoke         }
      rhosim_task.(:debug) { Rake::Task["run:rhosimulator:run"  ].invoke('wait') }
    end

    task :get_log => ["config:iphone"] do
      #puts $simapppath
      # $sdkver = $emulator_version.to_s unless $emulator_version.nil?
      #
      # simapp = File.join($simdir, $sdkver, "Applications")
      #
      # Dir.glob(File.join($simdir, $sdkver, "Applications", "*")).each do |simapppath|
      #     need_rm = true if File.directory? simapppath
      #     if File.exists?(File.join(simapppath, 'rhorunner.app', 'name'))
      #       name = File.read(File.join(simapppath, 'rhorunner.app', 'name'))
      #       puts "found app name: #{name}"
      #       guid = File.basename(simapppath)
      #       puts "found guid: #{guid}"
      #       if name == $app_config['name']
      #           $guid = guid
      #           need_rm = false
      #       end
      #   end
      #   rm_rf simapppath if need_rm
      #   rm_rf simapppath + ".sb" if need_rm
      # end
      #
      # simapp = File.join($simdir, $emulator_version, "Applications")
      #
      #
      # rholog = simapp + "/" + $guid + "/Library/Caches/Private Documents/rholog.txt"
      log_name  =   File.join($app_path, 'rholog.txt')
      puts "log_file=" + log_name
    end

    #[run:iphone:simulator:package]
    namespace "simulator" do
      desc "run IPA package on simulator"
      task :package, [:package_path] => ["config:iphone"] do |t, args|

        currentdir = Dir.pwd()


        package_path = args[:package_path]
        #unpack package

        tmp_dir  =   File.join($tmpdir, 'launch_package')
        rm_rf tmp_dir
        mkdir_p tmp_dir

        #cp package_path, File.join(tmp_dir, 'package.ipa')
        Jake.run('unzip', [package_path, '-d', tmp_dir])

        Dir.chdir File.join(tmp_dir, 'Payload')

        app_file = nil

        Dir::glob(File.join(tmp_dir, 'Payload', '*.app')).each { |x| app_file = x }

        if app_file == nil
          raise 'can not find *.app folder inside package !'
        end

        puts '$$$ founded app folder is ['+app_file+']'

        log_name  =   File.join($app_path, 'rholog.txt')
        File.delete(log_name) if File.exist?(log_name)

        commandis = $iphonesim + ' launch "' + app_file + '" ' + $sdkver.gsub(/([0-9]\.[0-9]).*/,'\1') + ' ' + $emulatortarget + ' "' +log_name+'"'

        kill_iphone_simulator

        $ios_run_completed = false

        thr = Thread.new do
           puts 'start thread with execution of application'
           #if ($emulatortarget != 'iphone') && ($emulatortarget != 'ipad')
            #   puts  'use old execution way - just open iPhone Simulator'
            #   system("open \"#{$sim}/iPhone Simulator.app\"")
            #   $ios_run_completed = true
            #   sleep(1000)
           #else
               puts 'use iphonesim tool - open iPhone Simulator and execute our application, also support device family (iphone/ipad)'
               puts 'Execute command: '+commandis
               system(commandis)
               $ios_run_completed = true
               sleep(1000)
           #end
        #}
        end

        #if ($emulatortarget != 'iphone') && ($emulatortarget != 'ipad')
         #  thr.join
        #else
           puts 'start waiting for run application in Simulator'
           while (!File.exist?(log_name)) && (!$ios_run_completed)
              puts ' ... still waiting'
              sleep(1)
           end
           puts 'stop waiting - application started'
           #sleep(1000)
           thr.kill
           #thr.join
           puts 'application is started in Simulator'
           exit
        #end

        puts "end run iphone app package"
        Dir.chdir currentdir

      end
    end

  end

  task :buildsim => ["config:iphone", "build:iphone:rhodes"] do
     print_timestamp('run:buildsim START')
     unless $sdk =~ /^iphonesimulator/
       puts "SDK must be one of the iphonesimulator sdks to run in the iphone simulator"
       exit 1
     end
     kill_iphone_simulator

     #use_old_scheme = ($emulatortarget != 'iphone') && ($emulatortarget != 'ipad')
     use_old_scheme = false

     $sdkver = $sdk.gsub(/^iphonesimulator/, '')
     # Workaround: sometimes sdkver could differ from emulator version.
     # Example: iPhone SDK 4.0.1. In this case sdk is still iphonesimulator4.0 but version of simulator is 4.0.1
     $sdkver = $emulator_version.to_s unless $emulator_version.nil?

=begin

     #if use_old_scheme

          elements = []
          binplist = File.join(ENV['HOME'], 'Library', 'Preferences', 'com.apple.iphonesimulator.plist')
          xmlplist = '/tmp/iphone.plist'
          if File.exists? binplist
              `plutil -convert xml1 -o #{xmlplist} #{binplist}`

              elements = []
              doc = REXML::Document.new(File.new(xmlplist))
              nextignore = false
              doc.elements.each('plist/dict/*') do |element|
                  if nextignore
                      nextignore = false
                      next
                  end
                  if element.name == 'key'
                      if element.text == 'currentSDKRoot' or element.text == 'SimulateDevice'
                          nextignore = true
                          next
                      end
                  end

                  elements << element
              end
         end

         e = REXML::Element.new 'key'
         e.text = 'SimulateDevice'
         elements << e
         e = REXML::Element.new 'string'
         e.text = $sdkver == '3.2' ? 'iPad' : 'iPhone'
         elements << e
         e = REXML::Element.new 'key'
         e.text = 'currentSDKRoot'
         elements << e
         e = REXML::Element.new 'string'
         e.text = $sdkroot
         elements << e

         File.open(xmlplist, 'w') do |f|
             f.puts "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
             f.puts "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
             f.puts "<plist version=\"1.0\">"
             f.puts "<dict>"
             elements.each do |e|
                 f.puts "\t#{e.to_s}"
             end
             f.puts "</dict>"
             f.puts "</plist>"
         end

         `plutil -convert binary1 -o #{binplist} #{xmlplist}`

         rhorunner = File.join($app_path, "/project/iphone") + "/build/#{$configuration}-iphonesimulator/rhorunner.app"
         puts "rhorunner: #{rhorunner}"

         puts "our app name: #{$app_config['name']}"
         puts "simdir: #{$simdir}"

         Dir.glob(File.join($simdir, $sdkver, "Applications", "*")).each do |simapppath|
             need_rm = true if File.directory? simapppath
             if File.exists?(File.join(simapppath, 'rhorunner.app', 'name'))
                 name = File.read(File.join(simapppath, 'rhorunner.app', 'name'))
                 puts "found app name: #{name}"
                 guid = File.basename(simapppath)
                 puts "found guid: #{guid}"
                 if name == $app_config['name']
                     $guid = guid
                     need_rm = false
                 end
             end
             rm_rf simapppath if need_rm
             rm_rf simapppath + ".sb" if need_rm
         end

         puts "app guid: #{$guid}"

         mkdir_p File.join($simdir, $sdkver)

         simapp = File.join($simdir, $sdkver, "Applications")
         simlink = File.join($simdir, $sdkver, "Library", "Preferences")

         $simrhodes = File.join(simapp, $guid)
         chmod 0744, File.join($simrhodes, "rhorunner.app", "rhorunner") unless !File.exists?(File.join($simrhodes, "rhorunner.app", "rhorunner"))

         mkdir_p File.join($simrhodes, "Documents")
         mkdir_p File.join($simrhodes, "Library", "Preferences")
         mkdir_p File.join($simrhodes, "Library", "Caches", "Private Documents")

         rm_rf File.join($simrhodes, 'rhorunner.app')
         cp_r rhorunner, $simrhodes
         ['com.apple.PeoplePicker.plist', '.GlobalPreferences.plist'].each do |f|
             `ln -f -s "#{simlink}/#{f}" "#{$simrhodes}/Library/Preferences/#{f}"`
         end

#>>>>>>>>>>
        #`echo "#{$applog}" > "#{$simrhodes}/Documents/rhologpath.txt"`
        rholog = simapp + "/" + $guid + "/Library/Caches/Private Documents/rholog.txt"


        #simpublic = simapp + "/" + $guid + "/Documents/apps/public"
        simpublic = simapp + "/" + $guid + "/Library/Caches/Private Documents/apps/public"

        apppublic = $app_path + "/sim-public-#{$sdkver}"


        apprholog = $app_path + "/rholog-#{$sdkver}.txt"
        apprholog = $app_path + "/" + $app_config["applog"] if $app_config["applog"]

        rm_f apprholog
        rm_f apppublic
        #`ln -f -s "#{simpublic}" "#{apppublic}"`
        #`ln -f -s "#{rholog}" "#{apprholog}"`
        `echo > "#{rholog}"`
        f = File.new("#{simapp}/#{$guid}.sb","w")
        f << "(version 1)\n(debug deny)\n(allow default)\n"
        f.close
     #end

=end


    print_timestamp('run:buildsim FINISH')
  end

  # split this off separate so running it normally is run:iphone
  # testing we will not launch emulator directly
  desc "Builds everything, launches iphone simulator"
  task :iphone => ['run:iphone:build', 'run:iphone:run']

  task :allspecs do
    $dont_exit_on_failure = true
    Rake::Task['run:iphone:phone_spec'].invoke
    Rake::Task['run:iphone:framework_spec'].invoke
    failure_output = ""
    if $failed.to_i > 0
      failure_output = ""
      failure_output += "phone_spec failures:\n\n" + File.open(app_expanded_path('phone_spec') + "/faillog.txt").read if
        File.exist?(app_expanded_path('phone_spec') + "/faillog.txt")
      failure_output += "framework_spec failures:\n\n" + File.open(app_expanded_path('framework_spec') + "/faillog.txt").read if
        File.exist?(app_expanded_path('framework_spec') + "/faillog.txt")
      chdir basedir
      File.open("faillog.txt", "w") { |io| failure_output.each {|x| io << x }  }
    end
    puts "Agg Total: #{$total}"
    puts "Agg Passed: #{$passed}"
    puts "Agg Failed: #{$failed}"
    exit 1 if $total.to_i==0
    exit $failed.to_i
  end

  namespace "device" do
    desc "run IPA package on device"
    task :package, [:package_path] => ["config:iphone"] do |t, args|
      raise "run on device is UNSUPPORTED !!!"
    end
  end


end

namespace "clean" do
  desc "Clean iphone"
  task :iphone => ["clean:iphone:all", "clean:common"]

  namespace "iphone" do
#    desc "Clean rhodes binaries"
    task :rhodes => ["config:iphone"] do

      app_path = File.join($app_path, 'bin', 'target', 'iOS')
      rm_rf app_path

      iphone_project_folder = File.join($app_path, "project","iphone")

      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")
      appname_project = appname_fixed.slice(0, 1).capitalize + appname_fixed.slice(1..-1) + ".xcodeproj"


      if File.exists?(iphone_project_folder)

        chdir iphone_project_folder

        args = ['clean', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk, '-project', appname_project, '-quiet']
        if RUBY_PLATFORM =~ /(win|w)32$/
        else
          ret = IPhoneBuild.run_and_trace($xcodebuild,args,{:rootdir => $startdir})
          unless ret == 0
            puts "Error cleaning"
            exit 1
          end
        end
        chdir $startdir

        chdir File.join($app_path, "project","iphone")
           rm_rf File.join('build','Debug-*')
           rm_rf File.join('build','Release-*')
        chdir $startdir

      end


=begin
      # check hash for remove only current application
      found = true


      while found do
        found = false
        Find.find($simdir) do |path|
          if File.basename(path) == "rhorunner.app"

            if File.exists?(File.join(path, 'name'))
                name = File.read(File.join(path, 'name'))
                puts "found app name: #{name}"
                guid = File.basename(File.dirname(path))
                puts "found guid: #{guid}"
                if name == $app_config['name']
                  puts '>> We found our application !'
                  $guid = guid
                  found = true
                end
            end


          end
        end

        if found
         Dir.glob($simdir + '*').each do |sdk|
          simapp = sdk + "/Applications"
          simrhodes = File.join(simapp,$guid)
          rm_rf simrhodes
          rm_rf simrhodes + ".sb"
         end
        end
      end
=end
    end

#    desc "Clean rhobundle"
    task :rhobundle => ["config:iphone"] do
      if File.exists?($bindir)
        rm_rf $bindir
      end
    end


    def run_clean_for_extension(extpath, xcodeproject, xcodetarget)
      # only depfile is optional parameter !

      currentdir = Dir.pwd()
      Dir.chdir File.join(extpath, "platform/iphone")

      xcodeproject = File.basename(xcodeproject)


      #targetdir = ENV['TARGET_TEMP_DIR']
      tempdir = ENV['TEMP_FILES_DIR']
      rootdir = ENV['RHO_ROOT']
      xcodebuild = ENV['XCODEBUILD']
      configuration = ENV['CONFIGURATION']
      sdk = ENV['SDK_NAME']
      bindir = ENV['PLATFORM_DEVELOPER_BIN_DIR']
      sdkroot = ENV['SDKROOT']
      arch = ENV['ARCHS']
      gccbin = bindir + '/gcc-4.2'
      arbin = bindir + '/ar'

      iphone_path = '.'

      simulator = sdk =~ /iphonesimulator/

      if configuration == 'Distribution'
         configuration = 'Release'
      end

      rm_rf 'build'

      args = ['clean', '-target', xcodetarget, '-configuration', configuration, '-sdk', sdk, '-project', xcodeproject]

      additional_string = ''
      if simulator
      #    args << '-arch'
      #    args << 'i386'
        additional_string = ' ARCHS="i386 x86_64"'
      end

      require   rootdir + '/lib/build/jake.rb'

      if RUBY_PLATFORM =~ /(win|w)32$/
      else
        ret = IPhoneBuild.run_and_trace(xcodebuild,args,{:rootdir => rootdir, :string_for_add_to_command_line => additional_string})
      end
      Dir.chdir currentdir

    end


    def clean_extension_lib(extpath, sdk, xcodeproject, xcodetarget)

      puts "clean extension START :" + extpath

      if sdk =~ /iphonesimulator/
        $sdkver = sdk.gsub(/iphonesimulator/,"")
        $sdkroot = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" + $sdkver + ".sdk"
      else
        $sdkver = sdk.gsub(/iphoneos/,"")
        $sdkroot = $devroot + "/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS" + $sdkver + ".sdk"
      end

      ENV['RHO_PLATFORM'] = 'iphone'
      simulator = sdk =~ /iphonesimulator/
      ENV["PLATFORM_DEVELOPER_BIN_DIR"] ||= $devroot + "/Platforms/" + ( simulator ? "iPhoneSimulator" : "iPhoneOS" ) +
        ".platform/Developer/usr/bin"
      ENV["SDKROOT"] = $sdkroot

      ENV["BUILD_DIR"] ||= $startdir + "/platform/iphone/build"
      #ENV["TARGET_TEMP_DIR"] ||= target_dir
      ENV["TEMP_FILES_DIR"] ||= ENV["TARGET_TEMP_DIR"]

      ENV["ARCHS"] ||= simulator ? "i386 x86_64" : "armv7 armv7s arm64"
      ENV["RHO_ROOT"] = $startdir

      # added by dmitrys
      ENV["XCODEBUILD"] = $xcodebuild
      ENV["CONFIGURATION"] ||= $configuration
      ENV["SDK_NAME"] ||= sdk


      build_script = File.join(extpath, 'clean')


      if (xcodeproject != nil) and (xcodetarget != nil)

          run_clean_for_extension(extpath, xcodeproject, xcodetarget)

      else

        # modify executable attribute
        if File.exists? build_script
            if !File.executable? build_script
                 #puts 'change executable attribute for build script in extension : '+build_script
                 begin
                     #File.chmod 0700, build_script
                     #puts 'executable attribute was writed for : '+build_script
                 rescue Exception => e
                     puts 'ERROR: can not change attribute for clean script in extension ! Try to run clean command with sudo: prefix.'
                 end
            else
                 puts 'clean script in extension already executable : '+build_script
            end
            #puts '$$$$$$$$$$$$$$$$$$     START'
            currentdir = Dir.pwd()
            Dir.chdir extpath
            sh %{$SHELL ./clean}
            #Jake.run32("$SHELL "+build_script)
            Dir.chdir currentdir
            #puts '$$$$$$$$$$$$$$$$$$     FINISH'
            #if File.executable? build_script
                 #puts Jake.run('./build', [], extpath)
                 #exit 1 unless $? == 0
            #else
            #     puts 'ERROR: build script in extension is not executable !'
            #end
        else
            puts 'clean script in extension not found => pure ruby extension or not supported clean'
        end

      end

      puts "clean extension FINISH :" + extpath


    end


    task :extensions => ["config:iphone"] do
      init_extensions(nil, 'get_ext_xml_paths')
      #puts 'CLEAN ########################  ext='
      puts "extpaths: #{$app_config["extpaths"].inspect.to_s}"
      $stdout.flush
      $app_extensions_list.each do |ext, commin_ext_path |
          if commin_ext_path != nil
            puts 'CLEAN ext='+ext.to_s+'        path='+commin_ext_path.to_s
            extpath = File.join( commin_ext_path, 'ext')

            prebuilt_copy = false

            xcodeproject = nil  #xcodeproject
            xcodetarget = nil   #xcodetarget

            extyml = File.join(commin_ext_path,"ext.yml")
            if File.file? extyml
              extconf = Jake.config(File.open(extyml))
              extconf_iphone = extconf['iphone']
              exttype = 'build'
              exttype = extconf_iphone['exttype'] if extconf_iphone and extconf_iphone['exttype']

              xcodeproject = extconf_iphone['xcodeproject'] if extconf_iphone and extconf_iphone['xcodeproject']
              xcodetarget = extconf_iphone['xcodetarget'] if extconf_iphone and extconf_iphone['xcodetarget']

              if exttype != 'prebuilt'
                  clean_extension_lib(extpath, $sdk, xcodeproject, xcodetarget)
              end
            end
            #if ! prebuilt_copy
            #  build_extension_lib(extpath, sdk, target_dir, xcodeproject, xcodetarget, depfile)
            #end
          end
      end
    end

    task :all => ["clean:iphone:rhodes", "clean:iphone:rhobundle", "clean:iphone:extensions"]
  end
end


namespace "simulator" do
  namespace "iphone" do

    desc "build application package for simulator"
    task :production => ["config:iphone"] do
      Rake::Task['device:iphone:production'].invoke
    end

    desc "Builds and signs iphone for production, use prebuild binaries"
    task :production_with_prebuild_binary => ["config:iphone"] do
      Rake::Task['device:iphone:production_with_prebuild_binary'].invoke
    end

  end
end

namespace "device" do
  namespace "iphone" do
    # device:iphone:production
    desc "Builds and signs iphone for production"
    task :production => ["config:iphone", "build:iphone:rhodes"] do
      print_timestamp('device:iphone:production START')
      #copy build results to app folder

      app_path = File.join($app_path, 'bin', 'target', 'iOS', $sdk, $configuration)

      iphone_path = File.join($app_path, "/project/iphone")
      if $sdk =~ /iphonesimulator/
         iphone_path = File.join(iphone_path, 'build', $configuration+'-iphonesimulator')
      else
         iphone_path = File.join(iphone_path, 'build', $configuration+'-iphoneos')
      end
      appname = $app_config["name"]
      if appname == nil
         appname = 'rhorunner'
      end

      # fix appname for remove restricted symbols
      #appname = appname.downcase.split(/[^a-zA-Z0-9]/).map{|w| w.downcase}.join("_")
      appname = appname.split(/[^a-zA-Z0-9\_\-]/).map{|w| w}.join("_")

      src_file = File.join(iphone_path, 'rhorunner.app')
      dst_file = File.join(app_path, appname+'.app')

      rm_rf dst_file
      rm_rf app_path

      mkdir_p app_path

      puts 'copy result build package to application target folder ...'
      cp_r src_file, dst_file
      make_app_info
      ipapath = prepare_production_ipa(app_path, appname)
      prepare_production_plist(app_path, appname)
      copy_all_png_from_icon_folder_to_product(app_path)
      print_timestamp('device:iphone:production FINISH')
      puts '************************************'
      puts '*'
      puts "SUCCESS ! Production package built and placed into : "+ipapath
      puts '*'
      puts '************************************'
    end

    def determine_prebuild_path_iphone(config)
      RhoPackages.request 'rhodes-containers'
      require 'rhodes/containers'
      Rhodes::Containers::get_container_path_prefix('iphone', config)
    end


    desc "Builds and signs iphone for production, use prebuild binaries"
    task :production_with_prebuild_binary => ["config:iphone"] do

      print_timestamp('device:iphone:production_with_prebuild_binary START')

      #Support build.yml settings on cloud by copying to rhoconfig.txt
      Rake::Task['config:common:ymlsetup'].invoke

      currentdir = Dir.pwd()

      $skip_build_rhodes_main = true
      $skip_build_extensions = true
      $skip_build_xmls = true
      $use_prebuild_data = true
      $skip_build_js_api_files = true

      is_simulator = ($sdk =~ /iphonesimulator/)

      parent_ipa_path = File.join(determine_prebuild_path_iphone($app_config), "prebuild.ipa")

      puts '$ parent_ipa_path = '+parent_ipa_path

      Rake::Task['build:iphone:rhodes'].invoke

      chdir $app_path

      print_timestamp('bundle was builded')

      parent_app_bin = File.join($app_path, 'project/iphone/binp')
      rm_rf parent_app_bin
      mkdir_p parent_app_bin
      #cp parent_ipa_path, File.join(parent_app_bin, parent_ipa_path)

      chdir parent_app_bin
      #cmd "%{unzip "+parent_ipa_path+" -d "+parent_app_bin+" }"
      Jake.run('unzip', [parent_ipa_path, '-d', parent_app_bin])

      print_timestamp('parent IPA was unzipped')

      #copy bundle from bin to binp
      src_bundle_folder = File.join($app_path, "/project/iphone/bin/RhoBundle/")
      dst_bundle_folder = File.join($app_path, "/project/iphone/binp/Payload/prebuild.app/")

      #resource_to_copy = ['apps', 'db', 'lib', 'hash', 'name', 'rhofilelist.txt']






      # save container api folder
      rm_rf File.join($app_path, "/project/iphone/binp/tmpapi")
      mkdir_p File.join($app_path, "/project/iphone/binp/tmpapi")
      cp_r File.join(dst_bundle_folder, "apps/public/api"),File.join($app_path, "/project/iphone/binp/tmpapi/api")



      #apps
      rm_rf File.join(dst_bundle_folder, "apps")
      cp_r File.join(src_bundle_folder, "apps"),File.join(dst_bundle_folder, "apps")

      #restore container api
      rm_rf File.join(dst_bundle_folder, "apps/public/api")
      cp_r File.join($app_path, "/project/iphone/binp/tmpapi/api"),File.join(dst_bundle_folder, "apps/public")

      rm_rf File.join($app_path, "/project/iphone/binp/tmpapi")

      #db
      rm_rf File.join(dst_bundle_folder, "db")
      cp_r File.join(src_bundle_folder, "db"),dst_bundle_folder

      #hash
      rm_rf File.join(dst_bundle_folder, "hash")
      cp_r File.join(src_bundle_folder, "hash"),File.join(dst_bundle_folder, "hash")

      #hash
      rm_rf File.join(dst_bundle_folder, "name")
      cp_r File.join(src_bundle_folder, "name"),File.join(dst_bundle_folder, "name")

      #hash
      rm_rf File.join(dst_bundle_folder, "rhofilelist.txt")
      cp_r File.join(src_bundle_folder, "rhofilelist.txt"),File.join(dst_bundle_folder, "rhofilelist.txt")


      #copy icons
      src_app_icons_folder = File.join($app_path, "icon")
      dst_bundle_icons_folder = dst_bundle_folder
      ICONS.each do |pair|
        name = pair[0]+'.png'

        app_icon = File.join(src_app_icons_folder, name)
        bundle_icon = File.join(dst_bundle_icons_folder, name)

        if File.exists? app_icon
           rm_rf bundle_icon
           cp app_icon,bundle_icon

        end
      end



      #copy loading
      src_app_loading_folder = File.join($app_path, "app")
      dst_bundle_loading_folder = dst_bundle_folder
      LOADINGIMAGES.each do |name|
        defname = name.sub('loading', 'Default')

        app_loading = File.join(src_app_loading_folder, name)+'.png'
        bundle_loading = File.join(dst_bundle_loading_folder, defname)+'.png'

        rm_rf bundle_loading

        if File.exists? app_loading
           cp app_loading,bundle_loading
        end
      end


      # fix plist
      #rm_rf File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/Entitlements.plist')
      #cp File.join($app_path, 'project/iphone/Entitlements.plist'),File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/Entitlements.plist')

      #rm_rf File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/Info.plist')
      #cp File.join($app_path, 'project/iphone/Info.plist'),File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/Info.plist')

      chdir File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/')
      #Jake.run('plutil', ['-convert', 'xml1', 'Entitlements.plist'])
      Jake.run('plutil', ['-convert', 'xml1', 'Info.plist'])


      prebuild_plist = File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/Info.plist')
      app_plist = File.join($app_path, 'project/iphone/Info.plist')

      rm_rf app_plist
      cp prebuild_plist,app_plist

      Rake::Task['build:iphone:update_plist'].reenable
      Rake::Task['build:iphone:update_plist'].invoke

      rm_rf prebuild_plist
      cp app_plist,prebuild_plist

      chdir File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/')
      Jake.run('plutil', ['-convert', 'binary1', 'Info.plist'])


      #iTunesArtwork
      itunes_artwork_in_project = File.join(parent_app_bin, "Payload/prebuild.app/iTunesArtwork")
      itunes_artwork_in_project_2 = File.join(parent_app_bin, "Payload/prebuild.app/iTunesArtwork@2x")

      itunes_artwork_default = File.join($app_path, "/project/iphone/iTunesArtwork")
      itunes_artwork  = itunes_artwork_default

      itunes_artwork_new = File.join($app_path, "resources","ios","iTunesArtwork.png")
      if File.exists? itunes_artwork_new
          itunes_artwork = itunes_artwork_new
      end


      if !$app_config["iphone"].nil?
        if !$app_config["iphone"]["production"].nil?
          if !$app_config["iphone"]["production"]["ipa_itunesartwork_image"].nil?
            art_test_name = $app_config["iphone"]["production"]["ipa_itunesartwork_image"]
            if File.exists? art_test_name
              itunes_artwork = art_test_name
            else
              art_test_name = File.join($app_path,$app_config["iphone"]["production"]["ipa_itunesartwork_image"])
              if File.exists? art_test_name
                itunes_artwork = art_test_name
              else
                itunes_artwork = $app_config["iphone"]["production"]["ipa_itunesartwork_image"]
              end
            end
          end
        end
      end

      itunes_artwork_2 = itunes_artwork
      itunes_artwork_2 = itunes_artwork_2.gsub(".png", "@2x.png")
      if itunes_artwork_2.index('@2x') == nil
        itunes_artwork_2 = itunes_artwork_2.gsub(".PNG", "@2x.PNG")
      end
      if itunes_artwork_2.index('@2x') == nil
        itunes_artwork_2 = itunes_artwork_2 + '@2x'
      end

      rm_rf itunes_artwork_in_project
      cp itunes_artwork,itunes_artwork_in_project
      if itunes_artwork == itunes_artwork_default
        BuildOutput.warning("iTunesArtwork (image required for iTunes) not found - use from XCode project, can be default !!!" )
      end
      rm_rf itunes_artwork_in_project_2
      cp itunes_artwork_2,itunes_artwork_in_project_2
      if itunes_artwork == itunes_artwork_default
        BuildOutput.warning("iTunesArtwork@2x (image required for iTunes) not found - use from XCode project, can be default !!!" )
      end

      # copy to Payload root
      rm_rf File.join(parent_app_bin, "iTunesArtwork")
      #cp itunes_artwork, File.join(parent_app_bin, "iTunesArtwork")



      #repack into IPA and sign
      #zip -qr "Application.resigned.ipa" Payload
      appname = $app_config["name"]
      if appname == nil
        appname = 'rhorunner'
      end
      appname = appname.split(/[^a-zA-Z0-9\_\-]/).map{|w| w}.join("_")

      ipaname = appname + ".ipa"
      app_path = File.join($app_path, 'bin', 'target', 'iOS', $sdk, $configuration)

      #rename to new name
      mv File.join($app_path, 'project/iphone/binp/Payload/prebuild.app/'),File.join($app_path, 'project/iphone/binp/Payload/'+appname+'.app/')

      executable_file =  File.join($app_path, 'project/iphone/binp/Payload/'+appname+'.app/', 'rhorunner')
      if !File.executable? executable_file
         begin
             File.chmod 0700, executable_file
             puts 'executable attribute was writed for : '+executable_file
         rescue Exception => e
             puts 'ERROR: can not change attribute for executable in application package ! Try to run build command with sudo: prefix.'
         end
      else
        puts '$$$ executable is already have executable attribute !!! $$$'
      end
      print_timestamp('application bundle was updated')


      chdir parent_app_bin

      #sign
      if !is_simulator


        rm_rf File.join(parent_app_bin, "Payload/prebuild.app/_CodeSignature")
        rm_rf File.join(parent_app_bin, "Payload/prebuild.app/CodeResources")

        prov_file_path = File.join($app_path, 'production/embedded.mobileprovision')

        entitlements = nil

        if !$app_config["iphone"].nil?
          entitlements = $app_config["iphone"]["entitlements"]
          if entitlements == ""
            entitlements = nil
          end
          if !$app_config["iphone"]["production"].nil?
            if !$app_config["iphone"]["production"]["mobileprovision_file"].nil?
              test_name = $app_config["iphone"]["production"]["mobileprovision_file"]
              if File.exists? test_name
                prov_file_path = test_name
              else
                test_name = File.join($app_path,$app_config["iphone"]["production"]["mobileprovision_file"])
                if File.exists? test_name
                  prov_file_path = test_name
                else
                  prov_file_path = $app_config["iphone"]["production"]["mobileprovision_file"]
                end
              end
            end
          end
        end

        cp prov_file_path, File.join(parent_app_bin, "Payload/"+appname+".app/embedded.mobileprovision")

        #/usr/bin/codesign -f -s "iPhone Distribution: Certificate Name" --resource-rules "Payload/Application.app/ResourceRules.plist" "Payload/Application.app"

        if entitlements == nil
           if $app_config['capabilities'].index('push')
              #make fix file
              tmp_ent_dir = File.join($app_path, "/project/iphone/push_fix_entitlements")
              rm_rf tmp_ent_dir
              mkdir_p tmp_ent_dir
              entitlements = File.join(tmp_ent_dir, "Entitlements.plist")

              File.open(entitlements, 'w') do |f|
                  f.puts "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                  f.puts "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                  f.puts "<plist version=\"1.0\">"
                  f.puts "<dict>"
                  f.puts "<key>aps-environment</key>"
                  if $configuration == "Distribution"
                    f.puts "<string>production</string>"
                  else
                    f.puts "<string>development</string>"
                  end
                  f.puts "<key>get-task-allow</key>"
	                f.puts "<true/>"
                  f.puts "</dict>"
                  f.puts "</plist>"
              end



           end
        end

        if entitlements != nil
            tst_path = File.join($app_path, entitlements)
           if File.exists? tst_path
              entitlements = tst_path
           end
        end


        if entitlements != nil
            Jake.run('/usr/bin/codesign', ['-f', '-s', '"'+$signidentity+'"', '-i', '"'+$app_config["iphone"]["BundleIdentifier"]+'"', '--entitlements="'+entitlements+'"', 'Payload/'+appname+'.app'])
        else
            Jake.run('/usr/bin/codesign', ['-f', '-s', '"'+$signidentity+'"', '-i', '"'+$app_config["iphone"]["BundleIdentifier"]+'"', 'Payload/'+appname+'.app'])
        end
        #Jake.run('/usr/bin/codesign', ['-f', '-s', '"'+$signidentity+'"', '--resource-rules', 'Payload/prebuild.app/ResourceRules.plist', 'Payload/prebuild.app'])

        unless $?.success?
            raise "Error during signing of  application package !"
        end

      end

      print_timestamp('updated application was signed')

      sh %{zip -r -y temporary_archive.zip .}

      rm_rf app_path

      mkdir_p app_path

      puts 'copy result build package to application target folder ...'

      cp File.join(parent_app_bin, "temporary_archive.zip"), File.join(app_path, ipaname)

      rm_rf parent_app_bin
      #chdir File.join($app_path, 'project/iphone/binp/Payload/'+appname+'.app/')
      #Jake.run('plutil', ['-convert', 'xml1', 'Info.plist'])

      #Jake.run('zip', ['-qr', ipaname, 'Payload'])
      Dir.chdir currentdir

      print_timestamp('device:iphone:production_with_prebuild_binary FINISH')
      puts '************************************'
      puts '*'
      puts "SUCCESS ! Production package builded and placed into : "+File.join(app_path, ipaname)
      puts '*'
      puts '************************************'



    end


     task :make_container, [:container_prefix_path] => :production do |t, args|
      container_prefix_path = args[:container_prefix_path]
      appname = $app_config["name"]
      if appname == nil
        appname = 'rhorunner'
      end
      appname = appname.split(/[^a-zA-Z0-9\_\-]/).map{|w| w}.join("_")

      ipaname = appname + ".ipa"
      app_path = File.join($app_path, 'bin', 'target', 'iOS', $sdk, $configuration)


      rm_rf container_prefix_path
      mkdir_p container_prefix_path

      cp File.join(app_path, ipaname), File.join(container_prefix_path, "prebuild.ipa")
    end



    task :production_with_prebuild_libs => ["config:iphone"] do

      print_timestamp('device:iphone:production_with_prebuild_libs START')

      rm_rf File.join($app_path, "project/iphone")
      $use_prebuild_data = true


      Rake::Task['build:iphone:rhodes'].invoke



      #copy build results to app folder

      app_path = File.join($app_path, 'bin', 'target', 'iOS', $sdk, $configuration)

      iphone_path = File.join($app_path, "/project/iphone")
      if $sdk =~ /iphonesimulator/
         iphone_path = File.join(iphone_path, 'build', $configuration+'-iphonesimulator')
      else
         iphone_path = File.join(iphone_path, 'build', $configuration+'-iphoneos')
      end
      appname = $app_config["name"]
      if appname == nil
         appname = 'rhorunner'
      end

      # fix appname for remove restricted symbols
      #appname = appname.downcase.split(/[^a-zA-Z0-9]/).map{|w| w.downcase}.join("_")
      appname = appname.split(/[^a-zA-Z0-9\_\-]/).map{|w| w}.join("_")

      src_file = File.join(iphone_path, 'rhorunner.app')
      dst_file = File.join(app_path, appname+'.app')

      rm_rf dst_file
      rm_rf app_path

      mkdir_p app_path

      puts 'copy result build package to application target folder ...'
      cp_r src_file, dst_file
      make_app_info
      prepare_production_ipa(app_path, appname)
      prepare_production_plist(app_path, appname)
      copy_all_png_from_icon_folder_to_product(app_path)
      print_timestamp('device:iphone:production_with_prebuild_libs FINISH')
    end


  end

end

namespace :stop do
  task :iphone do
    kill_iphone_simulator
  end

  task "iphone:emulator" => :iphone
end

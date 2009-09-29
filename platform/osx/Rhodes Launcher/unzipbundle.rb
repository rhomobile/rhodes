require 'Find'
require 'fileutils'
include FileUtils


chdir File.dirname(__FILE__)

chdir "rhorunner.app"

rm_rf "apps"
rm_rf "lib"

`unzip -o rhobundle.zip`


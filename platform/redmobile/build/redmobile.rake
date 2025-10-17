# platform/redmobile/build/redmobile.rake
#
# This file provides aliases for android rake tasks to support the redmobile platform.
# It allows running `rake <namespace>:redmobile:<task>` which will execute the
# corresponding `rake <namespace>:android:<task>`.

# Main tasks
task 'device:redmobile:production' => 'device:android:production'
task 'device:redmobile:debug' => 'device:android:debug'
task 'device:redmobile:install' => 'device:android:install'

# Run tasks
task 'run:redmobile:emulator' => 'run:android:emulator'
task 'run:redmobile:device' => 'run:android:device'

# Clean task
task 'clean:redmobile' => 'clean:android'

# Config task
task 'config:redmobile' => 'config:android'
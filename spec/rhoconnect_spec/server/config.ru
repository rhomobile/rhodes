#!/usr/bin/env ruby
require 'rhoconnect/application/init'

# secret is generated along with the app
Rhoconnect::Server.set     :secret,      '1ad5ff6920081308bcc1425b3106e57d95dbbf31ae9068b7e603aacc6193afb47c665dc086c065eaebc8ba2c6ec3dd3ba286d482f0f47f3ae424be2251c1a2aa'

#ENV['AMAZON_ACCESS_KEY_ID'] = access key id should be set via environment variable
#ENV['AMAZON_SECRET_ACCESS_KEY'] = secret access key sould be set via environment variable

# !!! Add your custom initializers and overrides here !!!
# For example, uncomment the following line to enable Stats
#Rhoconnect::Server.enable  :stats
# uncomment the following line to disable Resque Front-end console
#Rhoconnect.disable_resque_console = true
# uncomment the following line to disable Rhoconnect Front-end console
#Rhoconnect.disable_rc_console = true

# Load RhoConnect application
require './application'

# run RhoConnect Application
run Rhoconnect.app

TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine \
  ../../../lib/extensions/bigdecimal/ext/bigdecimal.pro

unix:!macx {
 # SUBDIRS += curl
}
macx {
 SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes

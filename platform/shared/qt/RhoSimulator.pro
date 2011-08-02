TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine
unix {
 # SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes

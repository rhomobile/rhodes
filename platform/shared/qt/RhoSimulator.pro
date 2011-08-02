TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine
unix:!macx {
 # SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes

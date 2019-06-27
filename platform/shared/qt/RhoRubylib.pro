CONFIG += c++14

TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine

CONFIG += ordered
SUBDIRS += rhodes

QMAKE_CXXFLAGS_RELEASE += -MP9
QMAKE_CXXFLAGS_DEBUG += -MP9

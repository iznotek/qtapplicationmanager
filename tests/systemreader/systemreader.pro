TARGET = tst_systemreader

include($$PWD/../tests.pri)

QT *= appman_monitor-private \
      appman_manager-private \
      appman_window-private \
      appman_application-private \
      appman_common-private

SOURCES += tst_systemreader.cpp

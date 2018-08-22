includesPath =  \
    $$PWD/      \

INCLUDEPATH += $$includesPath
DEPENDPATH += $$includesPath

SOURCES += \
    $$PWD/MySetting.cpp \
    $$PWD/MySettingDialog.cpp \
    $$PWD/MySettingCommon.cpp

HEADERS += \
    $$PWD/MySetting.h \
    $$PWD/MySettingDialog.h \
    $$PWD/MySettingCommon.h


FORMS += \
    $$PWD/MySettingDialog.ui \
    $$PWD/MySettingCommon.ui

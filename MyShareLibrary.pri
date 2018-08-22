includesPath =  \
    $$PWD/      \

INCLUDEPATH += $$includesPath
DEPENDPATH += $$includesPath

include(MySingleApplication/MySingleApplication.pri)
include(MyMainwindow/MyMainwindow.pri)
include(MyLabel/MyLabel.pri)
include(MyTimekeeping/MyTimekeeping.pri)
include(MyNameSpace/MyNameSpace.pri)
include(MySetting/MySetting.pri)
#include(MySingleton/MySingleton.pri)
include(MyReport/MyReport.pri)
include(database/database.pri)
include(MyDebug/MyDebug.pri)
include(MyDatabase/MyDatabase.pri)
include(MyComboBox/MyComboBox.pri)

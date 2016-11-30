HEADERS +=  viewlisttable.h\
            databasedirection.h\
            backupftp.h\
            fordelete.h\
            numprefix.h\
            photolabel.h\
            update.h \
            lineedit.h\
            photoload.h\
            searchform.h\
            smtp.h\
            registration.h \
            prefixnumber.h \
            ftpform.h\
            boolitemdelegate.h\
    bolditemdelegate.h \
    uploadbase.h \
    intitemdelegate.h \
    $$PWD/dateitemdelegate.h \
    $$PWD/noedititemdelegat.h \
    $$PWD/putfile.h \
    $$PWD/sqlform.h

    $$PWD/programmsettings.h

SOURCES +=  viewlisttable.cpp\
            databasedirection.cpp\
            backupftp.cpp\
            fordelete.cpp\
            numprefix.cpp\
            photolabel.cpp\
            update.cpp \
            lineedit.cpp\
            photoload.cpp\
            searchform.cpp\
            smtp.cpp\
            registration.cpp\
            prefixnumber.cpp \
            ftpform.cpp\
            boolitemdelegate.cpp\
    bolditemdelegate.cpp \
    uploadbase.cpp \
    intitemdelegate.cpp \
    $$PWD/dateitemdelegate.cpp \
    $$PWD/putfile.cpp \
    $$PWD/sqlform.cpp

    $$PWD/programmsettings.cpp

include(Import.pri)

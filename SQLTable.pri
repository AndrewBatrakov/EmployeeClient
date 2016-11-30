HEADERS +=  organization.h\
            subdivision.h\
            employee.h\
            contract.h\
            physical.h\
            grafikrabot.h \
    $$PWD/sizform.h \
    $$PWD/postform.h \
    $$PWD/obosoblform.h \
    $$PWD/scoreform.h

SOURCES +=  organization.cpp\
            subdivision.cpp\
            employee.cpp\
            contract.cpp\
            physical.cpp\
            grafikrabot.cpp \
    $$PWD/sizform.cpp \
    $$PWD/postform.cpp \
    $$PWD/obosoblform.cpp \
    $$PWD/scoreform.cpp

include(PromBezop.pri)
include(Laboratory.pri)
include(SIZ.pri)
include(Obuchenie.pri)
include(Scores.pri)
include(Protocol.pri)

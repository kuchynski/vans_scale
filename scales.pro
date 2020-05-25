#QT -= gui
QT += widgets
QT += printsupport
#CONFIG += c++11 console
#CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS
QT_DISABLE_DEPRECATED_BEFORE

HEADERS += \
    SStateBloc.h \
    SAxel.h \
    CVan.h \
    CTypeVan.h \
    CTurn.h \
    ctable.h \
    CString.h \
    CStackFourth.h \
    CStackDual.h \
    CStack.h \
    CSostav.h \
    csavefiletxt.h \
    cprocessoroftare.h \
    cparam.h \
    coptions.h \
    cmonitor.h \
    cinput.h \
    cimage.h \
    CFilter.h \
    CDateTime.h \
    ccomport.h \
    ccommander.h \
    cchecks.h \
    cbuttons.h \
    CBuffer.h \
    CBloc.h \
    cbd.h \
    CAutoschepka.h \
    CAllWindows.h \
    cbuttonscheck.h \
    cspinbox.h \
    cpboptions.h \
    clabelchange.h \
    ctarirovka.h \
    cfilebin.h \
    cresult.h \
    cresulttare.h \
    cresultdefect.h \
    cmanywindows.h \
    CDefect.h \
    cimagedefect.h \
    CFFT.h

SOURCES += \
    main.cpp \
    SAxel.cpp \
    CVan.cpp \
    CTypeVan.cpp \
    ctable.cpp \
    CString.cpp \
    CSostav.cpp \
    csavefiletxt.cpp \
    cprocessoroftare.cpp \
    cparam.cpp \
    coptions.cpp \
    cmonitor.cpp \
    cinput.cpp \
    cimage.cpp \
    CFilter.cpp \
    CDateTime.cpp \
    ccomport.cpp \
    ccommander.cpp \
    cchecks.cpp \
    cbuttons.cpp \
    CBloc.cpp \
    cbd.cpp \
    CAutoschepka.cpp \
    CAllWindows.cpp \
    cbuttonscheck.cpp \
    cspinbox.cpp \
    cpboptions.cpp \
    clabelchange.cpp \
    ctarirovka.cpp \
    cfilebin.cpp \
    cresult.cpp \
    cresulttare.cpp \
    cresultdefect.cpp \
    cmanywindows.cpp \
    CDefect.cpp \
    cimagedefect.cpp \
    CFFT.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
















































# Copyright (c) 2011 Nokia Corporation.

QT += core gui declarative

TARGET = quickhit
TEMPLATE = app
VERSION = 2.0.3

INCLUDEPATH += ./ge_src/

SOURCES += \
    src/gameengine.cpp \
    src/invsounds.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/myeventfilter.cpp \
    src/plugindata.cpp \
    ge_src/GEAudioBuffer.cpp \
    ge_src/GEAudioBufferPlayInstance.cpp \
    ge_src/GEAudioMixer.cpp \
    ge_src/GEAudioOut.cpp \
    ge_src/GEInterfaces.cpp

HEADERS += \
    src/gameengine.h \
    src/invsounds.h \
    src/mainwindow.h \
    src/myeventfilter.h \
    src/plugindata.h \
    plugins/levelplugininterface.h \
    ge_src/GEAudioBuffer.h \
    ge_src/GEAudioBufferPlayInstance.h \
    ge_src/GEAudioMixer.h \
    ge_src/GEAudioOut.h \
    ge_src/GEInterfaces.h \
    ge_src/trace.h

RESOURCES += rsc/resources.qrc

OTHER_FILES += qml/*.qml

# Uncomment/comment the following line to enable/disable debug prints of
# Qt GameEnabler audio framework.
#DEFINES += GE_DEBUG


symbian {
    TARGET = QuickHit
    TARGET.UID3 = 0xe7ced714
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x20000 0x5000000

    TARGET.CAPABILITY = \
        LocalServices \
        Location \
        NetworkServices \
        ReadUserData \
        WriteUserData \
        UserEnvironment

    QT += multimedia
    CONFIG += mobility
    MOBILITY = systeminfo

    RESOURCES += rsc/no_components.qrc

    LIBS += -lcone -leikcore -lavkon -lhwrmvibraclient

    ICON = icons/icon.svg

    # Add LevelOne.sis into QuickHit sis
    levelone.pkg_postrules.main = "IF NOT package(0xE07dfb66)"\
                                   "; Install levelone.sis only if not already installed"\
                                    "@\"levelone.sis\", (0xE07dfb66)" \
                                    "endif "
    DEPLOYMENT += levelone

    PRIVATEDIR = $$replace(TARGET.UID3, "^0x", "")

    # Enables In-App Purchase API
    DEFINES += IA_PURCHASE
    contains(DEFINES, IA_PURCHASE) {
        message(In-App Purchase API enabled)

        CONFIG += inapppurchase
        QT += network script

        INCLUDEPATH += ./iap_src
        HEADERS += \
            iap_src/downloadmanager.h \
            iap_src/iapurchase.h \
            iap_src/product.h \
            iap_src/request.h

        SOURCES += \
            iap_src/downloadmanager.cpp \
            iap_src/iapurchase.cpp \
            iap_src/product.cpp

        iapVariantId.sources = ./iap/IAP_VARIANTID.txt
        iapVariantId.path = .
        DEPLOYMENT += iapVariantId

        # Enables test mode for IAP
        DEFINES += IA_PURCHASE_TEST_MODE

        # For testing In-App Purchase without Nokia Store
        contains(DEFINES, IA_PURCHASE_TEST_MODE) {
            message(In-App Purchase API in TEST_MODE)
            addConfigFiles.sources = ./iap/TEST_MODE.txt
            addConfigFiles.path = .
            DEPLOYMENT += addConfigFiles
        }
    }
}


unix:!symbian {
    CONFIG += mobility
    MOBILITY = systeminfo

    BINDIR = /opt/usr/bin
    DATADIR = /usr/share
    DEFINES += DATADIR=\\\"$$DATADIR\\\" \
       PKGDATADIR=\\\"$$PKGDATADIR\\\"

    target.path = $$BINDIR

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += icons/$${TARGET}.png

    maemo5 {
        message(Maemo build)
        RESOURCES += rsc/no_components.qrc
        OTHER_FILES += qml/no_components.qml
        QT += opengl multimedia
        desktop.path = $$DATADIR/applications/hildon
        desktop.files += qtc_packaging/debian_fremantle/$${TARGET}.desktop
    }
    else {
        message(Harmattan build)
        DEFINES += Q_WS_MAEMO_6
        MOBILITY += multimedia

        RESOURCES += rsc/harmattan.qrc
        OTHER_FILES += qml/harmattan.qml

        desktop.path = $$DATADIR/applications/
        desktop.files += qtc_packaging/debian_harmattan/$${TARGET}.desktop

        # Classify the application as a game to support volume keys on Harmattan.
        gameclassify.files += qtc_packaging/debian_harmattan/$${TARGET}.conf
        gameclassify.path = /usr/share/policy/etc/syspart.conf.d

        INSTALLS += gameclassify
    }

    INSTALLS += \
        target \
        desktop \
        icon64
}


win32 {
    message(Windows)
    TARGET = QuickHit
    QT += multimedia
    RESOURCES += rsc/no_components.qrc

    # Copy level files into c:\\quickhitdata folder
    system(mkdir c:\\quickhitdata)
    system(mkdir c:\\quickhitdata\\levelone\\gfx)
    system(mkdir c:\\quickhitdata\\levelone\\sound)
    system(mkdir c:\\quickhitdata\\leveltwo\\gfx)
    system(mkdir c:\\quickhitdata\\leveltwo\\sound)

    system(copy plugins\\LevelOne\\Level.qml c:\\quickhitdata\\levelone)
    system(copy plugins\\LevelOne\\gfx\\*.* c:\\quickhitdata\\levelone\\gfx)
    system(copy plugins\\LevelOne\\sound\\*.* c:\\quickhitdata\\levelone\\sound)

    system(copy plugins\\LevelTwo\\qml\\Level.qml c:\\quickhitdata\\leveltwo)
    system(copy plugins\\LevelTwo\\qml\\Enemy.qml c:\\quickhitdata\\leveltwo)
    system(copy plugins\\LevelTwo\\qml\\Level.js c:\\quickhitdata\\leveltwo)
    system(copy plugins\\LevelTwo\\gfx\\*.* c:\\quickhitdata\\leveltwo\\gfx)
    system(copy plugins\\LevelTwo\\sound\\*.* c:\\quickhitdata\\leveltwo\\sound)
}



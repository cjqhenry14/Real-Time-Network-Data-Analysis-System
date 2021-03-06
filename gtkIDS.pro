######################################################################
# Automatically generated by qmake (2.01a) ?? 4? 2 13:38:14 2014
######################################################################
#`pkg-config --cflags --libs gtk+-2.0`
TEMPLATE = app
LIBS += -L/usr/lib/mysql -lmysqlclient -lz -lpcap 
TARGET = 
CONFIG += link_pkgconfig
PKGCONFIG += gtk+-2.0
DEPENDPATH += . \
              include \
              src/Decoder \
              src/DetectionEngine \
              src/init \
              src/Preprocessor \
              src/Sniffer \
              src/Manager \
              src/RuleBase \
              src/AttackStore \

INCLUDEPATH += . include \
              /usr/include/mysql \

# Input
HEADERS += include/decoder_dataCache.h \
           include/detect_acvl.h \
           include/preprocessor_process.h \
           include/sniffer_sqlToCache.h \
           include/init.h \
           include/manager.h \
           include/rulebase.h \
           include/attack_store.h \
           include/record.h \

SOURCES += src/Decoder/dataCache.cpp \
           src/DetectionEngine/detect_acvl.cpp \
           src/init/init.cpp \
           src/Preprocessor/process.cpp \
           src/Sniffer/sqlToCache.cpp \
           src/Sniffer/pcapToCache.cpp \
           src/Manager/manager.cpp \
           src/RuleBase/rulebase.cpp \
           src/AttackStore/attack_store.cpp \
           src/AttackStore/record.cpp \



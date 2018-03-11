TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -msse2

SOURCES += main.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += eigen3

#DEFINES += "EIGEN_DONT_VECTORIZE"

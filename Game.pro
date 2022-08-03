TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        game.c \
        main.c

HEADERS += \
    game.h

unix:!macx: LIBS += -L$$PWD/../GameEngine-Debug/ -lGameEngine

INCLUDEPATH += $$PWD/../GameEngine
DEPENDPATH += $$PWD/../GameEngine

LIBS += -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr

DISTFILES += \
    shaders/3DObject/line_shader.frag \
    shaders/3DObject/line_shader.vert \
    shaders/3DObject/outline_shader.frag \
    shaders/3DObject/outline_shader.vert \
    shaders/3DObject/shader.frag \
    shaders/3DObject/shader.vert \
    shaders/Sprite/shader.frag \
    shaders/Sprite/shader.vert \
    shaders/Sprite/test2D.frag \
    shaders/Sprite/test2D.vert

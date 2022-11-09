
obj = bin/fonts/fantazer-normal.o \
	bin/fonts/Clear_Sans_Bold.o \
	bin/fonts/arial.o \
	bin/fonts/Lilita_Regular.o \
	bin/fonts/null-normal.o \
	bin/shaders/model/frag.o \
	bin/shaders/model/vert.o \
	bin/shaders/gui/widget_frag.o \
	bin/shaders/gui/widget_vert.o \
	bin/shaders/gui/widget_window_frag.o \
	bin/shaders/gui/widget_window_vert.o \
	bin/shaders/text/frag.o \
	bin/shaders/text/vert.o \
	bin/shaders/sprite/frag.o \
	bin/shaders/sprite/vert.o \
	bin/shaders/3d_object/frag.o \
	bin/shaders/3d_object/vert.o \

all: clean $(obj) cpy

	
bin/shaders/3d_object/%.o : shaders/3d_object/%.spv
	ld -r -b binary $< -o $@	
	
bin/shaders/gui/%.o : shaders/gui/%.spv
	ld -r -b binary $< -o $@
	
bin/shaders/model/%.o : shaders/model/%.spv
	ld -r -b binary $< -o $@
	
bin/shaders/text/%.o : shaders/text/%.spv
	ld -r -b binary $< -o $@
	
bin/shaders/sprite/%.o : shaders/sprite/%.spv
	ld -r -b binary $< -o $@

bin/fonts/%.o : fonts/%.ttf
	ld -r -b binary $< -o $@
	
bin/fonts/%.o : fonts/%.otf
	ld -r -b binary $< -o $@

cpy: 
	cp -r bin ../GameEngine
	
clean:
	$(RM) *.spv bin/*.o


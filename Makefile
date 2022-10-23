
obj = bin/fonts/fantazer-normal.o \
	bin/fonts/Clear_Sans_Bold.o \
	bin/fonts/arial.o \
	bin/fonts/Lilita_Regular.o \
	bin/fonts/null-normal.o \
	bin/shaders/model/model_frag.o \
	bin/shaders/model/model_vert.o \
	bin/shaders/gui/widget_frag.o \
	bin/shaders/gui/widget_vert.o \
	bin/shaders/gui/widget_text_frag.o \
	bin/shaders/gui/widget_text_vert.o \
	bin/shaders/gui/widget_window_frag.o \
	bin/shaders/gui/widget_window_vert.o \

all: clean $(obj)

shaders/gui/%.spv : shaders/gui/%.frag
	glslc $< -o $@	
	
shaders/gui/%.spv : shaders/gui/%.vert
	glslc $< -o $@	
	
bin/shaders/gui/%.o : shaders/gui/%.spv
	ld -r -b binary $< -o $@
		
shaders/model/%.spv : shaders/model/%.frag
	glslc $< -o $@	
	
shaders/model/%.spv : shaders/model/%.vert
	glslc $< -o $@	
	
bin/shaders/model/%.o : shaders/model/%.spv
	ld -r -b binary $< -o $@

bin/fonts/%.o : fonts/%.ttf
	ld -r -b binary $< -o $@
	
bin/fonts/%.o : fonts/%.otf
	ld -r -b binary $< -o $@

clean:
	$(RM) *.spv bin/*.o


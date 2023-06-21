
obj = bin/fonts/fantazer-normal.o \
	bin/fonts/RobotoBlack.o \
	bin/fonts/TenorSans.o \
	bin/fonts/null-normal.o \
	bin/shaders/model/frag.o \
	bin/shaders/model/vert.o \
	bin/shaders/model/shadow_frag.o \
	bin/shaders/model/shadow_vert.o \
	bin/shaders/gui/painter_frag.o \
	bin/shaders/gui/painter_vert.o \
	bin/shaders/gui/widget_frag.o \
	bin/shaders/gui/widget_vert.o \
	bin/shaders/gui/widget_window_frag.o \
	bin/shaders/gui/widget_window_vert.o \
	bin/shaders/text/frag.o \
	bin/shaders/text/vert.o \
	bin/shaders/quad/frag.o \
	bin/shaders/quad/frag_2.o \
	bin/shaders/quad/frag_3.o \
	bin/shaders/quad/vert.o \
	bin/shaders/geometry/frag.o \
	bin/shaders/geometry/vert.o \
	bin/shaders/depth/frag.o \
	bin/shaders/depth/vert.o \
	bin/shaders/depth/frag_omni.o \
	bin/shaders/depth/vert_omni.o \
	bin/shaders/depth/frag_spot.o \
	bin/shaders/depth/vert_spot.o \
	bin/shaders/sprite/frag.o \
	bin/shaders/sprite/vert.o \
	bin/shaders/image/frag.o \
	bin/shaders/image/vert.o \
	bin/shaders/terrain/frag.o \
	bin/shaders/terrain/vert.o \
	bin/shaders/terrain/tesc.o \
	bin/shaders/terrain/tese.o \
	bin/shaders/terrain/shadow_frag.o \
	bin/shaders/terrain/shadow_vert.o \
	bin/shaders/tree/frag.o \
	bin/shaders/tree/vert.o \
	bin/shaders/tree/instance_frag.o \
	bin/shaders/tree/instance_vert.o \
	bin/shaders/tree/sdf_frag.o \
	bin/shaders/tree/sdf_vert.o \
	bin/shaders/default/frag2D.o \
	bin/shaders/default/vert2D.o \
	bin/shaders/default/frag3D.o \
	bin/shaders/default/vert3D.o \
	bin/shaders/sky/sky_frag.o \
	bin/shaders/sky/sky_vert.o \
	bin/shaders/sky/stars_frag.o \
	bin/shaders/sky/stars_vert.o \
	bin/shaders/sky/atmospheric_frag.o \
	bin/shaders/sky/atmospheric_vert.o \
	bin/shaders/3d_object/frag.o \
	bin/shaders/3d_object/vert.o \
	bin/shaders/3d_object/instance_frag.o \
	bin/shaders/3d_object/instance_vert.o \
	bin/shaders/3d_object/shadow_frag.o \
	bin/shaders/3d_object/shadow_vert.o \
	bin/shaders/3d_object/line_frag.o \
	bin/shaders/3d_object/line_vert.o \
	bin/shaders/particle/frag2D.o \
	bin/shaders/particle/vert2D.o \
	bin/shaders/particle/frag3D.o \
	bin/shaders/particle/vert3D.o \
	bin/shaders/water/frag.o \
	bin/shaders/water/vert.o \

all: 
	cd shaders && $(MAKE)
	$(MAKE) make_union
	$(MAKE) make_obj
	
make_obj : $(obj) cpy

make_union : $(obj)
	ld -relocatable $^ -o data/data1.bin

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

bin/shaders/particle/%.o : shaders/particle/%.spv
	ld -r -b binary $< -o $@

bin/shaders/terrain/%.o : shaders/terrain/%.spv
	ld -r -b binary $< -o $@

bin/shaders/tree/%.o : shaders/tree/%.spv
	ld -r -b binary $< -o $@

bin/shaders/default/%.o : shaders/default/%.spv
	ld -r -b binary $< -o $@

bin/shaders/image/%.o : shaders/image/%.spv
	ld -r -b binary $< -o $@

bin/shaders/sky/%.o : shaders/sky/%.spv
	ld -r -b binary $< -o $@

bin/shaders/depth/%.o : shaders/depth/%.spv
	ld -r -b binary $< -o $@

bin/shaders/quad/%.o : shaders/quad/%.spv
	ld -r -b binary $< -o $@

bin/shaders/geometry/%.o : shaders/geometry/%.spv
	ld -r -b binary $< -o $@

bin/shaders/water/%.o : shaders/water/%.spv
	ld -r -b binary $< -o $@

bin/fonts/%.o : fonts/%.ttf
	ld -r -b binary $< -o $@

bin/fonts/%.o : fonts/%.otf
	ld -r -b binary $< -o $@

cpy: 
	cp -r data ../GameEngine

clean: $(obj)
	$(RM) $^


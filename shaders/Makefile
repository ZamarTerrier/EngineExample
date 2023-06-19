
obj = \
        model/frag.spv \
        model/vert.spv \
        model/shadow_frag.spv \
        model/shadow_vert.spv \
        gui/painter_frag.spv \
        gui/painter_vert.spv \
        gui/widget_frag.spv \
        gui/widget_vert.spv \
        gui/widget_window_frag.spv \
        gui/widget_window_vert.spv \
        text/frag.spv \
        text/vert.spv \
        default/frag2D.spv \
        default/vert2D.spv \
        default/frag3D.spv \
        default/vert3D.spv \
        quad/frag_3.spv \
        quad/frag_2.spv \
        quad/frag.spv \
        quad/vert.spv \
        image/frag.spv \
        image/vert.spv \
        geometry/frag.spv \
        geometry/vert.spv \
        depth/frag.spv \
        depth/vert.spv \
        depth/frag_omni.spv \
        depth/vert_omni.spv \
        depth/frag_spot.spv \
        depth/vert_spot.spv \
        sprite/frag.spv \
        sprite/vert.spv \
        terrain/frag.spv \
        terrain/vert.spv \
        terrain/tesc.spv \
        terrain/tese.spv \
        terrain/shadow_frag.spv \
        terrain/shadow_vert.spv \
        tree/frag.spv \
        tree/vert.spv \
        tree/sdf_frag.spv \
        tree/sdf_vert.spv \
        tree/instance_frag.spv \
        tree/instance_vert.spv \
        sky/sky_frag.spv \
        sky/sky_vert.spv \
        sky/stars_frag.spv \
        sky/stars_vert.spv \
        sky/atmospheric_frag.spv \
        sky/atmospheric_vert.spv \
        3d_object/frag.spv \
        3d_object/vert.spv \
        3d_object/shadow_frag.spv \
        3d_object/shadow_vert.spv \
        3d_object/instance_frag.spv \
        3d_object/instance_vert.spv \
        3d_object/line_frag.spv \
        3d_object/line_vert.spv \
        particle/frag2D.spv \
        particle/vert2D.spv \
        particle/frag3D.spv \
        particle/vert3D.spv \

all: $(obj)
	
3d_object/line_frag.spv : 3d_object/line.frag
	glslc $< -o $@

3d_object/line_vert.spv : 3d_object/line.vert
	glslc $< -o $@	

3d_object/frag.spv : 3d_object/shader.frag
	glslc $< -o $@

3d_object/vert.spv : 3d_object/shader.vert
	glslc $< -o $@

3d_object/instance_frag.spv : 3d_object/shader_instance.frag
	glslc $< -o $@

3d_object/instance_vert.spv : 3d_object/shader_instance.vert
	glslc $< -o $@

3d_object/shadow_frag.spv : 3d_object/shader_shadow.frag
	glslc $< -o $@

3d_object/shadow_vert.spv : 3d_object/shader_shadow.vert
	glslc $< -o $@

gui/painter_frag.spv : gui/painter.frag
	glslc $< -o $@

gui/painter_vert.spv : gui/painter.vert
	glslc $< -o $@

gui/widget_frag.spv : gui/widget_frag.frag
	glslc $< -o $@
	
gui/widget_vert.spv : gui/widget_vert.vert
	glslc $< -o $@
	
gui/widget_window_frag.spv : gui/widget_window_frag.frag
	glslc $< -o $@

gui/widget_window_vert.spv : gui/widget_window_vert.vert
	glslc $< -o $@

depth/frag.spv : depth/shader.frag
	glslc $< -o $@

depth/vert.spv : depth/shader.vert
	glslc $< -o $@
	
depth/frag_omni.spv : depth/shader_omni.frag
	glslc $< -o $@

depth/vert_omni.spv : depth/shader_omni.vert
	glslc $< -o $@
	
depth/frag_spot.spv : depth/spot_shader.frag
	glslc $< -o $@

depth/vert_spot.spv : depth/spot_shader.vert
	glslc $< -o $@

model/frag.spv : model/shader.frag
	glslc $< -o $@

model/vert.spv : model/shader.vert
	glslc $< -o $@

model/shadow_frag.spv : model/shader_shadow.frag
	glslc $< -o $@

model/shadow_vert.spv : model/shader_shadow.vert
	glslc $< -o $@
	
particle/frag2D.spv : particle/particle2D.frag
	glslc $< -o $@

particle/vert2D.spv : particle/particle2D.vert
	glslc $< -o $@

particle/frag3D.spv : particle/particle3D.frag
	glslc $< -o $@

particle/vert3D.spv : particle/particle3D.vert
	glslc $< -o $@

quad/frag_3.spv : quad/shader_3.frag
	glslc $< -o $@

quad/frag_2.spv : quad/shader_2.frag
	glslc $< -o $@

quad/frag.spv : quad/shader.frag
	glslc $< -o $@

quad/vert.spv : quad/shader.vert
	glslc $< -o $@

geometry/frag.spv : geometry/shader.frag
	glslc $< -o $@

geometry/vert.spv : geometry/shader.vert
	glslc $< -o $@

sky/sky_frag.spv : sky/sky_shader.frag
	glslc $< -o $@

sky/sky_vert.spv : sky/sky_shader.vert
	glslc $< -o $@

sky/stars_frag.spv : sky/stars_shader.frag
	glslc $< -o $@

sky/stars_vert.spv : sky/stars_shader.vert
	glslc $< -o $@

sky/atmospheric_frag.spv : sky/atmospheric_shader.frag
	glslc $< -o $@

sky/atmospheric_vert.spv : sky/atmospheric_shader.vert
	glslc $< -o $@

sprite/frag.spv : sprite/shader.frag
	glslc $< -o $@

sprite/vert.spv : sprite/shader.vert
	glslc $< -o $@

terrain/frag.spv : terrain/shader.frag
	glslc $< -o $@

terrain/vert.spv : terrain/shader.vert
	glslc $< -o $@

terrain/tesc.spv : terrain/shader.tesc
	glslc $< -o $@

terrain/tese.spv : terrain/shader.tese
	glslc $< -o $@
	
terrain/shadow_frag.spv : terrain/shader_shadow.frag
	glslc $< -o $@

terrain/shadow_vert.spv : terrain/shader_shadow.vert
	glslc $< -o $@

tree/frag.spv : tree/shader.frag
	glslc $< -o $@

tree/vert.spv : tree/shader.vert
	glslc $< -o $@

tree/sdf_frag.spv : tree/shader_sdf.frag
	glslc $< -o $@

tree/sdf_vert.spv : tree/shader_sdf.vert
	glslc $< -o $@

tree/instance_frag.spv : tree/shader_instance.frag
	glslc $< -o $@

tree/instance_vert.spv : tree/shader_instance.vert
	glslc $< -o $@

default/frag2D.spv : default/shader2D.frag
	glslc $< -o $@

default/vert2D.spv : default/shader2D.vert
	glslc $< -o $@
	
default/frag3D.spv : default/shader3D.frag
	glslc $< -o $@

default/vert3D.spv : default/shader3D.vert
	glslc $< -o $@

image/vert.spv : image/shader.vert
	glslc $< -o $@

image/frag.spv : image/shader.frag
	glslc $< -o $@

text/frag.spv : text/shader.frag
	glslc $< -o $@

text/vert.spv : text/shader.vert
	glslc $< -o $@

clean: $(obj)
	$(RM) $^


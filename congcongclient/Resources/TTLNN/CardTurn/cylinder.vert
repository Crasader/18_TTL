attribute vec4 a_position;
attribute vec2 a_texCoord;
#ifdef GL_ES
varying highp vec2 v_texture_coord;
varying highp vec4 teture_position;
varying highp float color_z;
uniform highp float radius;
#else
varying vec2 v_texture_coord;
varying vec4 teture_position;
varying float color_z;
uniform float radius;
#endif

void main(void)
{
    gl_Position = CC_MVPMatrix * a_position;
	teture_position = gl_Position;
    v_texture_coord = a_texCoord;
    v_texture_coord.y = (1.0-v_texture_coord.y);
	
	color_z=a_position.x/radius;
}

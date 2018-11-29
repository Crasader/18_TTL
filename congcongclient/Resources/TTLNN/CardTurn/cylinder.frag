#ifdef GL_ES
varying highp vec2 v_texture_coord;
varying highp vec4 teture_position;
varying highp float color_z;
uniform highp float offset;
#else
varying vec2 v_texture_coord;
varying vec4 teture_position;
varying float color_z;
uniform float offset;
#endif

uniform vec4 u_color;
uniform sampler2D u_sampler0;

void main(void)
{
	gl_FragColor=u_color*texture2D(u_sampler0, vec2(v_texture_coord.x+offset,v_texture_coord.y)) * vec4(1.0-color_z, 1.0-color_z, 1.0-color_z, 1.0);
}

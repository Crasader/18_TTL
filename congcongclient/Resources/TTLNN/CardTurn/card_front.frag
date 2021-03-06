#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif

uniform vec4 u_color;
uniform sampler2D u_sampler0;

void main(void)
{
    gl_FragColor = u_color*texture2D(u_sampler0, vec2(v_texture_coord.x, 1.0-v_texture_coord.y))*vec4(0.95, 0.95, 0.95, 1.0);
}

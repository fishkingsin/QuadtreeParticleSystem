uniform sampler2D tex;

void main (void) {
	gl_FragColor = texture2D(tex, vec2(gl_TexCoord[0].xy)*0.25) * gl_Color;
}

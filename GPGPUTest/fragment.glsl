uniform int initialized;
uniform sampler2D tex;

// Initialize the image
void init()
{
	 vec2 texCoord = gl_TexCoord[0].xy;
	 if (texCoord.x > 0.4 && texCoord.x < 0.6 && texCoord.y > 0.4 && texCoord.y < 0.6) {
		 gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
	 } else {
		 gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	 }
}

// update the color of each pixel
void update()
{
	const float offset = 1.0 / 512.0;
	vec2 texCoord = gl_TexCoord[0].xy;

	// get the colors of the surrounding pixels
	vec4 c  = texture2D(tex, texCoord);
	vec4 l  = texture2D(tex, texCoord + vec2(-offset,     0.0));
	vec4 t  = texture2D(tex, texCoord + vec2(    0.0,  offset));
	vec4 r  = texture2D(tex, texCoord + vec2( offset,     0.0));
	vec4 b  = texture2D(tex, texCoord + vec2(    0.0, -offset));

	// use the heat equation to updte the color of this pixel
	gl_FragColor = c + (l + t + r + b - c * 4.0) * 0.2;
}

void main()
{
	if (initialized == 0) {
		init();
	} else {
		update();
	}
}
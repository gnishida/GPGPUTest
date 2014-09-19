uniform float hoge;
uniform int initialized;
uniform sampler2D texUnit;


vec4 rgb(float heat)
{
	vec4 color;
	/*
	color.b = min(1, max(0, 2.0 - heat * 3.0));
	color.r = min(1, max(0, heat * 3.0 - 1.0));
	color.g = 1.0 - color.b - color.r;
	*/
	if (heat < 0.33333) {
		color.b = 1.0;
		color.r = 0.0;
		color.g = heat * 3.0;
	} else if (heat > 0.66666) {
		color.b = 0.0;
		color.r = 1.0;
		color.g = 3 - heat * 3.0;
	} else {
		color.b = 2 - heat * 3.0;
		color.r = heat * 3.0 - 1;
		color.g = 1.0;
	}

	return color;
}

float heat(vec4 color)
{
	if (color.b > color.r && color.b > color.g) {
		return min(1, color.g * 0.33334);
	} else if (color.r > color.b && color.r > color.g) {
		return min(1, 1 - color.g * 0.33334);
	} else {
		return min(1, (color.r + 1) * 0.33334);
	}
}

void init()
{
	 vec2 texCoord = gl_TexCoord[0].xy;
	 if (texCoord.x > 0.4 && texCoord.x < 0.6 && texCoord.y > 0.4 && texCoord.y < 0.6) {
		 gl_FragColor = rgb(1.0);
	 } else {
		 gl_FragColor = rgb(0.0);
	 }
}

void update()
{
	const float offset = 1.0 / 512.0;
	vec2 texCoord = gl_TexCoord[0].xy;

	vec4 c  = texture2D(texUnit, texCoord);
	vec4 l  = texture2D(texUnit, texCoord + vec2(-offset,     0.0));
	vec4 t  = texture2D(texUnit, texCoord + vec2(    0.0,  offset));
	vec4 r  = texture2D(texUnit, texCoord + vec2( offset,     0.0));
	vec4 b  = texture2D(texUnit, texCoord + vec2(    0.0, -offset));
	float new_heat = heat(c) + (heat(l) + heat(t) + heat(r) + heat(b) - heat(c) * 4) * 0.2;
	if (new_heat < 0) new_heat = 0;
	if (new_heat > 1) new_heat = 1;
	gl_FragColor = rgb(new_heat);
}

void main()
{
	if (initialized == 0) {
		init();
	} else {
		update();
	}
}
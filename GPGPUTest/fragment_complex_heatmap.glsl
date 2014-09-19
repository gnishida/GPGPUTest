uniform float hoge;
uniform int initialized;
uniform sampler2D texUnit;


vec4 rgb(float heat)
{
	vec4 color;
	if (heat < 0.125) {
		color = vec4(0.0, 0.0, 0.5 + heat * 4.0, 0);
	} else if (heat < 0.375) {
		color = vec4(0.0, heat * 4.0 - 0.5, 1.0, 0);
	} else if (heat < 0.625) {
		color = vec4(heat * 4.0 - 1.5, 1, 2.5 - heat * 4.0, 0.0);
	} else if (heat < 0.875) {
		color = vec4(1.0, 3.5 - heat * 4.0, 0.0, 0.0);
	} else {
		color = vec4(4.5 - heat * 4.0, 0.0, 0.0, 0.0);
	}

	return color;
}

float heat(vec4 color)
{
	if (color.g == 0.0 && color.r == 0.0) {
		return (color.b - 0.5) / 4.0;
	} else if (color.r == 0.0) {
		return (color.g + 0.5) / 4.0;
	} else if (color.g == 0.0 && color.b == 0.0) {
		return (4.5 - color.r) / 4.0;
	} else if (color.b == 0.0) {
		return (3.5 - color.g) / 4.0;
	} else {
		return (1.5 + color.r) / 4.0;
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
	float new_heat = heat(c) + (heat(l) + heat(t) + heat(r) + heat(b) - heat(c) * 4.0) * 0.1;
	if (new_heat < 0.0) new_heat = 0.0;
	if (new_heat > 1.0) new_heat = 1.0;
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
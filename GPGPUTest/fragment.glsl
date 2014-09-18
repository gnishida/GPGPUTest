uniform float hoge;
uniform sampler2D texUnit;

void main(void)
{
   const float offset = 1.0 / 512.0;
   vec2 texCoord = gl_TexCoord[0].xy;
   vec4 c  = texture2D(texUnit, texCoord);
   vec4 bl = texture2D(texUnit, texCoord + vec2(-offset, -offset));
   vec4 l  = texture2D(texUnit, texCoord + vec2(-offset,     0.0));
   vec4 tl = texture2D(texUnit, texCoord + vec2(-offset,  offset));
   vec4 t  = texture2D(texUnit, texCoord + vec2(    0.0,  offset));
   vec4 ur = texture2D(texUnit, texCoord + vec2( offset,  offset));
   vec4 r  = texture2D(texUnit, texCoord + vec2( offset,     0.0));
   vec4 br = texture2D(texUnit, texCoord + vec2( offset,  offset));
   vec4 b  = texture2D(texUnit, texCoord + vec2(    0.0, -offset));
   gl_FragColor = vec4(0, 0, hoge, 0);
}
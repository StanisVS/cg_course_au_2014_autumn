uniform sampler1D tex;
uniform float cx;
uniform float cy;
uniform float fscale;
uniform int iter;

void main() {
    vec2 z;
    z.x = 3.0 * (gl_TexCoord[0].x - 0.5)/fscale;
    z.y = 2.0 * (gl_TexCoord[0].y - 0.5)/fscale;
    int i;
    int iter = 1000;

    for(i = 0; i < iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + cx;
        float y = (z.y * z.x + z.x * z.y) + cy;
        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    gl_FragColor = vec4(float(i) / 100.0, float(i) / 200.0, float(i) / 300.0,1);
}

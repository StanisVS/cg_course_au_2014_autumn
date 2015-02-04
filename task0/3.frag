varying vec2 pos;

void main(void)
{
    vec2 pos = pos / 0.5;
    if((fract(pos.x) < 0.5 && fract(pos.y) < 0.5) ||
       (fract(pos.x) > 0.5 && fract(pos.y) > 0.5)){
      gl_FragColor = vec4(0,0,0,1.0);
    } else{
      gl_FragColor = vec4(1,1,1,1.0);
    }
}

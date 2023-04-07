#version 430
out vec4 outColor;
uniform dvec2 resolution;
uniform dvec4 range;
uniform dvec2 c;
void main() {
    dvec2 coor = gl_FragCoord.xy / resolution.xy;
    dvec2 z = dvec2(range.x + coor.x * (range.z - range.x), range.y + coor.y * (range.w - range.y));
    double iterations = 50.0;
    for(int i = 0; i < iterations; i++)
    {
        z = dvec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        double r = z.x * z.x + z.y * z.y;
        if(r > 4)
        {
            double value = i/iterations;
            if(value > 0.75)
            {
                value = (value - 0.75) * 4;
                outColor = vec4(1-value,0.0, 0.0 , 1.0);
            }
            else if(value > 0.5)
            {
                value = (value - 0.5) * 4;
                outColor = vec4(0.0,1-value, 0.0 , 1.0);
            }
            else if(value > 0.25)
            {
                value = (value) *2;
                
                outColor = vec4(1-value,0.0 ,1-value, 1.0);
            }
            else
            {
                value = value * 4;
                outColor = vec4(0.0,0.0, 1-value , 1.0);
            }
            return;
        }
    }
    double r = z.x * z.x + z.y * z.y;
    outColor = vec4(0.0,0.0,0.0, 1.0);

}


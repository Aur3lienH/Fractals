#version 430
out vec4 outColor;
uniform dvec2 resolution;
uniform dvec4 range;
uniform double iterations;

void main() {
    dvec2 coor = gl_FragCoord.xy / resolution.xy;
    dvec2 c = dvec2(range.x + coor.x * (range.z - range.x), range.y + coor.y * (range.w - range.y));
    dvec2 z = dvec2(0.0, 0.0);
    for(int i = 0; i < iterations; i++)
    {
        z = dvec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        double r = z.x * z.x + z.y * z.y;
        if(r > 4)
        {
            double value = (i + 1.0 - log(log(float(sqrt(z.x * z.x + z.y * z.y))))/log(float(2.0)));
            value = (value / iterations) * 1/100 * iterations;
            outColor = vec4(value, value, value, 1.0);
            return;

        }
    }
    outColor = vec4(0.0,0.0,0.0, 1.0);

}

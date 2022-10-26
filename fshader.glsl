#version 430

in vec2 tc;

out vec4 color;
void main()
{
    float r, g, b;
    r = (sin(tc.x * 2.0f) + 1.0f) / 2.0f;
    g = (cos(tc.y * 2.0f) + 1.0f) / 2.0f;
    b = 1.0f;
    color = vec4(r, g, b, 1.0f);
}



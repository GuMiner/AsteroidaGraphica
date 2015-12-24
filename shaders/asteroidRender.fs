#version 400 core

out vec4 color;

in VS_OUT
{
    vec4 color;
    vec3 barycentricPosition;
} fs_in;

void main(void)
{
    vec4 inpColor = fs_in.color;

    float min = 0.577350f; // Minimum possible barycentric position.
    float dist = (length(fs_in.barycentricPosition) - min) / (1.0f - min);

    inpColor.a = round(dist * 10.0f) / 10.0f;
    if (inpColor.a < 0.30f)
    {
        // Make physical cutouts in our asteroids, which looks awesome.
        discard;
    }
    else
    {
        // Make everything else completely solid, but darkened appropriate.y
        inpColor.r *= inpColor.a * 2.0;
        inpColor.g *= inpColor.a * 2.0;
        inpColor.b *= inpColor.a * 2.0;
        inpColor.a = 1.0f;
    }

    color = inpColor;
}
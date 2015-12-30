#version 400 core

out vec4 color;

in VS_OUT
{
	float viewDistance;
    vec4 color;
	vec4 highlightColor;
    vec4 barycentricPosition;
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
		inpColor = fs_in.highlightColor;
    }
    else
    {
        // Make everything else completely solid, but darkened appropriately
        inpColor.r *= inpColor.a * 2.0;
        inpColor.g *= inpColor.a * 2.0;
        inpColor.b *= inpColor.a * 2.0;
        inpColor.a = 1.0f;
    }

	// Apply fog.
	float extinctionFogFactor = 0.022f;
	float inscatteringFogFactor = 0.011f;
	vec4 fogColor = vec4(0.7f, 0.8f, 0.9f, 0.0f);
	float extinction = exp(-fs_in.viewDistance*extinctionFogFactor);
	float inscattering = exp(-fs_in.viewDistance*inscatteringFogFactor);

    color = inpColor*extinction + fogColor*(1.0f - inscattering);
}
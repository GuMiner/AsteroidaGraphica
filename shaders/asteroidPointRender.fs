#version 400 core

out vec4 color;

in VS_OUT
{
    vec4 color;
} fs_in;

void main(void)
{
	float halfCoordX = gl_PointCoord.x - 0.5f;
	float halfCoordY = gl_PointCoord.y - 0.5f;
	if (halfCoordX*halfCoordX + halfCoordY*halfCoordY > 0.32) // 0.4
	{
		discard;
	}

	// Pass-through color fragment shader.
	float freq = 2.0f;
	float factorX = cos(freq*halfCoordX);
	float factorY = cos(freq*halfCoordY);	

    color = vec4(factorX*factorY*fs_in.color.xyz, 1.0f);
}
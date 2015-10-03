float4 main( float4 pos : POSITION, uint instanceID : SV_InstanceID) : SV_POSITION
{
	static const float PI = 3.14159265f;
	float angle = instanceID * 5 * PI / 180.0;
	pos.x *= 0.3;
	pos.y *= 0.3;

	pos.x += 0.8*cos(angle);
	pos.y += 0.8*sin(angle);

	return pos;
}
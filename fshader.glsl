#version 460

#define EPSILON 0.001
#define MAX_STEP 255
#define CLIP_DIST 1000.0

in vec2 tc;
uniform uint time;

out vec4 fragColor;

float
sphere_sdf(vec3 sphere_pos, float sphere_radius, vec3 p)
{
    return length(sphere_pos - p) - sphere_radius;
}

float
dist_estimate(vec3 p) {
	return sphere_sdf(vec3(0.0, 0.0, -3.0), 1.0, p);
}

vec3
norm_estimate(vec3 p) {
	return normalize(vec3(
		dist_estimate(vec3(p.x + EPSILON, p.y, p.z)) - dist_estimate(vec3(p.x - EPSILON, p.y, p.z)),
		dist_estimate(vec3(p.x, p.y + EPSILON, p.z)) - dist_estimate(vec3(p.x, p.y - EPSILON, p.z)),
		dist_estimate(vec3(p.x, p.y, p.z + EPSILON)) - dist_estimate(vec3(p.x, p.y, p.z - EPSILON))
	));
}

void main()
{
	fragColor = vec4(0.3,0.5,0.6,1.0);
	vec2 res = vec2(720.0, 480.0);
	vec2 fragCoord = tc * res;
	vec2 p = (2.0*(fragCoord.xy + 0.5)-res.xy)/res.y;

	vec3 ro = vec3(0.0, 0.0, 0.0);
	vec3 rd = normalize(vec3(p, -1.0));
	vec3 light_pos = vec3(5.0 * sin(time/1000.0), 1.0, 1.0);

	float dist = CLIP_DIST;
	for (int i = 0; i < MAX_STEP; i++) {
		dist = dist_estimate(ro);
		ro += rd * dist;
	}

	if (dist < EPSILON) {
		fragColor.xyz = vec3(0.882, 0.901, 0.717);
		vec3 light_dir = normalize(light_pos - ro);
		vec3 normal = norm_estimate(ro);

		vec3 R = normalize(2.0 * dot(light_dir, normal) * normal - light_dir);
		vec3 V = normalize(-ro);

		fragColor.xyz *= 0.4 * dot(norm_estimate(ro), light_dir) + 0.4 * max(0.0, pow(dot(R, V), 10.0)) + 0.5;
	}
}

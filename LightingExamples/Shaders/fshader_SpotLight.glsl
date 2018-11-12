#version 330
				
in vec4 fs_color;
in vec3 fs_normal;
in vec3 fs_eye_dir;

layout (location = 0) out vec4 color;

uniform vec3 light_position;
uniform vec3 light_dir;
uniform float light_cos_cutoff;
uniform vec3 I_l;       // light intensity
float I_a = 1.0;				  // ambient light intensity

uniform float ambient_factor = 0.25f;
uniform float shininess_n;	
uniform vec3 K_s;

void main()
{
	vec3 K_a = ambient_factor * fs_color.rgb;
	vec3 K_d = fs_color.rgb;

	vec3 L = normalize(light_position-fs_eye_dir);
	vec3 N = normalize(fs_normal);
	vec3 V = normalize(-fs_eye_dir);
	vec3 H = (L+V)/length(L+V);
	vec3 Sd = normalize(-light_dir);

	
	vec3 ambient = I_a * K_a;
	vec3 diffuse = I_l * K_d * max(0.f, dot(L, N));
	vec3 specular = I_l * K_s * pow(max(0.f, dot(N, H)), shininess_n);

	if ( dot(Sd,L) < light_cos_cutoff )
	{
		diffuse = vec3(0, 0, 0);
		specular = vec3(0, 0, 0);
	}

	color.rgb = ambient + diffuse + specular;
	color.rgb = (vec3(1.f, 1.f, 1.f), color.rgb);
	color.a = fs_color.a;
}


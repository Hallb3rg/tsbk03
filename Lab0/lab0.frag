/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

//in float shade;

in vec3 normal;
in vec3 frag_Position;
in vec2 frag_texcoord;
out vec4 out_Color;
uniform sampler2D exampletexture;

void main(void)
{
    // Light sources
    vec3 light[3];
    light[0] = vec3(0.8, 0.8, 0.8);
    light[1] = vec3(-0.8, 0.0, -0.8);
    light[2] = vec3(0.8, 0.0, -0.8);
    vec3 lightcolor[3];
    lightcolor[0] = vec3(1.0, 0.0, 0.);
    lightcolor[1] = vec3(0.0, 1.0, 0.0);
    lightcolor[2] = vec3(0.0, 0.0, 1.0);
    bool isDirectional[3];
    isDirectional[0] = true;
    isDirectional[1] = false;
    isDirectional[2] = false;
    
    const int spec_n = 20;
    
    const vec3 amb = vec3(1.0, 1.0, 1.0);
    
    vec3 r, s, v, tot_i;

    tot_i = vec3(0);

    for (int i = 0; i < 3; i++) {
        if(!isDirectional[i]) {
            s = normalize(frag_Position - light[i]);
        } else {
            s = -normalize(light[i]);
        }
        r = 2*normalize(normal)*(dot(s, normalize(normal))) - s;
    
        v = -frag_Position;

        float cos_phi = dot(normalize(r), normalize(v));

        //Diffuse shading
        vec3 diffuse = vec3(dot(normalize(normal), light[i]));
        diffuse = clamp(diffuse, 0, 1);
	
        //Specular
        vec3 specular = vec3(1*1*pow(cos_phi, spec_n)); 
        specular = clamp(specular, 0, 1);
        // OOOOOBBBS!!! THIS LINE DISABLES SPEC!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //specular = vec3(0.0);
        tot_i = tot_i + lightcolor[i]*(diffuse + specular);
    }

    tot_i = tot_i + amb;

    out_Color=vec4(tot_i, 1.0) * texture(exampletexture,frag_texcoord);
}


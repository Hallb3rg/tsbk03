#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions 
    

    mat3 Mvt = transpose(mat3(Ps, Pt, out_Normal));

    light = Mvt*light;
       

    // get the 3 textures needed to calculate bs and bt

    vec4 bump_original = texture(texUnit, outTexCoord);
    vec4 bump_dx = texture(texUnit, vec2(outTexCoord.x+offset,outTexCoord.y));
    vec4 bump_dy = texture(texUnit, vec2(outTexCoord.x,outTexCoord.y+offset));

    // compares original to modifed texture to find bs and bt
    

    float bs = bump_original.r - bump_dx.r;
    float bt = bump_original.r - bump_dy.r;

    //calculated normal according to book/slides.

    //vec3 normal = normalize(out_Normal + bs*cross(Pt,out_Normal) + bt*cross(out_Normal, Ps));
    
    vec3 normal = vec3(bs,bt,1);

    normal = normalize(normal);
    //vec3 normal = normalize(Mvt*out_Normal);
	
    // Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = vec4( dot(normal, light)); //* texture(texUnit, outTexCoord);
}

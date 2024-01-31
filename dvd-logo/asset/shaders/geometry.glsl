#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec4 ModelPosition;
out vec2 Tex;

in vec4 Posi[];
in vec2 ttex[];


vec3 createNormal(vec3 v1, vec3 v2, vec3 v3)
{
    vec3 edge1 = v2 - v1;
    vec3 edge2 = v3 - v1;
    vec3 normal = normalize(cross(edge1, edge2));
			
    return normal;
}

void main()
{
    Normal = createNormal( vec3(Posi[0]),  vec3(Posi[1]),  vec3(Posi[2]) );

    gl_Position = gl_in[0].gl_Position;
    ModelPosition = Posi[0];
    Tex = ttex[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    ModelPosition = Posi[1];
        Tex = ttex[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    ModelPosition = Posi[2];
        Tex = ttex[2];

    EmitVertex();
    EndPrimitive();
}

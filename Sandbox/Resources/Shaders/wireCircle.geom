#version 450

layout(points) in;
layout(line_strip, max_vertices = 128) out;
//layout(invocations = 4) in;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

in VS_Out
{
    vec4 Colour;
    float Radius;
    int PointCount;
} ge_In[];

out vec4 circleColour;

vec2 AngleToDir(float angleRad)
{
    return vec2(cos(angleRad), sin(angleRad));
}

void main()
{
    circleColour = ge_In[0].Colour;

    //pi * 2
    const float TAU = 6.28318530718; //Full turn in radians

    int PointCount = ge_In[0].PointCount;
    PointCount = clamp(PointCount, 0, 127);
    //const int PointsPerInstance = PointCount;
    //const int RemainingInstances = (4 - 1) - gl_InvocationID;

    //const int RenderFrom = PointsPerInstance * gl_InvocationID;
    //const int RenderTill = PointCount - RemainingInstances * PointsPerInstance;
    for(int i = 0; i <= PointCount; ++i)
    {
        const float t = float(i) / float(PointCount);
        const float angleRad = t * TAU;
        vec2 point = AngleToDir(angleRad);

        //scale the point
        point *= ge_In[0].Radius;
        gl_Position = u_Projection * u_View * ( gl_in[0].gl_Position + vec4(point, 0.0, 0.0));
        EmitVertex();
    }
    EndPrimitive();
}
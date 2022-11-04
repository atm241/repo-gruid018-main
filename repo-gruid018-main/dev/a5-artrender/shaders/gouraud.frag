#version 330

// This color comes in from the output of the vertex shader stage.  The current
// fragment will lie somewhere within a triangle.  So, the vec4 that is passed
// in here is actually an interpolated version of the colors output by the 3
// vertex shader programs run for the 3 vertices of the triangle.
in vec4 color;

in vec2 tex_coords;

// All fragment shaders are required to output a vec4 color.
out vec4 final_color;

uniform sampler2D surface_texture;

void main() {

    vec4 tex_color = texture(surface_texture, tex_coords);

    final_color = tex_color;

    // For a Gouraud shader, there is nothing more to compute at this stage.  We
    // just output the input color.
    // final_color = color;
}

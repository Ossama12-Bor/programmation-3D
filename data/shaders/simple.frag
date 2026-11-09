#version 330 core

out vec4 out_color;
in vec3 var_color;

uniform int choice_exo;


void main(void) 
{
  out_color = vec4(var_color, 1);
}

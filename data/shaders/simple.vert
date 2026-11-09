#version 330 core

in vec3 vtx_position;
in vec3 vtx_normal;
out vec3 var_color;
//in vec3 vtx_color;
//TODO ADD transformation matrices object and view
uniform mat4 obj_mat;
uniform mat4 view_mat;
// This is the projection matrix
uniform mat4 project_mat;


uniform int choice_exo;

void main()
{
  vec4 p;

  switch (choice_exo) {

    // CHAIR
    case 1:
      // TODO , CHANGE the computation of p according to transformation matrix obj_mat
      p = vec4(vtx_position,1);
      gl_Position = obj_mat * p;
      var_color = vec3(0.5);
      break;

    // SOLAR_SYSTEM OR LEMMING OR MONKEY CASE
    case 2:
    case 3:
      // TODO , CHANGE the computation of p  according to transformation matrices
      p = vec4(vtx_position,1);
      gl_Position = project_mat * view_mat * obj_mat * p;
      // TODO : set var_color appropriately
      var_color = vtx_normal;
      break;

    default:
      break;

  }

}

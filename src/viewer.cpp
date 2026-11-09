#include "viewer.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
  : _winWidth(0), _winHeight(0),_scale(1),CHOICE_EXERCICE(1),moon_theta(0),earth_theta(0),thetaRotation_earth(0),thetaRotation_moon(0)
{

}

Viewer::~Viewer()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff



// choiceExercice : 1 (CHAIR), 2 (SOLAR_SYSTEM), 3(MONKEY)




// initialize OpenGL context
void Viewer::init(int w, int h){
    loadShaders();

    switch (CHOICE_EXERCICE) {
      case 1:
        if(!_mesh.load(DATA_DIR"/models/chair.off")) exit(1);
        break;
      case 2:
        if(!_mesh.load(DATA_DIR"/models/sphere.obj")) exit(1);
        break;
      case 3:
        // CHOOSE ONE of the MESH
        if (!_mesh.load(DATA_DIR "/models/lemming.off"))
        //if (!_mesh.load(DATA_DIR "/models/monkey2.obj")) exit(1);
        break;
      default:
        break;
    }



    _mesh.initVBA();

    reshape(w,h);
    _trackball.setCamera(&_cam);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.2, 0.2, 0.2, 0.5);
}

void Viewer::reshape(int w, int h){
    _winWidth = w;
    _winHeight = h;
    _cam.setViewport(w,h);
    // Positionne la caméra pour une vue de 3/4
    _cam.lookAt(Vector3f(1,1,1),Vector3f(0,0,0),Vector3f(0,1,0));

    _cam.lookAt(Vector3f(0,0,5),Vector3f(0,0,0),Vector3f(0,1,0));
    glViewport(0, 0, _winWidth, _winHeight);

}

void Viewer::sceneChair() {

  // TODO 

  // ADD TRANSFORMATION 
  _scale = 0.5;
  // chair N° 1 
  Matrix4f T1 = Matrix4f::Identity();
  T1(0,3) = -0.5;
  T1(1,3) = -1;
  Matrix4f S = Matrix4f::Identity();
  S(0,0) = _scale;
  S(1,1) = _scale;
  S(2,2) = _scale;
  Matrix4f M1 = T1 * S;
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M1.data());
  _mesh.draw(_shader);

  // chair N° 2
  Matrix4f R1 = Matrix4f::Identity();
  R1.block<3,3>(0,0) = AngleAxisf(M_PI, Vector3f::UnitY()).toRotationMatrix();
  Matrix4f T2 = Matrix4f::Identity();
  T2(0,3) = 0.5;
  T2(1,3) = -1;  
  Matrix4f M2 = T2 * R1 * S;
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M2.data());
  _mesh.draw(_shader);

  // chair N° 3
  Matrix4f TFC = Matrix4f::Identity();
  TFC(1,3) = -0.5f;
  Matrix4f R2 = Matrix4f::Identity();
  R2.block<3,3>(0,0) = AngleAxisf(_theta, Vector3f::UnitZ()).toRotationMatrix();
  Matrix4f TBC = Matrix4f::Identity();
  TBC(1,3) = 0.5f;
  Matrix4f M3 = TBC * R2 * TFC;
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M3.data());
  _mesh.draw(_shader);


  


 
}


void Viewer::sceneSolarSystem() {
  Matrix4f M = Matrix4f::Identity();
  
  //TODO ADD TRANSFORMATION MATRICES
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M.data());
  glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
  glUniformMatrix4fv(_shader.getUniformLocation("project_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());
  _mesh.draw(_shader);
  // EARTH
  float r = 3.0f;
  Matrix4f R_earth = Matrix4f::Identity();

  R_earth.block<3,3>(0,0) = AngleAxisf(earth_theta, Vector3f::UnitY()).toRotationMatrix();
  earth_theta +=0.02;

  Matrix4f T_earth = Matrix4f::Identity();
  T_earth(0,3) = r; 

  Matrix4f S_earth = Matrix4f::Identity();
  S_earth(0,0) = 0.5;
  S_earth(1,1) = 0.5;
  S_earth(2,2) = 0.5;

  float inclinaison = 23.0f * M_PI/180.0f; 
  Vector3f axisRotation(sin(inclinaison), cos(inclinaison), 0.0f);
  axisRotation.normalize();

  Matrix4f R_rotation_earth = Matrix4f::Identity();
  R_rotation_earth.block<3,3>(0,0) = AngleAxisf(thetaRotation_earth, axisRotation).toRotationMatrix();
  thetaRotation_earth += 0.2;

  Matrix4f M_earth = R_earth * T_earth * R_rotation_earth * S_earth;

  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M_earth.data());
  _mesh.draw(_shader);

  // MOON

  float r_moon = 1.0f;

  Matrix4f T_Moon = Matrix4f::Identity();
  T_Moon(0,3) = r_moon; 

  Matrix4f S_moon = Matrix4f::Identity();
  S_moon(0,0) = 0.25;
  S_moon(1,1) = 0.25;
  S_moon(2,2) = 0.25;


  float inclinaison_moon = 6.68f * M_PI/180.0f;
  Matrix4f R_rotation_moon = Matrix4f::Identity();
  R_rotation_moon.block<3,3>(0,0) = AngleAxisf(thetaRotation_moon, Vector3f(sin(inclinaison_moon), cos(inclinaison_moon), 0.0f)).toRotationMatrix();
  thetaRotation_moon += 0.4;
  
  Matrix4f R_Moon = Matrix4f::Identity();
  float inclinaison_moon_orbit = 5.14 * M_PI/180.0f;
  R_Moon.block<3,3>(0,0) = AngleAxisf(moon_theta,Vector3f(sin(inclinaison_moon_orbit), -cos(inclinaison_moon_orbit), 0.0f) ).toRotationMatrix();
  moon_theta +=0.3;

  Matrix4f M_Moon =   R_earth * T_earth * R_Moon * T_Moon * R_rotation_moon * S_moon;

  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M_Moon.data());
  _mesh.draw(_shader);



}

void Viewer::sceneMesh()
{
  Matrix4f M;
  M.setIdentity();
  
  // TODO APPLY AXIS ROTATION to M 
  Matrix4f R1 = Matrix4f::Identity();
  R1.block<3,3>(0,0) = AngleAxisf(_theta, Vector3f(1,0,0)).toRotationMatrix();

  Vector3f axis(1,1,1);
  axis.normalize();
  Matrix4f R2 = Matrix4f::Identity();
  R2.block<3,3>(0,0) = AngleAxisf(_theta, axis).toRotationMatrix();

  Vector3f v(0,1,0);
  Vector3f u(1,1,1);
  u.normalize();
  Vector3f axis2 = v.cross(u).normalized();
  float theta = acos(v.dot(u));  
 
  M.block<3,3>(0,0) = AngleAxisf(theta, axis2).toRotationMatrix();
  // TODO:  Pass matrices obj_mat, view_mat, project_at to  SHADER _shader
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M.data());
  glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
  glUniformMatrix4fv(_shader.getUniformLocation("project_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());


  

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  _mesh.draw(_shader);

}


/*!
   callback to draw graphic primitives
 */
void Viewer::drawScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthFunc(GL_LESS);
  _shader.activate();

  glUniform1i(_shader.getUniformLocation("choice_exo"), CHOICE_EXERCICE);


  switch (CHOICE_EXERCICE) {
    case 1:
      sceneChair();
      break;
    case 2:
      sceneSolarSystem();
      break;
    case 3:
      sceneMesh();
      break; 
   default:
      break;

  }
  _shader.deactivate();
}

void Viewer::drawScene2D()
{
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _shader.activate();
  _scale = 0.5;
  glUniform1i(_shader.getUniformLocation("choice_exo"), CHOICE_EXERCICE);

  // chair N° 1 
  Matrix4f T1 = Matrix4f::Identity();
  T1(0,3) = -0.5;
  T1(1,3) = -1;
  Matrix4f S = Matrix4f::Identity();
  S(0,0) = _scale;
  S(1,1) = _scale;
  S(2,2) = _scale;
  Matrix4f M1 = T1 * S;
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M1.data());
  _mesh.draw(_shader);

  // chair N° 2
  Matrix4f R1 = Matrix4f::Identity();
  R1.block<3,3>(0,0) = AngleAxisf(M_PI, Vector3f::UnitY()).toRotationMatrix();
  Matrix4f T2 = Matrix4f::Identity();
  T2(0,3) = 0.5;
  T2(1,3) = -1;  
  Matrix4f M2 = T2 * R1 * S;
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M2.data());
  _mesh.draw(_shader);

  // chair N° 3
  Matrix4f TFC = Matrix4f::Identity();
  TFC(1,3) = -0.5f;
  Matrix4f R2 = Matrix4f::Identity();
  R2.block<3,3>(0,0) = AngleAxisf(_theta, Vector3f::UnitZ()).toRotationMatrix();
  Matrix4f TBC = Matrix4f::Identity();
  TBC(1,3) = 0.5f;
  Matrix4f M3 = TBC * R2 * TFC;
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M3.data());
  _mesh.draw(_shader);


  


  _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
    drawScene();
}

void Viewer::loadShaders()
{
    // Here we can load as many shaders as we want, currently we have only one:
    _shader.loadFromFiles(DATA_DIR"/shaders/simple.vert", DATA_DIR"/shaders/simple.frag");
    checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::keyPressed(int key, int action, int /*mods*/)
{
  //std::cerr << "Key pressed: " << key << " action: " << action << std::endl;

  if(key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    loadShaders();
  }

  if(action == GLFW_PRESS || action == GLFW_REPEAT )
  {
    if (key==GLFW_KEY_UP)
    {
      _scale = _scale + 0.2f;
      _theta = _theta + 0.2f;
       _translation.y() = _translation.y()+0.1;

    }
    else if (key==GLFW_KEY_DOWN)
    {
      _scale = _scale -0.2f;
      _theta = _theta - 0.2f;
       //std::cerr << _translation.y() << std::endl;
      _translation.y() = _translation.y()-0.1;
    }
    else if (key==GLFW_KEY_LEFT)
    {
      //std::cerr << _translation.x() << std::endl;
      _translation.x() = _translation.x()-0.1;
    }
    else if (key==GLFW_KEY_RIGHT)
    {
      _translation.x() = _translation.x()+0.1;
    }
    else if (key==GLFW_KEY_PAGE_UP)
    {
      _scale = _scale + 0.2f;
    }
    else if (key==GLFW_KEY_PAGE_DOWN)
    {
      _scale = _scale - 0.2f;
    }
    else if(key == GLFW_KEY_P){
      _theta = _theta + 0.2;
    }
    else if(key == GLFW_KEY_N){
      _theta = _theta - 0.2;
    }
    else if(key == GLFW_KEY_KP_1){
      CHOICE_EXERCICE = 1 ;
      _mesh.clear();
      if(!_mesh.load(DATA_DIR"/models/chair.off")) exit(1);
      _mesh.initVBA();
    }
    else if(key == GLFW_KEY_KP_2){
      CHOICE_EXERCICE = 2 ;
      _mesh.clear();
      if(!_mesh.load(DATA_DIR"/models/sphere.obj")) exit(1);
      _mesh.initVBA();
    }
    else if(key == GLFW_KEY_KP_3){
      CHOICE_EXERCICE = 3 ;
      _mesh.clear();
      if(!_mesh.load(DATA_DIR"/models/lemming.off")) exit(1);
      _mesh.initVBA();
    }

  }
}

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mousePressed(GLFWwindow */*window*/, int /*button*/, int action)
{
  if(action == GLFW_PRESS)
  {
      _trackingMode = TM_ROTATE_AROUND;
      _trackball.start();
      _trackball.track(_lastMousePos);
  }
  else if(action == GLFW_RELEASE)
  {
      _trackingMode = TM_NO_TRACK;
  }
}


/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mouseMoved(int x, int y)
{
    if(_trackingMode == TM_ROTATE_AROUND)
    {
        _trackball.track(Vector2i(x,y));
    }

    _lastMousePos = Vector2i(x,y);
}

void Viewer::mouseScroll(double /*x*/, double y)
{
  _cam.zoom(-0.1*y);
}

void Viewer::charPressed(int /*key*/)
{
}

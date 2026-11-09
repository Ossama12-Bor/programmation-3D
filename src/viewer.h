#ifndef VIEWER_H
#define VIEWER_H

#include "opengl.h"
#include "shader.h"
#include "camera.h"
#include "trackball.h"
#include "mesh.h"

#include <iostream>

class Viewer{
public:
    //! Constructor
    Viewer();
    virtual ~Viewer();

    // gl stuff
    void init(int w, int h);
    void drawScene();
    void drawScene2D();
    void updateAndDrawScene();
    void reshape(int w, int h);
    void loadShaders();

    // events
    void mousePressed(GLFWwindow* window, int button, int action);
    void mouseMoved(int x, int y);
    void mouseScroll(double x, double y);
    void keyPressed(int key, int action, int mods);
    void charPressed(int key);

    void sceneChair();
    void sceneSolarSystem();
    void sceneMesh() ;
  
  private : int _winWidth, _winHeight;

    float _scale = 1;
    Eigen::Vector2f _translation = Eigen::Vector2f(0.0f,0.0f);

    Camera _cam;
    Shader _shader;
    Mesh   _mesh;
    float _theta =0.0f;
    int CHOICE_EXERCICE = 1;
    float moon_theta;
    float earth_theta;
    float thetaRotation_earth;
    float thetaRotation_moon;

    float _angle_rot_earth_around_sun = 0.0f;
    float _angle_rot_earth = 0.0f;


    // Mouse parameters for the trackball
    enum TrackMode
    {
      TM_NO_TRACK=0, TM_ROTATE_AROUND, TM_ZOOM,
      TM_LOCAL_ROTATE, TM_FLY_Z, TM_FLY_PAN
    };
    TrackMode _trackingMode = TM_NO_TRACK;
    Trackball _trackball;
    Eigen::Vector2i _lastMousePos;
};

#endif

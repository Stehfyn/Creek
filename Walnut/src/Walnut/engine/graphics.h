#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;
//(Part 1): Include all of our helper symbols that are necessary to render based off our
// model-view-projection matrix
#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "cube.h"
#include "triangle.h"
#include "Pyramid.h"
#include <stack>
#include "Mesh.h"
class Graphics
{
  public:
    Graphics();
    ~Graphics();
    // (Part 1): Initialize our camera member, initialize our shaders responsible for processing our vertices and indices,
    // instantiating the object members we wish to render. Also the initialization of our model, view, and projection members indicating their 
    // respective locations inside our shader (GLint)
    bool Initialize(int width, int height);
    // (Part 1): Update our game objects by sending them delta time and current speed to update based off of. delta time goes unused in this assignment as the time-step
    // is unfixed, however this implementation does use the speed parameter to update the position matrix of the each object
    void Update(float dt, glm::vec3 speed);
    // (Part 1): Rendering of each object. Currently this whole class is pretty static and thus is done manually for both object members(cube and triangle).
    // Essentially we clear the screen with a defined maroonish color as well as the depth buffer, enable our shader program for use on the gpu, send in our
    // current projection and view matrices to the uniform values seen by all individual programs being run on our vertexes, then render explicitly our triangle
    // and cube by passing in the current model matrices that are implicitly defined by an objects position, rotation, and scale matrices. Lastly, we check for any errors
    // opengl has to communicate. This uses our ErrorString() method as a helper.
    void Render();
    void ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale,
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);
    // (Part 1): Unused, returns base object pointer of our triangle member.
    Object* getInteractWith();
    // (Part 1): Explicit get<Object>() methods that return pointers our members. definitely an anti-pattern at the moment but for right now it just works™
    Triangle* getTriangle() { return m_triangle; }
    Cube* getCube() { return m_planet; }
  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_vertPos;
    GLint m_vertCol;

    Mesh* m_starship;
    Triangle* m_triangle;
    Cube* m_planet;
    Cube* m_moon1;
    Cube* m_moon2;

    Pyramid* m_sun;
};

#endif /* GRAPHICS_H */

#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

// (Part 1): See aforementioned in header comment
bool Graphics::Initialize(int width, int height)
{

  // For OpenGL 3

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  // Find where vertex attributes are in shader
  m_vertPos = m_shader->GetAttribLocation("v_position");
  m_vertCol = m_shader->GetAttribLocation("v_color");

  m_sun = new Sphere(100, glm::vec3(1.f, 1.f, .0f));
  m_sun->Initialize(m_vertPos, m_vertCol);
  m_sun->doRandomPositionOrientation();

  m_planet = new Sphere(100);
  m_planet->Initialize(m_vertPos, m_vertCol);

  m_starship = new Mesh("assets/starship.obj");
  m_starship->Initialize(m_vertPos, m_vertCol);

  m_gizmoX = new Mesh("assets/Gizmo.obj", glm::vec3(0.0f, 1.0f, 0.0f));
  m_gizmoX->Initialize(m_vertPos, m_vertCol);
  m_gizmoY = new Mesh("assets/Gizmo.obj", glm::vec3(0.0f, 0.0f, 1.0f));
  m_gizmoY->Initialize(m_vertPos, m_vertCol);
  m_gizmoZ = new Mesh("assets/Gizmo.obj", glm::vec3(1.0f, 0.0f, 0.0f));
  m_gizmoZ->Initialize(m_vertPos, m_vertCol);

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}
// (Part 1): See aforementioned in header comment
void Graphics::Update(float dt, glm::vec3 speed)
{
    static glm::vec3 gizmoPos = glm::vec3(5.0f, 0.0f, 0.0f);
    static float angle = 0.0f;
    float dist = 4.0f;
    std::stack<glm::mat4> tstack;
    std::stack<glm::mat4> rstack;

    m_sun->rotation = glm::rotate(m_sun->rotation, 1 * dt, glm::vec3(0.0f, 1.0f, 0.0f)); //rotate around own axis
    tstack.push(m_sun->translation);
    rstack.push(m_sun->rotation);

    m_planet->scale = glm::scale(glm::mat4(1.0f), glm::vec3(.5,.5,.5));
    m_planet->translation = glm::translate(tstack.top(), glm::vec3(cos(angle) * dist, sin(0.0f) * dist, sin(angle) * dist));
    m_planet->rotation = glm::rotate(rstack.top(), 1 * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    tstack.push(m_planet->translation);
    rstack.push(m_planet->rotation);

    dist = 1.5f; 
    m_starship->scale = glm::scale(glm::mat4(1.0f), glm::vec3(.25, .25, .25));
    m_starship->translation = glm::translate(tstack.top(), glm::vec3(cos(angle) * dist, sin(0.0f) * dist, sin(angle)) * dist);
    m_starship->rotation = glm::rotate(glm::mat4(1.0f), -1.0f * angle, glm::vec3(.0f, 1.0f, 0.0f)); //turn with the planet
    m_starship->rotation = glm::rotate(m_starship->rotation, -1.5f, glm::vec3(.0f, 1.0f, 0.0f)); // rotate model forward so ship faces forward
    m_starship->rotation = glm::rotate(m_starship->rotation, .5f, glm::vec3(1.0f, 0.0f, .0f)); //tilt 30 degree towards planet

    m_gizmoX->translation = glm::translate(glm::mat4(1.0f), gizmoPos);
    m_gizmoX->scale = glm::scale(glm::mat4(1.0f), glm::vec3(.005, .005, .005));
    m_gizmoX->rotation = glm::rotate(glm::mat4(1.0f), -1.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    m_gizmoY->translation = glm::translate(glm::mat4(1.0f), gizmoPos);
    m_gizmoY->scale = glm::scale(glm::mat4(1.0f), glm::vec3(.005, .005, .005));
    m_gizmoY->rotation = glm::mat4(1.0f);

    m_gizmoZ->translation = glm::translate(glm::mat4(1.0f), gizmoPos);
    m_gizmoZ->scale = glm::scale(glm::mat4(1.0f), glm::vec3(.005, .005, .005));
    m_gizmoZ->rotation = glm::rotate(glm::mat4(1.0f), 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));

    m_sun->Update();
    m_planet->Update();
    m_starship->Update();

    m_gizmoX->Update();
    m_gizmoY->Update();
    m_gizmoZ->Update();

    angle += (1.0f * dt);
    if (angle >= 2 * 3.14159) angle = 0.0f;
}

// (Part 1): See aforementioned in header comment
void Graphics::Render()
{
  //clear the screen
  glClearColor(0.01, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  std::stack<Object*> renderStack;
  renderStack.push(m_sun);
  renderStack.push(m_planet);
  renderStack.push(m_starship);

  renderStack.push(m_gizmoX);
  renderStack.push(m_gizmoY);
  renderStack.push(m_gizmoZ);

  while (!renderStack.empty())
  {
      if (renderStack.top() != NULL)
      {
          glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(renderStack.top()->GetModel()));
          renderStack.top()->Render(m_vertPos, m_vertCol);
      }
      renderStack.pop();
  }
  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
    std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
    glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat)
{
    tmat = glm::translate(glm::mat4(1.0f), glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
    rmat = glm::rotate(rmat, rotSpeed[0] * (float)dt, rotVector);
    smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

// (Part 1): See aforementioned in header comment
Object* Graphics::getInteractWith() {
    return nullptr;
}
// (Part 1): See aforementioned in header comment
std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

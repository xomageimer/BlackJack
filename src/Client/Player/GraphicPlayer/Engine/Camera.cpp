#include "Camera.h"

void Camera::SetPos(const glm::vec2 &NewCameraPos) {
    CameraPos = glm::vec3(NewCameraPos, CameraPos.z);
}

void Camera::Render() {
    glm::mat4 view = glm::lookAt(CameraPos, CameraPos + CameraFront, {0.0, 1.0, 0.0 });
    m_shader->glUniform("view", view);
    glm::mat4 projection;
    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    m_shader->glUniform("projection", projection);
}

Camera::Camera(std::shared_ptr<Shader> shrd) {
    this->m_shader = shrd;
}

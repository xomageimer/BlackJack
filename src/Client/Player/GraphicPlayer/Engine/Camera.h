#ifndef BLACKJACK_CAMERA_H
#define BLACKJACK_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic/Shader.h"

struct Camera {
public:
    Camera(const Camera &) = delete;
    Camera& operator=(const Camera &) = delete;
    Camera(Camera &&) = delete;
    Camera& operator=(Camera &) = delete;

    explicit Camera(std::shared_ptr<Shader> shrd);

    void SetPos (const glm::vec2 & NewCameraPos);

    void Render();

private:
    glm::vec3 CameraPos = {0.f, 0.f, 3.f};
    glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);;

    std::shared_ptr<Shader> m_shader;
};


#endif //BLACKJACK_CAMERA_H

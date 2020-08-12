#ifndef __pandasdl_camera2d_h__
#define __pandasdl_camera2d_h__

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../math/vector2.h"
#include "../math/rectangle.h"

namespace PandaSDL
{
    class Camera2D
    {
        public:
            Camera2D();
            ~Camera2D();

            float Rotation;
            float Zoom;

            void Setup(Rectangle view, Rectangle boundingBox = Rectangle::Empty());

            glm::mat4 GetViewMatrix() const;
            Rectangle GetBoundingBox() const;
            Rectangle GetViewRect() const;
            Vector2 GetPosition() const;
            void SetPosition(Vector2 position);
            void CenterPosition(Vector2 position);
            void OffsetPosition(int x, int y);
            void OffsetPosition(double x, double y);
            void OffsetPosition(float x, float y);
            void OffsetPosition(Vector2 position);
            
        protected:
            Rectangle _viewRect, _boundingBox;
            Vector2 _origin;
            Vector2 _position;

            void CheckBoundingBox();
            void UpdateViewFromPosition();
    };
}

#endif

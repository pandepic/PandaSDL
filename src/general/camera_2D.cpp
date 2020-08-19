#include "camera_2D.h"

PandaSDL::Camera2D::Camera2D()
: Rotation(0.0f), Zoom(1.0f)
{
}

PandaSDL::Camera2D::~Camera2D()
{
}

void PandaSDL::Camera2D::Setup(Rectangle view, Rectangle boundingBox)
{
    _viewRect = view;
    _boundingBox = boundingBox;
    _origin = PandaSDL::Vector2(view.Width / 2, view.Height / 2);
    _position = PandaSDL::Vector2(_viewRect.X, _viewRect.Y);
}

glm::mat4 PandaSDL::Camera2D::GetViewMatrix() const
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-_viewRect.X, -_viewRect.Y, 0.0f));
    view = glm::translate(view, glm::vec3(_origin.X, _origin.Y, 0.0f));
    view = glm::rotate(view, glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, glm::vec3(-_origin.X, -_origin.Y, 0.0f));
    view = glm::scale(view, glm::vec3(Zoom, Zoom, 1.0f));

    return view;
}

PandaSDL::Rectangle PandaSDL::Camera2D::GetBoundingBox() const
{
    return _boundingBox;
}

void PandaSDL::Camera2D::SetBoundingBox(Rectangle rect)
{
    _boundingBox = rect;
    CheckBoundingBox();
}

PandaSDL::Rectangle PandaSDL::Camera2D::GetViewRect() const
{
    return _viewRect;
}

PandaSDL::Vector2 PandaSDL::Camera2D::GetPosition() const
{
    return _position;
}

void PandaSDL::Camera2D::SetPosition(Vector2 position)
{
    _position = position;
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::CenterPosition(Vector2 position)
{
    // handle camera zoom
    auto scaledViewRect = _viewRect * Zoom;
    auto scaledPosition = position * Zoom;
    
    _position.X = scaledPosition.X - (scaledViewRect.Width / 2);
    _position.Y = scaledPosition.Y - (scaledViewRect.Height / 2);
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::CenterPosition(Rectangle rect)
{
    // handle camera zoom
    auto scaledViewRect = _viewRect * Zoom;
    auto scaledRect = rect * Zoom;
    
    _position.X = (scaledRect.X + (scaledRect.Width / 2)) - (scaledViewRect.Width / 2);
    _position.Y = (scaledRect.Y + (scaledRect.Height / 2)) - (scaledViewRect.Height / 2);
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::OffsetPosition(int x, int y)
{
    _position.X += x;
    _position.Y += y;
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::OffsetPosition(double x, double y)
{
    _position.X += x;
    _position.Y += y;
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::OffsetPosition(float x, float y)
{
    _position.X += x;
    _position.Y += y;
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::OffsetPosition(Vector2 position)
{
    _position += position;
    
    UpdateViewFromPosition();
    CheckBoundingBox();
}

void PandaSDL::Camera2D::CheckBoundingBox()
{
    if (_boundingBox.IsEmpty())
        return;

    if (_viewRect.X < _boundingBox.X)
    {
        _viewRect.X = _boundingBox.X;
        _position.X = _viewRect.X;
    }
    if (_viewRect.X + _viewRect.Width > _boundingBox.X + _boundingBox.Width)
    {
        _viewRect.X = (_boundingBox.X + _boundingBox.Width) - _viewRect.Width;
        _position.X = _viewRect.X;
    }
    if (_viewRect.Y < _boundingBox.Y)
    {
        _viewRect.Y = _boundingBox.Y;
        _position.Y = _viewRect.Y;
    }
    if ((_viewRect.Y + _viewRect.Height) > (_boundingBox.Y + _boundingBox.Height))
    {
        _viewRect.Y = (_boundingBox.Y + _boundingBox.Height) - _viewRect.Height;
        _position.Y = _viewRect.Y;
    }
}

void PandaSDL::Camera2D::UpdateViewFromPosition()
{
    _viewRect.X = (int)_position.X;
    _viewRect.Y = (int)_position.Y;
}

std::string PandaSDL::Camera2D::ToString()
{
    return _viewRect.ToString() + " - " + std::to_string(Zoom) + " - " + std::to_string(Rotation);
}
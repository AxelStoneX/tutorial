#ifndef OGLDRAW_H
#define OGLDRAW_H

#include <QGLWidget>

// ===========================================================================
class OGLDraw : public QGLWidget
{
protected:
    virtual void initializeGL();
    virtual void resizeGL(int nWidth, int nHeight);
    virtual void paintGL();
public:
    OGLDraw(QWidget* pwgt = 0);
    void draw(int xOffset, int yOffset, GLenum type);
};

#endif // OGLDRAW_H

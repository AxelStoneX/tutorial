#include "OGLPyramid.h"
#include <QtGui>

OGLPyramid::OGLPyramid(QWidget* pwgt)
    : QGLWidget(pwgt)
    , m_xRotate(0)
    , m_yRotate(0)
{
}

/*virtual*/ void OGLPyramid::initializeGL()
{
    qglClearColor(Qt::black);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    m_nPyramid = createPyramid(1.2f);
}

/*virtual*/ void OGLPyramid::resizeGL(int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
}

/*virtual*/ void OGLPyramid::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);

    glRotatef(m_xRotate, 1.0, 0.0, 0.0);
    glRotatef(m_yRotate, 0.0, 1.0, 0.0);

    glCallList(m_nPyramid);
}

/*virtual*/ void OGLPyramid::mousePressEvent(QMouseEvent *pe)
{
    m_ptPosition = pe->pos();
}

/*virtual*/ void OGLPyramid::mouseMoveEvent(QMouseEvent *pe)
{
    m_xRotate += 180 * (GLfloat) (pe->y() - m_ptPosition.y()) / height();
    m_yRotate += 180 * (GLfloat) (pe->x() - m_ptPosition.x()) / width();
    updateGL();

    m_ptPosition = pe->pos();
}

GLuint OGLPyramid::createPyramid(GLfloat fSize)
{
    GLuint n = glGenLists(1);

    glNewList(n, GL_COMPILE);
        glBegin(GL_TRIANGLE_FAN);
            qglColor(Qt::green);
            glVertex3f(0.0, fSize, 0.0);
            glVertex3f(-fSize, -fSize, fSize);
            glVertex3f(fSize, -fSize, fSize);
            qglColor(Qt::yellow);
            glVertex3f(fSize, -fSize, -fSize);
            qglColor(Qt::blue);
            glVertex3f(-fSize, -fSize, -fSize);
            qglColor(Qt::white);
            glVertex3f(-fSize, -fSize, fSize);
        glEnd();

        glBegin(GL_QUADS);
            qglColor(Qt::red);
            glVertex3f(-fSize, -fSize, fSize);
            glVertex3f(fSize, -fSize, fSize);
            glVertex3f(fSize, -fSize, -fSize);
            glVertex3f(-fSize, -fSize, -fSize);
        glEnd();
    glEndList();

    return n;
}

#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "../include/World.h"

void buildRotMatrix(float *x, float *y, float *z, float *m) {
    m[0] = x[0];
    m[1] = x[1];
    m[2] = x[2];
    m[3] = 0;
    m[4] = y[0];
    m[5] = y[1];
    m[6] = y[2];
    m[7] = 0;
    m[8] = z[0];
    m[9] = z[1];
    m[10] = z[2];
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}


void cross(float *a, float *b, float *res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float *a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}


float length(float *v) {

    float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return res;

}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}


void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

    // catmull-rom matrix
    float m[4][4] = {{-0.5f, 1.5f,  -1.5f, 0.5f},
                     {1.0f,  -2.5f, 2.0f,  -0.5f},
                     {-0.5f, 0.0f,  0.5f,  0.0f},
                     {0.0f,  1.0f,  0.0f,  0.0f}};

    for (int i = 0; i < 3; ++i) {
        float a[4];
        float p[4] = {p0[i], p1[i], p2[i], p3[i]};
        multMatrixVector(*m, p, a);

        pos[i] = a[0] * pow(t, 3) + a[1] * pow(t, 2) + a[2] * t + a[3];
        deriv[i] = 3 * a[0] * pow(t, 2) + 2 * a[1] * t + a[2];
    }
}


void getGlobalCatmullRomPoint(double gt, glm::vec3* pos, glm::vec3* deriv, int nControlPoints, const std::vector<glm::vec3>& p) {

    double t = gt * nControlPoints;
    int index = floor(t);
    t = t - index;

    int indices[4];
    indices[0] = (index + nControlPoints - 1) % nControlPoints;
    indices[1] = (indices[0] + 1) % nControlPoints;
    indices[2] = (indices[1] + 1) % nControlPoints;
    indices[3] = (indices[2] + 1) % nControlPoints;

    float posArray[3], derivArray[3];
    float p0[3] = { p[indices[0]].x, p[indices[0]].y, p[indices[0]].z };
    float p1[3] = { p[indices[1]].x, p[indices[1]].y, p[indices[1]].z };
    float p2[3] = { p[indices[2]].x, p[indices[2]].y, p[indices[2]].z };
    float p3[3] = { p[indices[3]].x, p[indices[3]].y, p[indices[3]].z };

    getCatmullRomPoint(t, p0, p1, p2, p3, posArray, derivArray);

    pos->x = posArray[0];
    pos->y = posArray[1];
    pos->z = posArray[2];

    deriv->x = derivArray[0];
    deriv->y = derivArray[1];
    deriv->z = derivArray[2];
}

void renderCatmullRomCurve(int nControlPoints, const std::vector<glm::vec3>& p) {
    glm::vec3 pos, deriv;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        getGlobalCatmullRomPoint(i / 100.0, &pos, &deriv, nControlPoints, p);
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

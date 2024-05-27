#ifndef CATMULL_H
#define CATMULL_H

#include <vector>
#include <glm/glm.hpp>

void buildRotMatrix(float *x, float *y, float *z, float *m);

void cross(float *a, float *b, float *res);

void normalize(float *a);

float length(float *v);

void multMatrixVector(float *m, float *v, float *res);

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);

void getGlobalCatmullRomPoint(double gt, glm::vec3* pos, glm::vec3* deriv, int nControlPoints, const std::vector<glm::vec3>& p);

void renderCatmullRomCurve(int nControlPoints, const std::vector<glm::vec3>& p);

#endif

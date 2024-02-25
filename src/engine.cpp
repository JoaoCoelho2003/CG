#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
#include "../include/World.h"

World world;

float translateX = 0.0f;
float translateZ = 0.0f;
float rotateAngle = 0.0f;
float scaleHeight = 1.0f;

void referencial() {
    glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

}

void keyboard_movement(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            translateZ += 0.2f;
            break;
        case 's':
            translateZ -= 0.2f;
            break;
        case 'a':
            translateX += 0.2f;
            break;
        case 'd':
            translateX -= 0.2f;
            break;
        case 'q':
            rotateAngle += 1.0f;
            break;
        case 'e':
            rotateAngle -= 1.0f;
            break;
        case 'z':
            scaleHeight *= 1.1f;
            break;
        case 'x':
            scaleHeight *= 0.9f;
            break;
    }
    glutPostRedisplay();
}

void parseXML(const char* filename, World& world) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file." << std::endl;
        return;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("world");
    if (!root) {
        std::cerr << "Error: Missing <world> element in XML file." << std::endl;
        return;
    }

    // Parse model files
    tinyxml2::XMLElement* groupElement = root->FirstChildElement("group");
    if (groupElement) {
        tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
        if (modelsElement) {
            for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
                const char* filename;
                if (modelElement->QueryStringAttribute("file", &filename) == tinyxml2::XML_SUCCESS) {
                    Model model;
                    model.filename = filename;
                    world.models.push_back(model);
                }
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glLoadIdentity();

    gluLookAt(world.camera.posX, world.camera.posY, world.camera.posZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)world.window.width / world.window.height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);

    referencial();

    // Render models
    for (const auto& model : world.models) {
        std::ifstream inputFile(model.filename);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening model file: " << model.filename << std::endl;
            continue;
        }

        // Assuming each line in the model file represents a vertex
        glBegin(GL_TRIANGLES);
        float x, y, z;
        while (inputFile >> x >> y >> z) {
            glVertex3f(x, y, z);
        }
        glEnd();

        inputFile.close();
    }

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Engine");

    glEnable(GL_DEPTH_TEST);

    world.camera = {0.0f, 0.0f, 5.0f};
    world.window = {1920, 1080};

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard_movement);

    if (argc != 2) {
        std::cerr << "Usage: engine <config_file.xml>" << std::endl;
        return 1;
    }

    const char* configFile = argv[1];

    parseXML(configFile, world);

    glutMainLoop();

    return 0;
}
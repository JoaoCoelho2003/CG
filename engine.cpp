#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h> // Include FreeGLUT header

struct Model {
    std::string filename;
};

struct World {
    struct Camera {
        float posX, posY, posZ;
        float lookAtX, lookAtY, lookAtZ;
        float upX, upY, upZ;
        float fov;
        float nearClip;
        float farClip;
    } camera;

    struct Window {
        int width;
        int height;
    } window;

    std::vector<Model> models;
};

World world;

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

// FreeGLUT display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glLoadIdentity(); // Reset the modelview matrix

    // Set up camera
    gluLookAt(world.camera.posX, world.camera.posY, world.camera.posZ,
              world.camera.lookAtX, world.camera.lookAtY, world.camera.lookAtZ,
              world.camera.upX, world.camera.upY, world.camera.upZ);

    // Apply projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(world.camera.fov, (float)world.window.width / world.window.height, world.camera.nearClip, world.camera.farClip);
    glMatrixMode(GL_MODELVIEW);

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

// FreeGLUT reshape function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / height, 1.0, 1000.0); // Example FOV, near, and far
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Engine");

    glEnable(GL_DEPTH_TEST);

    // Initialize camera and window parameters
    world.camera = {0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 45.0f, 1.0f, 100.0f};
    world.window = {1920, 1080};

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    if (argc != 2) {
        std::cerr << "Usage: engine <config_file.xml>" << std::endl;
        return 1;
    }

    const char* configFile = argv[1];

    parseXML(configFile, world);

    glutMainLoop();

    return 0;
}
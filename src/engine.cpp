#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include "../include/World.h"

World world;

float translateX = 0.0f;
float translateZ = 0.0f;
float translateY = 0.0f;
float rotateAngle_lr = 0.0f;
float rotateAngle_ud = 0.0f;
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
        case 'z':
            scaleHeight *= 1.1f;
            break;
        case 'x':
            scaleHeight *= 0.9f;
            break;
        case '1':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case '2':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case '3':
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
    }
    glutPostRedisplay();
}

void keyboard_special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_PAGE_UP:
            translateY -= 0.2f;
            break;
        case GLUT_KEY_PAGE_DOWN:
            translateY += 0.2f;
            break;
        case GLUT_KEY_LEFT:
            rotateAngle_lr -= 1.0f;
            break;
        case GLUT_KEY_RIGHT:
            rotateAngle_lr += 1.0f;
            break;
        case GLUT_KEY_UP:
            rotateAngle_ud += 1.0f;
            break;
        case GLUT_KEY_DOWN:
            rotateAngle_ud -= 1.0f;
            break;
    }
    glutPostRedisplay();
}

void parseTransform(const tinyxml2::XMLElement* transformElement, Node& node) {
    for (const tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* type = child->Value();
        Transformation transform;
        if (strcmp(type, "translate") == 0) {
            transform.type = TransformationType::TRANSLATE;
        } else if (strcmp(type, "rotate") == 0) {
            transform.type = TransformationType::ROTATE;
            transform.values.push_back(child->FloatAttribute("angle"));
        } else if (strcmp(type, "scale") == 0) {
            transform.type = TransformationType::SCALE;
        }
        else {
            std::cerr << "Error: Unknown transformation type: " << type << std::endl;
            continue;
        }
        transform.values.push_back(child->FloatAttribute("x"));
        transform.values.push_back(child->FloatAttribute("y"));
        transform.values.push_back(child->FloatAttribute("z"));
        node.transformations.push_back(transform);
    }   
}

void parseModel(const tinyxml2::XMLElement* modelElement, Node& node) {
    for(const tinyxml2::XMLElement* child = modelElement->FirstChildElement(); child; child = child->NextSiblingElement()) {   
        const char* filename = child->Attribute("file");
        if (filename) {
            node.model_name.push_back(filename);
        }
    }
}

void parseGroup(const tinyxml2::XMLElement* groupElement, Tree& tree) {
    for (const tinyxml2::XMLElement* child = groupElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* type = child->Value();
        if (strcmp(type, "transform") == 0) {
            parseTransform(child, tree.node);
        } else if (strcmp(type, "models") == 0) {
            parseModel(child, tree.node);
        } else if (strcmp(type, "group") == 0) {
            Tree subtree;
            parseGroup(child, subtree);
            tree.children.push_back(subtree);
        } else {
            std::cerr << "Error: Unknown element type: " << type << std::endl;
        }
    }
}

void parseXML(const char* filename, World& tree) {
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

    tinyxml2::XMLElement* cameraElement = root->FirstChildElement("camera");
    if (cameraElement) {
        world.camera = {
            cameraElement->FirstChildElement("position")->FloatAttribute("x"),
            cameraElement->FirstChildElement("position")->FloatAttribute("y"),
            cameraElement->FirstChildElement("position")->FloatAttribute("z"),
            cameraElement->FirstChildElement("lookAt")->FloatAttribute("x"),
            cameraElement->FirstChildElement("lookAt")->FloatAttribute("y"),
            cameraElement->FirstChildElement("lookAt")->FloatAttribute("z"),
            cameraElement->FirstChildElement("up")->FloatAttribute("x"),
            cameraElement->FirstChildElement("up")->FloatAttribute("y"),
            cameraElement->FirstChildElement("up")->FloatAttribute("z"),
            cameraElement->FirstChildElement("projection")->FloatAttribute("fov"),
            cameraElement->FirstChildElement("projection")->FloatAttribute("near"),
            cameraElement->FirstChildElement("projection")->FloatAttribute("far")
        };
    } else {
        std::cerr << "Error: Missing <camera> element in XML file." << std::endl;
    }

    tinyxml2::XMLElement* windowElement = root->FirstChildElement("window");
    if (windowElement) {
        world.window = {
            windowElement->IntAttribute("width"),
            windowElement->IntAttribute("height")
        };
    } else {
        std::cerr << "Error: Missing <window> element in XML file." << std::endl;
    }

    tinyxml2::XMLElement* groupElement = root->FirstChildElement("group");
    if(groupElement) {
        parseGroup(groupElement, world.tree);
    }
}

void render_models(Tree tree, std::vector<Transformation> transformations = {}) {
    // push new transformations
    for (auto& transform : tree.node.transformations) {
        transformations.push_back(transform);
    }

    glPushMatrix();
    for (const auto& transformation : transformations) {
        switch (transformation.type) {
            case TransformationType::TRANSLATE:
                glTranslatef(transformation.values[0], transformation.values[1], transformation.values[2]);
                break;

            case TransformationType::ROTATE:
                glRotatef(transformation.values[0], transformation.values[1], transformation.values[2], transformation.values[3]);
                break;

            case TransformationType::SCALE:
                glScalef(transformation.values[0], transformation.values[1], transformation.values[2]);
                break;
        }
    }
    glPopMatrix();
    int model_index = 0;
    for (const auto& model_name : tree.node.model_name) {
        std::ifstream inputFile(model_name);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening model file: " << model_name << std::endl;
            continue;
        }
        // Assuming each line in the model file represents a vertex with position (x, y, z)
        glBegin(GL_TRIANGLES);
        float x, y, z;
        int vertices_number = 0;
        std::vector<Vertex> vertices;
        while (inputFile >> x >> y >> z) {
            glVertex3f(x, y, z);
            vertices.push_back({x, y, z});
            vertices_number++;
            if (vertices_number == 3) {
                world.models[model_index].vertices.push_back(vertices);
                vertices.clear();
                vertices_number = 0;
            }
        }
        glEnd();

        inputFile.close();
        model_index++;
    }
    // render child models
    for (auto& child : tree.children) {
        render_models(child, transformations);
    }
}

void render_loaded_models(std::vector<Model>& models) {
    for (const auto& model : models) {
        for (const auto& vertices : model.vertices) {
            glBegin(GL_TRIANGLES);
            for (const auto& vertex : vertices) {
                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
            glEnd();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLoadIdentity();

    gluLookAt(world.camera.posX, world.camera.posY, world.camera.posZ,
              world.camera.lookAtX, world.camera.lookAtY, world.camera.lookAtZ,
              world.camera.upX, world.camera.upY, world.camera.upZ);

    // Apply global transformations
    glRotatef(rotateAngle_lr, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateAngle_ud, 0.0f, 0.0f, 1.0f);
    glTranslatef(translateX, translateY, translateZ);    
    referencial();
    glMatrixMode(GL_MODELVIEW);
    glColor3f(1.0f, 1.0f, 1.0f);


    if (!world.models.empty()) {
        std::cout << "Rendering loaded models..." << std::endl;
        render_loaded_models(world.models);
    }
    else {
        std::cout << "Rendering models first time..." << std::endl; 
        render_models(world.tree);
    }
    glutSwapBuffers();
}

void resize(int w, int h) {

	if(h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
    glViewport(0, 0, w, h);

    gluPerspective(world.camera.fov, (float)world.window.width / world.window.height, world.camera.near, world.camera.far);

	glMatrixMode(GL_MODELVIEW);
}

void printTree(Tree tree, int level) {
    for (int i = 0; i < level; i++) {
        std::cout << "  ";
    }
    std::cout << "Group" << std::endl;
    for (const auto& transform : tree.node.transformations) {
        for (int i = 0; i < level; i++) {
            std::cout << "  ";
        }
        std::cout << "  Transformation: ";
        switch (transform.type) {
            case TransformationType::TRANSLATE:
                std::cout << "Translate ";
                break;
            case TransformationType::ROTATE:
                std::cout << "Rotate ";
                break;
            case TransformationType::SCALE:
                std::cout << "Scale ";
                break;
        }
        for (const auto& value : transform.values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    for (const auto& model : tree.node.model_name) {
        for (int i = 0; i < level; i++) {
            std::cout << "  ";
        }
        std::cout << "  Model: " << model << std::endl;
    }
    for (const auto& child : tree.children) {
        printTree(child, level + 1);
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Engine");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    world.camera = {0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 60.0f, 1.0f, 1000.0f};
    world.window = {800, 800};

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard_movement);
    glutSpecialFunc(keyboard_special);
    glutReshapeFunc(resize);

    if (argc != 2) {
        std::cerr << "Usage: engine <config_file.xml>" << std::endl;
        return 1;
    }

    const char* configFile = argv[1];

    parseXML(configFile, world);

    printTree(world.tree, 0);

    glutMainLoop();

    return 0;
}
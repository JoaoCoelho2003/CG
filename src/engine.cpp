#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../include/World.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/CatmullRom.h"
#include <IL/il.h>

World world;

float translateX = 0.0f;
float translateZ = 0.0f;
float translateY = 0.0f;
float rotateAngle_lr = 0.0f;
float rotateAngle_ud = 0.0f;
float scaleHeight = 1.0f;
float movementSpeed = 0.1f;
bool cursorVisible = true;
bool showReferenceAxes = true;
bool showCatmullRom = true;

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


void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        glutSetCursor(GLUT_CURSOR_NONE);
        cursorVisible = false;
    }
}

void keyboard_movement(unsigned char key, int x, int y) {
    if (key == 27) {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        cursorVisible = true;
    }

    float forwardX = world.camera.lookAtX - world.camera.posX;
    float forwardY = world.camera.lookAtY - world.camera.posY;
    float forwardZ = world.camera.lookAtZ - world.camera.posZ;

    float length = sqrt(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    if (length != 0.0f) {
        forwardX /= length;
        forwardY /= length;
        forwardZ /= length;
    }

    float rightX = world.camera.upY * forwardZ - world.camera.upZ * forwardY;
    float rightY = world.camera.upZ * forwardX - world.camera.upX * forwardZ;
    float rightZ = world.camera.upX * forwardY - world.camera.upY * forwardX;

    float rightLength = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
    if (rightLength != 0.0f) {
        rightX /= rightLength;
        rightY /= rightLength;
        rightZ /= rightLength;
    }

    switch (key) {
        case 'w': {
            translateX -= movementSpeed * forwardX;
            translateY -= movementSpeed * forwardY;
            translateZ -= movementSpeed * forwardZ;
            break;
        }
        case 's': {
            translateX += movementSpeed * forwardX;
            translateY += movementSpeed * forwardY;
            translateZ += movementSpeed * forwardZ;
            break;
        }
        case 'a': {
            translateX -= movementSpeed * rightX;
            translateY -= movementSpeed * rightY;
            translateZ -= movementSpeed * rightZ;
            break;
        }
        case 'd': {
            translateX += movementSpeed * rightX;
            translateY += movementSpeed * rightY;
            translateZ += movementSpeed * rightZ;
            break;
        }
        case '1':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case '2':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case '3':
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case '+':
            movementSpeed += 0.1f;
            break;
        case '-':
            movementSpeed -= 0.1f;
            break;
    }

    glutPostRedisplay();
}



void mouse_movement(int x, int y) {
    static int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    static int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    if (!cursorVisible) {
        int deltaX = x - centerX;
        int deltaY = y - centerY;

        if (deltaX != 0 || deltaY != 0) {
            float sensitivity = 0.01f;

            float lookAtDeltaX = sensitivity * deltaX;
            float lookAtDeltaY = -sensitivity * deltaY;

            world.camera.lookAtX += lookAtDeltaX;
            world.camera.lookAtY += lookAtDeltaY;

            if (world.camera.lookAtY > 89.0f) {
                world.camera.lookAtY = 89.0f;
            } else if (world.camera.lookAtY < -89.0f) {
                world.camera.lookAtY = -89.0f;
            }

            glutWarpPointer(centerX, centerY);
        }
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
        case GLUT_KEY_F1:
            showReferenceAxes = !showReferenceAxes;
            break;
        case GLUT_KEY_F2:
            showCatmullRom = !showCatmullRom;
            break;
    }
    glutPostRedisplay();
}

void parseTransform(const tinyxml2::XMLElement* transformElement, Node& node) {
    for (const tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* type = child->Value();
        Transformation transform;
        if (strcmp(type, "translate") == 0) {
            if (child->Attribute("time")) {
                transform.type = TransformationType::CATMULL_ROM_TRANSLATE;
                transform.values.push_back(child->FloatAttribute("time"));
                transform.align = child->BoolAttribute("align");
            } else {
                transform.type = TransformationType::TRANSLATE;
            }
        } else if (strcmp(type, "rotate") == 0) {
            if (child->Attribute("angle")) {
                transform.type = TransformationType::ROTATE;
                transform.values.push_back(child->FloatAttribute("angle"));
            } else {
                transform.type = TransformationType::TIME_DEPENDENT_ROTATE;
                transform.values.push_back(child->FloatAttribute("time"));
            }
        } else if (strcmp(type, "scale") == 0) {
            transform.type = TransformationType::SCALE;
        } else {
            std::cerr << "Error: Unknown transformation type: " << type << std::endl;
            continue;
        }
        if(transform.type != TransformationType::CATMULL_ROM_TRANSLATE && transform.type != TransformationType::TIME_DEPENDENT_ROTATE){
            transform.values.push_back(child->FloatAttribute("x"));
            transform.values.push_back(child->FloatAttribute("y"));
            transform.values.push_back(child->FloatAttribute("z"));
        }
        else if(transform.type == TransformationType::CATMULL_ROM_TRANSLATE){
            for(const tinyxml2::XMLElement* point = child->FirstChildElement(); point; point = point->NextSiblingElement()){
                glm::vec3 controlPoint;
                controlPoint.x = point->FloatAttribute("x");
                controlPoint.y = point->FloatAttribute("y");
                controlPoint.z = point->FloatAttribute("z");
                transform.control_points.push_back(controlPoint);
            }
        }
        else{
            glm::vec3 controlPoint;
            controlPoint.x = child->FloatAttribute("x");
            controlPoint.y = child->FloatAttribute("y");
            controlPoint.z = child->FloatAttribute("z");
            transform.control_points.push_back(controlPoint);
        }
        node.transformations.push_back(transform);
    }   
}

void parseColor(const tinyxml2::XMLElement* colorElement, Material& material) {
    const tinyxml2::XMLElement* diffuse = colorElement->FirstChildElement("diffuse");
    if (diffuse) {
        material.diffuse[0] = diffuse->FloatAttribute("R") / 255.0f;
        material.diffuse[1] = diffuse->FloatAttribute("G") / 255.0f;
        material.diffuse[2] = diffuse->FloatAttribute("B") / 255.0f;
    }

    const tinyxml2::XMLElement* ambient = colorElement->FirstChildElement("ambient");
    if (ambient) {
        material.ambient[0] = ambient->FloatAttribute("R") / 255.0f;
        material.ambient[1] = ambient->FloatAttribute("G") / 255.0f;
        material.ambient[2] = ambient->FloatAttribute("B") / 255.0f;
    }

    const tinyxml2::XMLElement* specular = colorElement->FirstChildElement("specular");
    if (specular) {
        material.specular[0] = specular->FloatAttribute("R") / 255.0f;
        material.specular[1] = specular->FloatAttribute("G") / 255.0f;
        material.specular[2] = specular->FloatAttribute("B") / 255.0f;
    }

    const tinyxml2::XMLElement* emissive = colorElement->FirstChildElement("emissive");
    if (emissive) {
        material.emissive[0] = emissive->FloatAttribute("R") / 255.0f;
        material.emissive[1] = emissive->FloatAttribute("G") / 255.0f;
        material.emissive[2] = emissive->FloatAttribute("B") / 255.0f;
    }

    const tinyxml2::XMLElement* shininess = colorElement->FirstChildElement("shininess");
    if (shininess) {
        material.shininess = shininess->FloatAttribute("value");
    }
}

void parseTexture(const tinyxml2::XMLElement* textureElement, Model& model) {
    const char* file = textureElement->Attribute("file");
    if (file) {
        std::string filepath = std::string("./textures/") + file;
        ILuint imageID;
        ilGenImages(1, &imageID);
        ilBindImage(imageID);
        if (ilLoadImage(filepath.c_str())) {
            ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
            glGenTextures(1, &model.textureID);
            glBindTexture(GL_TEXTURE_2D, model.textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        ilDeleteImages(1, &imageID);
    }
}

void parseModel(const tinyxml2::XMLElement* modelElement, Node& node) {
    for (const tinyxml2::XMLElement* child = modelElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* filename = child->Attribute("file");
        if (filename) {
            std::string fullpath = std::string("./models/") + filename;
            node.model_name.push_back(fullpath);
            Model model;
            if (child->FirstChildElement("texture")) {
                parseTexture(child->FirstChildElement("texture"), model);
            }

            if (child->FirstChildElement("color")) {
                parseColor(child->FirstChildElement("color"), model.material);
            }

            world.models[fullpath] = model;
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

void parseLight(const tinyxml2::XMLElement* lightElement, Light& light) {
    const char* type = lightElement->Attribute("type");
    if (strcmp(type, "point") == 0) {
        light.position[3] = 1.0f;
    } else if (strcmp(type, "directional") == 0) {
        light.position[3] = 0.0f;
    }

    light.position[0] = lightElement->FloatAttribute("posX");
    light.position[1] = lightElement->FloatAttribute("posY");
    light.position[2] = lightElement->FloatAttribute("posZ");

    const tinyxml2::XMLElement* ambient = lightElement->FirstChildElement("ambient");
    if (ambient) {
        light.ambient[0] = ambient->FloatAttribute("R") / 255.0f;
        light.ambient[1] = ambient->FloatAttribute("G") / 255.0f;
        light.ambient[2] = ambient->FloatAttribute("B") / 255.0f;
        light.ambient[3] = 1.0f;
    }

    const tinyxml2::XMLElement* diffuse = lightElement->FirstChildElement("diffuse");
    if (diffuse) {
        light.diffuse[0] = diffuse->FloatAttribute("R") / 255.0f;
        light.diffuse[1] = diffuse->FloatAttribute("G") / 255.0f;
        light.diffuse[2] = diffuse->FloatAttribute("B") / 255.0f;
        light.diffuse[3] = 1.0f;
    }

    const tinyxml2::XMLElement* specular = lightElement->FirstChildElement("specular");
    if (specular) {
        light.specular[0] = specular->FloatAttribute("R") / 255.0f;
        light.specular[1] = specular->FloatAttribute("G") / 255.0f;
        light.specular[2] = specular->FloatAttribute("B") / 255.0f;
        light.specular[3] = 1.0f;
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

    tinyxml2::XMLElement* lights = root->FirstChildElement("lights");
    if (lights) {
        for (tinyxml2::XMLElement* light = lights->FirstChildElement("light"); light; light = light->NextSiblingElement("light")) {
            Light newLight;
            parseLight(light, newLight);
            world.lights.push_back(newLight);
        }
    }

    tinyxml2::XMLElement* groupElement = root->FirstChildElement("group");
    if(groupElement) {
        parseGroup(groupElement, world.tree);
    }
}

void render_models(Tree tree,std::vector<Transformation> transformations = {}) {
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

            case TransformationType::TIME_DEPENDENT_ROTATE: {
                float rotateTime = transformation.values[0];
                float angle = (((float)glutGet(GLUT_ELAPSED_TIME) / 1000) * 360) / (float)(rotateTime);

                glm::vec3 rotatePoint;
                if (!transformation.control_points.empty()) {
                    rotatePoint = transformation.control_points[0];
                } else {
                    rotatePoint = glm::vec3(0.0f, 0.0f, 0.0f);
                }

                glRotatef(angle, rotatePoint.x, rotatePoint.y, rotatePoint.z);
                break;
            }
            
            case TransformationType::CATMULL_ROM_TRANSLATE: {
                int nControlPoints = transformation.control_points.size();
                if (showCatmullRom) {
                    renderCatmullRomCurve(nControlPoints, transformation.control_points);
                }

                glm::vec3 pos, deriv;
                float totalTime = transformation.values[0];
                float elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
                float timeParameter = fmod(elapsedTime, totalTime) / totalTime;
                getGlobalCatmullRomPoint(timeParameter, &pos, &deriv, nControlPoints, transformation.control_points);

                glTranslatef(pos.x, pos.y, pos.z);

                if (transformation.align) {
                    float x[3] = {deriv.x, deriv.y, deriv.z};
                    normalize(x);
                    float y0[3] = {0, 1, 0};
                    float z[3];
                    cross(x, y0, z);
                    normalize(z);
                    float y[3];
                    cross(z, x, y);
                    normalize(y);

                    float matrix[16];
                    buildRotMatrix(x, y, z, matrix);

                    glMultMatrixf(matrix);
                }
                break;
            }
        }
    }

    for (const auto& model_name : tree.node.model_name) {
        Model& model = world.models[model_name];
        if (model.vbo == 0) {
            std::cout << "Loading model: " << model_name << std::endl;
            std::ifstream inputFile(model_name);
            if (!inputFile.is_open()) {
                std::cerr << "Error opening model file: " << model_name << std::endl;
                continue;
            }

            float x, y, z;
            Triangle triangle;
            std::vector<Vertex> vertices;
            while (inputFile >> x >> y >> z) {
                vertices.push_back({x, y, z});
            }
            inputFile.close();

            glGenBuffers(1, &model.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            model.triangles.resize(vertices.size() / 3);
        }

        glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), nullptr);
        glDrawArrays(GL_TRIANGLES, 0, model.triangles.size() * 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();

    for (auto& child : tree.children) {
        render_models(child,transformations);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLoadIdentity();

    gluLookAt(world.camera.posX, world.camera.posY, world.camera.posZ,
              world.camera.lookAtX, world.camera.lookAtY, world.camera.lookAtZ,
              world.camera.upX, world.camera.upY, world.camera.upZ);

    // Setup lighting
    for (size_t i = 0; i < world.lights.size(); ++i) {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, world.lights[i].position);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, world.lights[i].ambient);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, world.lights[i].diffuse);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, world.lights[i].specular);
    }

    // Apply global transformations
    glRotatef(rotateAngle_lr, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateAngle_ud, 0.0f, 0.0f, 1.0f);
    glTranslatef(translateX, translateY, translateZ);    

    if(showReferenceAxes){
        referencial();
    }
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw models with materials
    render_models(world.tree);

    // Draw light sources
    for (size_t i = 0; i < world.lights.size(); ++i) {
        glPushMatrix();
        glTranslatef(world.lights[i].position[0], world.lights[i].position[1], world.lights[i].position[2]);
        glColor3f(world.lights[i].diffuse[0], world.lights[i].diffuse[1], world.lights[i].diffuse[2]);
        glPopMatrix();
    }

    glutSwapBuffers();

    glutPostRedisplay();
}

void resize(int w, int h) {
    if (h == 0) {
        h = 1;
    }

    float aspectRatio = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (aspectRatio >= 1) {
        glViewport(0, 0, w, h);
        gluPerspective(world.camera.fov, aspectRatio, world.camera.near, world.camera.far);
    } else {
        glViewport(0, 0, w, w / aspectRatio);
        gluPerspective(world.camera.fov, aspectRatio, world.camera.near, world.camera.far);
    }

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

    glewInit();
    ilInit();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    world.camera = {0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 60.0f, 1.0f, 1000.0f};
    world.window = {800, 800};

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard_movement);
    glutPassiveMotionFunc(mouse_movement);
    glutMouseFunc(mouse_click);
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
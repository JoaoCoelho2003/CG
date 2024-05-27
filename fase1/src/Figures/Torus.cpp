#include "../../include/Torus.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

Torus::Torus(float r,float tr, int s, int sl): radius(r), tube_radius(tr), slices(s), stacks(sl) {}

void Torus::generateVertices() {
    vertices.clear();
    // Generate vertices to form a Torus
    float phi_step = 2 * M_PI / float(slices);
	float theta_step = 2 * M_PI / float(stacks);

 	float curr_phi = 0.0f;
		for (int i = 0; i <= slices; i++)
		{
            float phi_next = (i + 1) * phi_step;

			float curr_theta = 0.0f;
			for (int j = 0; j <= stacks; j++)
			{
				float theta_next = (j + 1) * theta_step;

                // Generate vertices
                glm::vec3 v1 = glm::vec3(
                    (radius + tube_radius * cos(curr_theta)) * cos(curr_phi),
                    tube_radius * sin(curr_theta),
                    (radius + tube_radius * cos(curr_theta)) * sin(curr_phi)
                );

                glm::vec3 v2 = glm::vec3(
                    (radius + tube_radius * cos(theta_next)) * cos(curr_phi),
                    tube_radius * sin(theta_next),
                    (radius + tube_radius * cos(theta_next)) * sin(curr_phi)
                );

                glm::vec3 v3 = glm::vec3(
                    (radius + tube_radius * cos(theta_next)) * cos(phi_next),
                    tube_radius * sin(theta_next),
                    (radius + tube_radius * cos(theta_next)) * sin(phi_next)
                );

                glm::vec3 v4 = glm::vec3(
                    (radius + tube_radius * cos(curr_theta)) * cos(phi_next),
                    tube_radius * sin(curr_theta),
                    (radius + tube_radius * cos(curr_theta)) * sin(phi_next)
                );

                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v4);

                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v4);
				// Update current tube angle
				curr_theta = theta_next;
			}

			// Update main segment angle
			curr_phi = phi_next;
    }
}

void Torus::writeToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        outputFile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    outputFile.close();
}
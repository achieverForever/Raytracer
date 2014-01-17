#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtx/rotate_vector.hpp>

#include "ReadFile.h"
#include "Util.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Variables.h"
#include "Scene.h"

using namespace std;

extern Scene scene;

bool ReadValues(stringstream & s, const int num, float * values)
{
	for(int i = 0; i < num; i++)
	{
		s >> values[i];
		if(s.fail())
		{
			cerr << "Failed Reading Values " << i << " Will Skip.\n";
			return false;
		}
	}
	return true;
}

void ReadFile(const char * fileName)
{
	vec3 ambient, diffuse, specular, emission;	// Variables to track current material properties
	float shininess;
	vec3 atten;
	Material matrl;
	bool isNewMaterial, isNewTransf;
	Material *pCurrMaterial = NULL;
	mat4 *pCurrMatrix = NULL;
	mat4 *pInvCurrMatrix = NULL;

	ambient = diffuse = specular = emission = Color(0.0f);
	shininess = 0.0f;
	atten = vec3(1.0f, 0.0f, 0.0f);
	isNewMaterial = false;
	isNewTransf = false;

	string str, cmd;
	ifstream in;
	in.open(fileName);
	if(in.is_open())
	{
		// Use a matrix stack to store transforms
		matrixStack.push(mat4(1.0f));	// Initialize the matrix stack with identity
		currMatrix = matrixStack.top();
		invCurrMatrix = glm::inverse(currMatrix);

		bool isValid = false;
		float values[10] = {0.0f};	// Maximum 10 values

		getline (in, str); 
		while (in) 
		{
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) 
			{
				// Ruled out comment and blank lines 

				stringstream s(str);
				s >> cmd; 
				int i; 
				float values[10]; 
				bool isValid; 

				// Lighting Command
				if (cmd == "point")		// Point light
				{
					isValid = ReadValues(s, 6, values); 
					if (isValid) 
						lights.push_back(Light(POINT, values[0], values[1], values[2],	// Position
											   values[3], values[4], values[5],			// Color
											   atten.x, atten.y, atten.z));				// Attenuation

				} else if(cmd == "directional")		// Directional light
				{
					isValid = ReadValues(s, 6, values); 
					if (isValid) 
						lights.push_back(Light(DIRECTIONAL, values[0], values[1], values[2],	// Direction
											   values[3], values[4], values[5]));				// Color
				} else if(cmd == "attenuation")
				{
					isValid = ReadValues(s, 3, values);
					if (isValid)
						atten = vec3(values[0], values[1], values[2]);
				}

				// Vertex data
				else if (cmd == "maxverts") {
					isValid = ReadValues(s, 1, values);
					if (isValid)
					{
						maxVerts = (int) values[0];
						vertices.clear();
						vertices.reserve(maxVerts);
					}
				} else if (cmd == "vertex") {
					isValid = ReadValues(s, 3, values);
					if (isValid)
						vertices.push_back(vec3(values[0], values[1], values[2]));
				}

				// Triangle command
				else if(cmd == "tri") {
					isValid = ReadValues(s, 3, values);
					if(isValid){
						// Store current material (to avoid redundancy)
						if(isNewMaterial)
						{
							isNewMaterial = false;

							matrl = Material(ambient, diffuse, specular, emission, shininess);
							materials.push_back(matrl);
							pCurrMaterial = &(materials.back());	// Update current material pointer
						}

						if (isNewTransf)
						{
							isNewTransf = false;

							currMatrix = matrixStack.top();
							transforms.push_back(currMatrix);
							pCurrMatrix = &(transforms.back());	// Update current matrix pointer

							invCurrMatrix = glm::inverse(currMatrix);
							transforms.push_back(invCurrMatrix);
							pInvCurrMatrix = &(transforms.back());	// Update current inverse matrix pointer
						}

						Triangle tri((int) values[0], (int) values[1], (int) values[2],
									 pCurrMaterial, &vertices, pCurrMatrix, NULL);
						triangles.push_back(tri);
					}
				}

				// Sphere command
				else if (cmd == "sphere") {
					isValid = ReadValues(s, 4, values); 
					if (isValid) {
						// Store current material (to avoid redundancy)
						if(isNewMaterial)
						{
							isNewMaterial = false;

							matrl = Material(ambient, diffuse, specular, emission, shininess);
							materials.push_back(matrl);
							pCurrMaterial = &(materials.back());
						}

						if (isNewTransf)
						{
							isNewTransf = false;

							currMatrix = matrixStack.top();
							transforms.push_back(currMatrix);
							pCurrMatrix = &(transforms.back());	// Update current matrix pointer

							invCurrMatrix = glm::inverse(currMatrix);
							transforms.push_back(invCurrMatrix);	
							pInvCurrMatrix = &(transforms.back());	// Update current inverse matrix pointer
						}

						Sphere sphere(vec3(values[0], values[1], values[2]), values[3],
									  (*pCurrMaterial), pCurrMatrix, pInvCurrMatrix);
						spheres.push_back(sphere);
					}
				}
				
				// Material Commands 
				// Ambient, diffuse, specular, shininess properties for each object.
				else if (cmd == "ambient") {
					isNewMaterial = true;
					isValid = ReadValues(s, 3, values); // colors 
					if (isValid) {
						for (i = 0; i < 3; i++) {
							ambient[i] = values[i]; 
						}
					}
				} else if (cmd == "diffuse") {
					isNewMaterial = true;
					isValid = ReadValues(s, 3, values); 
					if (isValid) {
						for (i = 0; i < 3; i++) {
							diffuse[i] = values[i]; 
						}
					}
				} else if (cmd == "specular") {
					isNewMaterial = true;
					isValid = ReadValues(s, 3, values); 
					if (isValid) {
						for (i = 0; i < 3; i++) {
							specular[i] = values[i]; 
						}
					}
				} else if (cmd == "emission") {
					isNewMaterial = true;
					isValid = ReadValues(s, 3, values); 
					if (isValid) {
						for (i = 0; i < 3; i++) {
							emission[i] = values[i]; 
						}
					}
				} else if (cmd == "shininess") {
					isNewMaterial = true;
					isValid = ReadValues(s, 1, values); 
					if (isValid) {
						shininess = values[0]; 
					}
				} else if (cmd == "size") {
					isValid = ReadValues(s,2,values); 
					if (isValid) { 
						width = (int) values[0]; height = (int) values[1]; 
					} 
				} else if (cmd == "output") {
					s >> outFileName;
				} else if (cmd == "maxdepth") {
					isValid = ReadValues(s, 1, values);
					if(isValid)
						maxDepth = (int) values[0];
				}
				
				// Camera commands
				else if (cmd == "camera") {
					isValid = ReadValues(s,10,values); // 10 values eye cen worldUp fov
					if (isValid) {
						// Set the camera's values
						for(i = 0; i < 3; i++)
							eye[i] = values[i];
						for (i = 3; i < 6; i++)
							center[i - 3] = values[i];
						for(i = 6; i < 9; i++)
							worldUp[i - 6] = values[i];
						fovy = values[9];
					}
				}

				else if (cmd == "translate") {
					isValid = ReadValues(s,3,values); 
					if (isValid) {
						isNewTransf = true;
						// Generate a translation matrix and multiply it with current matrix.
						RightMultiply(glm::translate(values[0], values[1], values[2]), matrixStack);
					}
				}
				else if (cmd == "scale") {
					isValid = ReadValues(s,3,values); 
					if (isValid) {
						isNewTransf = true;
						// Generate a scale matrix and multiply it with current matrix.
						RightMultiply(glm::scale(values[0], values[1], values[2]), matrixStack);
					}
				}
				else if (cmd == "rotate") {
					isValid = ReadValues(s,4,values); 
					if (isValid) {
						isNewTransf = true;
						// Generate a rotation matrix and multiply it with current matrix.
						RightMultiply(glm::rotate(values[3], values[0], values[1], values[2]), matrixStack);
					}
				}

				// Push/Pop matrix
				else if (cmd == "pushTransform") {
					matrixStack.push(matrixStack.top()); 
				} else if (cmd == "popTransform") {
					if (matrixStack.size() <= 1) {
						cerr << "Stack has no elements.  Cannot Pop\n"; 
					} else {
						matrixStack.pop(); 
						isNewTransf= true;
					}
				}

				else {
					cerr << "Unknown Command: " << cmd << " Skipping \n"; 
				}
			}
			getline (in, str); 
		}
	}
	else
	{
		cerr << "Failed to Open File " << fileName << " to Read\n";
	}
}




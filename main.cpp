// A special thanks to our dear friend Izan Beltrán
#include "EasyBMP.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "Object.hpp"
#include "Tetrahedron.hpp"
#include "Sphere.hpp"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;


void castRay(Ray &ray, vector<unique_ptr<Object>> &objects, int depth);
bool trace(Ray &ray, vector<unique_ptr<Object>> &objects);
ColorDbl monteCarlo(Vertex point, Vector normal, vector<unique_ptr<Object>> &objects, int depth, int max_iterations);

int main() {

    //------------- Setup the objects in the scene ---------------//
    
    // Array of vertices that builds up the room
    Vertex vertices[] = {
        Vertex(0, 6, 5, 1),
        Vertex(10, 6, 5, 1),
        Vertex(13, 0, 5, 1),
        Vertex(10, -6, 5, 1),
        Vertex(0, -6, 5, 1),
        Vertex(-3, 0, 5, 1),
        Vertex(0, 6, -5, 1),
        Vertex(10, 6, -5, 1),
        Vertex(13, 0, -5, 1),
        Vertex(10, -6, -5, 1),
        Vertex(0, -6, -5, 1),
        Vertex(-3, 0, -5, 1),
        Vertex(5, 0, -5, 1),
        Vertex(5, 0, 5, 1)
    };
    
    // Array of colors used in the scene
    ColorDbl colors[] = {
        ColorDbl(35, 100, 170),
        ColorDbl(61, 165, 217),
        ColorDbl(115, 191, 184),
        ColorDbl(254, 198, 1),
        ColorDbl(234, 115, 23),
        ColorDbl(255, 255, 255)
    };
    
    // Number of triangle objects in the scene
    const int N_TRIANGLES = 24;
    
    // Data for the creation of the triangles
    short t_data[] = {
        // Vertex 1, vertex 2, vertex 3, color
        0, 6, 7,    0,
        0, 7, 1,    1,
        1, 7, 2,    2,
        7, 8, 2,    3,
        8, 9, 2,    4,
        9, 3, 2,    0,
        4, 3, 9,    1,
        10, 4, 9,   2,
        11, 4, 10,  3,
        5, 4, 11,   4,
        0, 5, 11,   0,
        0, 11, 6,   1,
        6, 12, 7,   5,
        7, 12, 8,   5,
        8, 12, 9,   5,
        9, 12, 10,  5,
        10, 12, 11, 5,
        11, 12, 6,  5,
        0, 1, 13,   5,
        1, 2, 13,   5,
        2, 3, 13,   5,
        3, 4, 13,   5,
        4, 5, 13,   5,
        5, 0, 13,   5
    };
    
    // Create a vector containing pointers to all objects in the scene
    vector<unique_ptr<Object>> objects;

    // Add spheres to the vector
    objects.push_back( make_unique<Sphere>(Sphere(Vertex(5, 4, -1, 1), 1, colors[0], DIFFUSE)) );
    
    // Add tetrahedra to the vector
    objects.push_back( make_unique<Tetrahedron>( Tetrahedron(Vertex(5,-2.5,-0.2,1), Vertex(3,-2,0,1), Vertex(5,0,0,1), Vertex(5,-2,2,1), colors[2], DIFFUSE)) );
    
    // Add all triangles to the vector
    for (int i = 0; i < N_TRIANGLES; i++){
        objects.push_back( make_unique<Triangle>( Triangle(vertices[t_data[4*i]], vertices[t_data[4*i+1]], vertices[t_data[4*i+2]], colors[t_data[4*i+3]], DIFFUSE)) );
    }
    
    
    
    //------------- Render Scene ---------------//

    // Color Channels
    int red[SCREEN_WIDTH][SCREEN_HEIGHT];
    int green[SCREEN_WIDTH][SCREEN_HEIGHT];
    int blue[SCREEN_WIDTH][SCREEN_HEIGHT];
    
    // Temporary ray, will be updated with castRay in render loop
    Ray cameraRay = Ray(Vertex(10, 10, 0, 1), Vertex(10 , 10, -1, 1));
    
    cout << "Rendering scene...\n";
    
    int steps = (SCREEN_WIDTH * 2) / 20;
    
    // Loop through all the pixels of the window
    for (int i = 0; i < (SCREEN_WIDTH); i++)
    {
        if((i+1)%steps == 0) cout << ((i+1)*5) / steps << "%\n";
        
        for (int j = 0; j < (SCREEN_HEIGHT); j++)
        {
            // Set myRay to go from eye to the current pixel
            cameraRay = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + i*0.001, 0.4 - j*0.001, 1));
            
            // Use castRay to find the point where the ray will intersect
            castRay(cameraRay, objects, 0);
            
            // Save ray color
            red[i][j] = (int)cameraRay.color.r;
            green[i][j] = (int)cameraRay.color.g;
            blue[i][j] = (int)cameraRay.color.b;
        }
        
    }

    // Create Image
    
    string imageName = "raytracer.bmp";
    EasyBMP::RGBColor black(0, 0, 0);
    EasyBMP::Image img(SCREEN_WIDTH, SCREEN_HEIGHT, imageName, black);
    
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_HEIGHT; j++) {

            EasyBMP::RGBColor color(red[i][j], green[i][j], blue[i][j]);
            img.SetPixel(i, j, color);
        }
    }
    
    img.Write();
    
    cout << "Image saved as " << imageName << "\n";
}





void castRay(Ray &ray, vector<unique_ptr<Object>> &objects, int depth){
    
    if(depth > 1) return;
    
    if(trace(ray, objects)){
       
        switch ( objects[ray.objectIndex]->material() ) {
                
            case DIFFUSE:
            {
                double radiosity = 1.0;
                double shadow = 1.0;
                
                Vector toLight = Vertex(5, 0, 4.9, 1) - ray.intersection;
                double lightDistance = toLight.magnitude();
                toLight = toLight.normalize();
                
                double cosTheta = ray.objectNormal.dot(toLight);
                if (cosTheta < 0) cosTheta = 0;
                radiosity = 0.1 + 0.9 * cosTheta;
                
                Ray shadowRay = Ray(ray.intersection + toLight * 0.0001, ray.intersection + toLight);
                if( trace(shadowRay, objects) ){
                    Vector intersected = shadowRay.intersection - ray.intersection;
                    if(intersected.magnitude() < lightDistance) shadow = 0.4;
                }
                
                
                ColorDbl direct = objects[ray.objectIndex]->color() * shadow * radiosity;
                ColorDbl indirect = monteCarlo(ray.intersection, ray.objectNormal, objects, depth, 5);
                
                ray.color = direct * 0.7 + indirect * 0.3;
                
                return;
            }
               
                
            case REFLECTIVE:
            {
                Vector incoming = (ray.intersection - ray.start).normalize();
                Vector reflectionDirection = incoming - ray.objectNormal * 2 * (incoming.dot(ray.objectNormal));
                
                Ray reflected = Ray(ray.intersection + reflectionDirection * 0.001, ray.intersection + reflectionDirection);
                castRay(reflected, objects, depth +1);
                
                ray.color = reflected.color;
                
                return;
            }
                
                
            case TRANSPARENT:
            {
                Vector incoming = (ray.intersection - ray.start).normalize();
                
                double etai = 1, etat = 1.52;
                Vector n = ray.objectNormal.normalize();
                
                double cosi = incoming.dot(n);
                if (cosi < 0) {
                    cosi = -cosi;
                }
                else {
                    swap(etai, etat);
                    n = n * -1.0;
                }
                
                double eta = etai / etat;
                double k = 1 - eta * eta * (1 - cosi * cosi);
                
                Vector refractionDirection = incoming * eta  + n * (eta * cosi - sqrtf(k));

                Ray refracted = Ray(ray.intersection + refractionDirection * 0.001, ray.intersection + refractionDirection);
                castRay(refracted, objects, depth +1);
                
                ray.color = ray.color + refracted.color * 0.5;
                
                return;
            }
        }
            
    }
}





bool trace(Ray &ray, vector<unique_ptr<Object>> &objects){
    // If no object is intersected, intersected will stay false
    bool intersected = false;
    
    // Check if ray intersects with any objects. Store the object closest to starting point of ray
    for (int i = 0; i < objects.size(); i++){
        if ( objects[i]->rayIntersection(ray) ) {
            intersected = true;
            ray.objectIndex = i;
        }
    }
    
    return intersected;
}





ColorDbl monteCarlo(Vertex point, Vector normal, vector<unique_ptr<Object>> &objects, int depth, int max_iterations) {
    
    // Setup local Cartesian coordinate system with vectors normal, a & b
    Vector a, b;
    
    if( abs(normal.x) < abs(normal.y) ) {
        a = Vector(normal.z, 0, -normal.x).normalize();
    }
    else {
        a = Vector(0, -normal.z, -normal.y).normalize();
    }
    
    b = normal.cross(a);
    
    
    // Variable storing the resulting color of the monte carlo sampling
    ColorDbl result;
    
    for (int i = 0; i < max_iterations; i++) {
        // Generate random angles in the hemisphere
        double theta = drand48() * 0.5 * M_PI;
        double phi = drand48() * 2 * M_PI;
        
        // Vector pointing in a random direction in the hemisphere
        Vector sphereDirection (
                                sin(theta) * cos(phi),
                                cos(theta),
                                sin(theta) * sin(phi)
                                );
        
        // Sphere direction in world coordinates
        Vector direction(
            sphereDirection.x * b.x + sphereDirection.y * normal.x + sphereDirection.z * a.x,
            sphereDirection.x * b.y + sphereDirection.y * normal.y + sphereDirection.z * a.y,
            sphereDirection.x * b.z + sphereDirection.y * normal.z + sphereDirection.z * a.z
        );
        
        
        // Cast a sample ray and get intersected object's color
        Ray sample = Ray(point + direction * 0.001, point + direction);
        castRay(sample, objects, depth+1);
        result = result + sample.color;
    }
    
    // Divide the sum of colors with number of samples to get the average
    result = result * (1.0 / max_iterations);
    
    return result;
}

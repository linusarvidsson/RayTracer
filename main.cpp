// A special thanks to our dear friend Izan Beltrán
#include "EasyBMP.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <thread>

#include "Object.hpp"
#include "Tetrahedron.hpp"
#include "Sphere.hpp"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int L0 = 6000;


void castRay(Ray &ray, vector<unique_ptr<Object>> &objects, int depth);
bool trace(Ray &ray, vector<unique_ptr<Object>> &objects);
ColorDbl directLightLambertian(Vertex point, Vector normal, vector<unique_ptr<Object>> &objects);
ColorDbl directLightOrenNayar(double sigma, Vertex point, Vector normal, Vector out, vector<unique_ptr<Object>> &objects);
Ray refractionDirection(Vertex point, Vector incoming, Vector normal, double eta);
double reflectionCoefficient(double n1, double n2, double cosi);
Ray randomDirection(Vertex point, Vector normal);
float orenNayarBRDF(Vector in, Vector out, Vector normal, double sigma);

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
    objects.push_back( make_unique<Sphere>(Sphere(Vertex(8, 0, 3, 1), 1, colors[0], OREN_NAYAR)) ); //5, 4, -1
    
    // Add tetrahedra to the vector
    objects.push_back( make_unique<Tetrahedron>( Tetrahedron(Vertex(5,-2.5,-0.2,1), Vertex(3,-2,0,1), Vertex(5,0,0,1), Vertex(5,-2,2,1), colors[2], LAMBERTIAN)) );
    
    // Add all triangles to the vector
    for (int i = 0; i < N_TRIANGLES; i++){
        objects.push_back( make_unique<Triangle>( Triangle(vertices[t_data[4*i]], vertices[t_data[4*i+1]], vertices[t_data[4*i+2]], colors[t_data[4*i+3]], LAMBERTIAN)) );
    }
    
    // Create the light source
    objects.push_back(make_unique<Triangle>( Triangle(Vertex(4.5, 1, 4.99, 1), Vertex(4.5, -1, 4.99, 1), Vertex(6.22, 0, 4.99, 1), ColorDbl(255,255,255), EMISSIVE)));
    
    
    //------------- Render Scene ---------------//

    int max_intensity = 0;
    
    // Color Channels
    static double red[SCREEN_WIDTH][SCREEN_HEIGHT];
    static double green[SCREEN_WIDTH][SCREEN_HEIGHT];
    static double blue[SCREEN_WIDTH][SCREEN_HEIGHT];
    
    // Temporary ray, will be updated with castRay in render loop
    Ray cameraRay = Ray(Vertex(10, 10, 0, 1), Vertex(10 , 10, -1, 1));
    
    cout << "Rendering scene...\n";
    
    int steps = (SCREEN_WIDTH * 2) / 20;
    
    // Loop through all the pixels of the window
    for (int i = 0; i < (SCREEN_WIDTH); i++)
    {
        if((i+1)%steps == 0) cout << ((i+1)*10) / steps << "%\n";
        
        for (int j = 0; j < (SCREEN_HEIGHT); j++)
        {
            red[i][j] = green[i][j] = blue[i][j] = 0;
            
            Ray ray1 = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + (i + (-0.25 + 0.5 * drand48()))*0.001, 0.4 - (j + (-0.25 + 0.5 * drand48()))*0.001, 1));
            
            Ray ray2 = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + (i + 0.5 + (-0.25 + 0.5 * drand48()))*0.001, 0.4 - (j + (-0.25 + 0.5 * drand48()))*0.001, 1));
            
            Ray ray3 = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + (i + 0.5 + (-0.25 + 0.5 * drand48()))*0.001, 0.4 - (j + 0.5 + (-0.25 + 0.5 * drand48()))*0.001, 1));
            
            Ray ray4 = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + (i + (-0.25 + 0.5 * drand48()))*0.001, 0.4 - (j + 0.5 + (-0.25 + 0.5 * drand48()))*0.001, 1));
            
            
            std::thread t1(castRay, std::ref(ray1), std::ref(objects), 0);
            std::thread t2(castRay, std::ref(ray2), std::ref(objects), 0);
            std::thread t3(castRay, std::ref(ray3), std::ref(objects), 0);
            std::thread t4(castRay, std::ref(ray4), std::ref(objects), 0);
            
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            
            red[i][j] += 0.25 * (ray1.color.r + ray2.color.r + ray3.color.r + ray4.color.r);
            green[i][j] += 0.25 * (ray1.color.g + ray2.color.g + ray3.color.g + ray4.color.g);
            blue[i][j] += 0.25 * (ray1.color.b + ray2.color.b + ray3.color.b + ray4.color.b);
            
            /*
            
            // Temp loop for altialiasing, split pixel into 4 later
            for (int k = 0; k < 4; k++) {
                // Set myRay to go from eye to the current pixel
                cameraRay = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + (i + (0.5 * (k % 2)) + (-0.25 + 0.5 * drand48()))*0.001, 0.4 - (j + (0.5 * (k / 2)) + (-0.25 + 0.5 * drand48()))*0.001, 1));
                
                // Use castRay to find the point where the ray will intersect
                castRay(cameraRay, objects, 0);
                
                if (cameraRay.color.r > max_intensity) max_intensity = cameraRay.color.r;
                if (cameraRay.color.g > max_intensity) max_intensity = cameraRay.color.g;
                if (cameraRay.color.b > max_intensity) max_intensity = cameraRay.color.b;
                
                // Save ray color
                red[i][j] += 0.25 * cameraRay.color.r;
                green[i][j] += 0.25 * cameraRay.color.g;
                blue[i][j] += 0.25 * cameraRay.color.b;
            } */
        }
    }
    
    // Divide by max intensity
    /*for (int i = 0; i < (SCREEN_WIDTH); i++)
    {
        for (int j = 0; j < (SCREEN_HEIGHT); j++)
        {
            red[i][j] *= 10;
            green[i][j] *= 10;
            blue[i][j] *= 10;
        }
    }*/

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
    
    if(trace(ray, objects)){
       
        switch ( objects[ray.objectIndex]->material() ) {
                
            case LAMBERTIAN:
            {
                ColorDbl direct = objects[ray.objectIndex]->color() * directLightLambertian(ray.intersection, ray.objectNormal, objects) * (1.0/255.0);
                
                if (drand48() > 0.75) {
                    //Terminate the ray
                    //ray.color = direct * 0.7;
                    ray.color = direct;
                    return;
                } else {
                    Ray sample = randomDirection(ray.intersection, ray.objectNormal);
                    castRay(sample, objects, depth + 1);
                    ColorDbl indirect = sample.color * objects[ray.objectIndex]->color() * (1.0/255.0);
                    
                    //ray.color = direct * 0.7 + indirect * 0.3;
                    ray.color = direct + indirect;
                    return;
                }
            }
                
            case OREN_NAYAR:
            {
                Vector out = ray.start - ray.end;
                
                ColorDbl direct = objects[ray.objectIndex]->color() * directLightOrenNayar(0.7, ray.intersection, ray.objectNormal, out, objects) * (1.0/255.0);
                
                if (drand48() > 0.75) {
                    //Terminate the ray
                    //ray.color = direct * 0.7;
                    ray.color = direct;
                    return;
                } else {
                    Ray sample = randomDirection(ray.intersection, ray.objectNormal);
                    castRay(sample, objects, depth + 1);
                    
                    double BRDF = orenNayarBRDF(sample.end - sample.start, out, ray.objectNormal, 0.7);
                   
                    ColorDbl indirect = sample.color * objects[ray.objectIndex]->color() * BRDF * (1.0/255.0);
                    
                    //ray.color = direct * 0.7 + indirect * 0.3;
                    ray.color = direct + indirect;
                    return;
                }
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
                /*
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
                */
                
                // Calculate refracted ray
                Ray refracted = ray;
                
                double eta = 1.52;
                double R;
                
                double cosi = (incoming * -1).dot(ray.objectNormal);
                
                if (cosi > 0) {
                    refracted = refractionDirection(ray.intersection, incoming, ray.objectNormal, 1.0/eta);
                    R = reflectionCoefficient(1, eta, cosi);
                }
                else {
                    refracted = refractionDirection(ray.intersection, incoming, ray.objectNormal, eta);
                    R = reflectionCoefficient(eta, 1, cosi);
                }
                castRay(refracted, objects, depth +1);
                
                // Calculate reflected ray
                Vector reflectionDirection = incoming - ray.objectNormal * 2 * (incoming.dot(ray.objectNormal));
                
                Ray reflected = Ray(ray.intersection + reflectionDirection * 0.001, ray.intersection + reflectionDirection);
                castRay(reflected, objects, depth +1);
                
                
                ray.color = refracted.color * (1 - R) + reflected.color * R;
                
                return;
            }
                
            case EMISSIVE:
            {
                ray.color = ColorDbl(255,255,255);
                return;
            }
        }
            
    }
}


bool trace(Ray &ray, vector<unique_ptr<Object>> &objects){
    // If no object is intersected, intersected will stay false
    bool intersected = false;
    ray.t = 100000;
    
    // Check if ray intersects with any objects. Store the object closest to starting point of ray
    for (int i = 0; i < objects.size(); i++){
        if ( objects[i]->rayIntersection(ray) ) {
            intersected = true;
            ray.objectIndex = i;
        }
    }
    
    return intersected;
}



ColorDbl directLightLambertian(Vertex point, Vector normal, vector<unique_ptr<Object>> &objects) {

    const int M = 5;
    
    ColorDbl result = ColorDbl(0,0,0);
    
    for (int i = 0; i < M; i++) {
        double u, v;
        do {
            u = drand48();
            v = drand48();
        } while (u + v <= 1);
        
        Vertex lightPoint = objects.back()->point(u, v);
        
        
        Vector toLight = lightPoint - point;
        double lightDistance = toLight.magnitude();
        toLight = toLight.normalize();
        
        double visibility = 1;
        
        Ray shadowRay = Ray(point + toLight * 0.01, point + toLight);
        if( trace(shadowRay, objects) ){
            Vector intersected = shadowRay.intersection - point;
            if(intersected.magnitude() < lightDistance) visibility = 0;
        }

        double geometric = normal.dot(toLight) * objects.back()->getNormal().dot(toLight);
        if (geometric < 0) geometric = 0;
        
        double d2 = lightDistance * lightDistance;
        
        result = result + ColorDbl(L0,L0,L0) * (geometric * visibility / d2);
    }
    
    return result * (1.0 / (double)M);
}

ColorDbl directLightOrenNayar(double sigma, Vertex point, Vector normal, Vector out, vector<unique_ptr<Object>> &objects) {
    
    const int M = 5;
    
    ColorDbl result = ColorDbl(0,0,0);
    
    for (int i = 0; i < M; i++) {
        double u, v;
        do {
            u = drand48();
            v = drand48();
        } while (u + v <= 1);
        
        Vertex lightPoint = objects.back()->point(u, v);
        
        
        Vector toLight = lightPoint - point;
        double lightDistance = toLight.magnitude();
        toLight = toLight.normalize();
        
        double visibility = 1;
        
        Ray shadowRay = Ray(point + toLight * 0.01, point + toLight);
        if( trace(shadowRay, objects) ){
            Vector intersected = shadowRay.intersection - point;
            if(intersected.magnitude() < lightDistance) visibility = 0;
        }
        
        double geometric = normal.dot(toLight) * objects.back()->getNormal().dot(toLight);
        if (geometric < 0) geometric = 0;
        
        double d2 = lightDistance * lightDistance;
        
        result = result + ColorDbl(L0,L0,L0) * orenNayarBRDF(toLight, out, normal, sigma) * (geometric * visibility / d2);
    }
    
    return result * (1.0 / (double)M);
}

Ray refractionDirection(Vertex point, Vector incoming, Vector normal, double eta) {
    
    double n_i = normal.dot(incoming);
    double k = 1.0 - eta * eta * (1.0 - n_i * n_i);
    Vector refracted = incoming * eta - normal * (eta * n_i + sqrt(k));
    
    return Ray(point + refracted * 0.001, point + refracted);
}

double reflectionCoefficient(double n1, double n2, double cosi) {
    double R0 = (n1 - n2)/(n1 + n2);
    R0 = R0 * R0;
    
    return R0 + (1 - R0) * pow(1 - cosi, 5);
}

Ray randomDirection(Vertex point, Vector normal) {
    
    // Setup local Cartesian coordinate system with vectors normal, a & b
    Vector a, b;
    
    if( abs(normal.x) > abs(normal.y) ) {
        a = Vector(normal.z, 0, -normal.x).normalize();
    }
    else {
        a = Vector(0, -normal.z, -normal.y).normalize();
    }
    
    b = normal.cross(a);
    
    // Create a random direction
    double x = drand48();
    double y = drand48();
    
    double theta = asin(sqrt(y));
    double phi = 2 * M_PI * x;
    
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
    
    
    // Create the ray
    Ray result = Ray(point + direction * 0.001, point + direction);
    
    return result;
}

float orenNayarBRDF(Vector in, Vector out, Vector normal, double sigma)
{
    double A = 1 - (sigma*sigma / (2 * (sigma*sigma + 0.33)));
    double B = 0.45 * sigma*sigma / (sigma*sigma + 0.09);
    
    double phi_in = acos((in * -1).dot(normal));
    double phi_out = acos((out * -1).dot(normal));

    //Project in and out onto the surface plane and calculate the angle between them
    in = in - normal * in.dot(normal);
    out = out - normal * out.dot(normal);
    double theta_diff = acos(in.dot(out));
    
    return (1 / M_PI) * (A + B * max(0.0, cos(theta_diff)) * sin(phi_in) * sin(phi_out));
}

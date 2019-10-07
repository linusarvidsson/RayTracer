//#include "SDL.h"
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <memory>

//#include "Triangle.hpp"
#include "Object.hpp"
#include "Tetrahedron.hpp"
#include "Sphere.hpp"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

void castRay(Ray &ray, vector<unique_ptr<Object>> &objects, int lastIntersection, int depth);

int main() {
    
    //------------- Setup SDL window ---------------//
    
    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    
    
    
    //------------- Setup the objects in the scene ---------------//
    
    // Temporary vector containing the direction for the light in the scene
    Vector light = Vector(-1, -0.2, 1).normalize();
    
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
    objects.push_back( make_unique<Sphere>(Sphere(Vertex(9, 4, -1, 1), 2, colors[0])) );
    
    // Add tetrahedra to the vector
    objects.push_back( make_unique<Tetrahedron>( Tetrahedron(Vertex(6,-3,-1,1), Vertex(5,-2,-1,1), Vertex(6.4,-1.6,-1,1), Vertex(5.6,-2,0.2,1), colors[2])) );
    
    // Add all triangles to the vector
    for (int i = 0; i < N_TRIANGLES; i++){
        objects.push_back( make_unique<Triangle>( Triangle(vertices[t_data[4*i]], vertices[t_data[4*i+1]], vertices[t_data[4*i+2]], colors[t_data[4*i+3]])) );
    }
    
    // TEMPORARY CODE FOR TESTING SHADOW RAYS
    vector<unique_ptr<Object>> objects2;
    objects2.push_back( make_unique<Sphere>(Sphere(Vertex(9, 4, -1, 1), 2, colors[0])) );
    objects2.push_back( make_unique<Tetrahedron>( Tetrahedron(Vertex(6,-3,-1,1), Vertex(5,-2,-1,1), Vertex(6.4,-1.6,-1,1), Vertex(5.6,-2,0.2,1), colors[2])) );

    
    //------------- Render Scene ---------------//

    // Temporary ray, will be updated with castRay
    Ray myRay = Ray(Vertex(10, 10, 0, 1), Vertex(10 , 10, -1, 1));
    
    // Loop through all the pixels of the window
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            // Set myRay to go from eye to the current pixel
            myRay = Ray(Vertex(-2, 0, 0, 1), Vertex(-1.6, -0.4 + i*0.001, 0.4 - j*0.001, 1));
            
            // Use castRay to find the point where the ray will intersect
            //castRay(myRay, triangles, N_TRIANGLES, tetrahedra, 1, spheres, 1);
            castRay(myRay, objects, -1, 0);
            
            // The cosine value of the angle between the ray normal and the light source
            double cosTheta = myRay.normal.dot(light);
            
            // If the cosine value is negative the light source is not visable from the intersection point and should be set to 0
            if (cosTheta < 0) cosTheta = 0;
            
            // Radiosity of the point. Ambient factor + the light coming from the light source
            double radiosity = 0.3 + 0.7 * cosTheta;
            
            // Cast a shadow ray from the intersection point
            Ray shadowRay = Ray(myRay.intersection, vertices[13]);
            //castRay(shadowRay, triangles, 0, tetrahedra, 1, spheres, 1);
            castRay(shadowRay, objects2, -1, 0);
            
            // This is some proper bullshit right here
            if (shadowRay.color.r > 0) radiosity = 0.3;
            
            // Update the color of the pixel in the SDL window
            
            SDL_SetRenderDrawColor(renderer, radiosity * myRay.color.r, radiosity * myRay.color.g, radiosity * myRay.color.b, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
    
    //            if (tetrahedra[0].rayIntersection(myRay)) {
    //                Triangle intersection = tetrahedra[0].intersectionPoint(myRay);
    //                Vector normal = intersection.normal;
    //                Vector toLight = vertices[13] - intersection.intersectionPoint(myRay);
    //                double cosTheta = normal.dot(toLight) / (normal.magnitude() * toLight.magnitude());
    //                if (cosTheta < 0) cosTheta = 0;
    //                double radiosity = 0.3 + 0.7 * cosTheta;
    //
    //                SDL_SetRenderDrawColor(renderer, radiosity * tetrahedra[0].color.r, radiosity * tetrahedra[0].color.g, radiosity * tetrahedra[0].color.b, 255);
    //                SDL_RenderDrawPoint(renderer, i, j);
    //            }
    //
    //            if (spheres[0].rayIntersection(myRay)) {
    //                Vertex intersection = spheres[0].intersectionPoint(myRay);
    //                Vector normal = intersection - spheres[0].center;
    //                Vector toLight = vertices[13] - intersection;
    //                double cosTheta = normal.dot(toLight) / (normal.magnitude() * toLight.magnitude());
    //                if (cosTheta < 0) cosTheta = 0;
    //                double radiosity = 0.3 + 0.7 * cosTheta;
    //
    //                SDL_SetRenderDrawColor(renderer, radiosity * spheres[0].color.r, radiosity * spheres[0].color.g, radiosity * spheres[0].color.b, 255);
    //                SDL_RenderDrawPoint(renderer, i, j);
    
    //    Vertex intersection = spheres[0].intersectionPoint(myRay);
    //    Vector normal = intersection - spheres[0].center;
    //    normal = normal.normalize();
    //    Vector L = (myRay.start - myRay.end).normalize();
    //    Vector reflected = ((normal * normal.dot(L)) - L) * 2;
    //    Vertex rayDirection = intersection + reflected;
    //    Ray newRay = Ray(intersection, rayDirection);
    //
    //    for (int x = 0; x < N_TRIANGLES; x++)
    //    {
    //        if (triangles[x].rayIntersection(newRay))
    //        {
    //            double r = 0.2 * spheres[0].color.r + 0.8 * triangles[x].color.r;
    //            double g = 0.2 * spheres[0].color.g + 0.8 * triangles[x].color.g;
    //            double b = 0.2 * spheres[0].color.b + 0.8 * triangles[x].color.b;
    //            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    //            SDL_RenderDrawPoint(renderer, i, j);
    //            break;
    //        }
    //    }
    
    SDL_RenderPresent(renderer);
    
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void castRay(Ray &ray, vector<unique_ptr<Object>> &objects, int lastIntersection, int depth){
    
    for (int i = 0; i < objects.size(); i++){
       
        if (i != lastIntersection && objects[i]->rayIntersection(ray))
        {
            // We're only updating the ray with the first two items here (the circle and tetrahedron) because of temporary fulkod
            if (i < 2){
                Vector incoming = ray.intersection - ray.start;
                ray.end = incoming - ray.normal * 2 * (incoming.dot(ray.normal));
                ray.start = ray.intersection;
                castRay(ray, objects, i, depth +1);
            }
            return;
        }

    }
    
    
}



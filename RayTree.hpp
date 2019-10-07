//
//  RayTree.hpp
//  RayTracer
//
//  Created by Linus Arvidsson on 2019-10-07.
//  Copyright © 2019 Linus Arvidsson. All rights reserved.
//

#ifndef RayTree_hpp
#define RayTree_hpp

#include <stdio.h>
#include "Vertex.hpp"

class RayTree {
    
public:
    
    struct Node {
        // Vertex containing coordinates of the intersection point
        Vertex intersection;
        
        // Parent ray
        Node* parent;
        // Reflected ray R
        Node* R;
        // Refracted ray T
        Node* T;
        
    };
    
    RayTree();
    ~RayTree();
    
private:
    Node* root;
    
    
};


#endif /* RayTree_hpp */

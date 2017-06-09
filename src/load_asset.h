/*
 * William Emerison Six
 *
 * Copyright 2017 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


bool load_asset(const char* path,
                struct aiScene* aiScene,
                struct aiVector3D *scene_min,
                struct aiVector3D *scene_max,
                struct aiVector3D *scene_center);
  
void get_bounding_box(struct aiScene* aiScene,
                      struct aiVector3D* min,
                      struct aiVector3D* max);

void get_bounding_box_for_node (struct aiScene* aiScene,
                                const struct aiNode* nd,
                                struct aiVector3D* min,
                                struct aiVector3D* max,
                                struct aiMatrix4x4* trafo
                                );


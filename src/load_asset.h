/*
 * William Emerison Six
 *
 * Copyright 2018 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


bool load_asset(const char* path,
                const struct aiScene* aiScene,
                struct aiVector3D *scene_min,
                struct aiVector3D *scene_max,
                struct aiVector3D *scene_center);

void get_bounding_box(const struct aiScene* aiScene,
                      struct aiVector3D* min,
                      struct aiVector3D* max);

void get_bounding_box_for_node (const struct aiScene* aiScene,
                                const struct aiNode* nd,
                                struct aiVector3D* min,
                                struct aiVector3D* max,
                                struct aiMatrix4x4* trafo
                                );


/*
 * William Emerison Six
 *
 * Copyright 2017 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#include "common.h"

#include "load_asset.h"

bool
load_asset(const char* path,
           struct aiScene* aiScene,
           struct aiVector3D *scene_min,
           struct aiVector3D *scene_max,
           struct aiVector3D *scene_center)
{
  if (!aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality))
    return false;
  get_bounding_box(aiScene,scene_min,scene_max);
  scene_center->x = (scene_min->x + scene_max->x) / 2.0f;
  scene_center->y = (scene_min->y + scene_max->y) / 2.0f;
  scene_center->z = (scene_min->z + scene_max->z) / 2.0f;
  return true;
}

void
get_bounding_box(struct aiScene* aiScene,
                 struct aiVector3D* min,
                 struct aiVector3D* max)
{
  struct aiMatrix4x4 trafo;
  aiIdentityMatrix4(&trafo);
  
  min->x = min->y = min->z =  1e10f;
  max->x = max->y = max->z = -1e10f;
  get_bounding_box_for_node(aiScene,
                            aiScene->mRootNode,
                            min,
                            max,
                            &trafo);
}

void
get_bounding_box_for_node (struct aiScene* aiScene,
                           const struct aiNode* nd,
                           struct aiVector3D* min,
                           struct aiVector3D* max,
                           struct aiMatrix4x4* trafo
                           ){
  struct aiMatrix4x4 prev;
  unsigned int n = 0, t;
  
  prev = *trafo;
  aiMultiplyMatrix4(trafo,&nd->mTransformation);
  
  for (; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = aiScene->mMeshes[nd->mMeshes[n]];
    for (t = 0; t < mesh->mNumVertices; ++t) {
      
      struct aiVector3D tmp = mesh->mVertices[t];
      aiTransformVecByMatrix4(&tmp,trafo);
      
      min->x = aisgl_min(min->x,tmp.x);
      min->y = aisgl_min(min->y,tmp.y);
      min->z = aisgl_min(min->z,tmp.z);
      
      max->x = aisgl_max(max->x,tmp.x);
      max->y = aisgl_max(max->y,tmp.y);
      max->z = aisgl_max(max->z,tmp.z);
    }
  }
  
  for (n = 0; n < nd->mNumChildren; ++n) {
    get_bounding_box_for_node(aiScene,
                              nd->mChildren[n],
                              min,
                              max,
                              trafo);
  }
  *trafo = prev;
}

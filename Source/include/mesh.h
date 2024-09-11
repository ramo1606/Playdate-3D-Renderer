#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

/**
 * @brief Represents a 3D mesh.
 */
typedef struct {
    Vector3D* vertices;  /* Dynamic array of vertices */
    Face* faces;         /* Dynamic array of faces */
    Vector3D rotation;   /* Rotation of the mesh */
} Mesh;

/**
 * @brief Loads mesh data for a cube.
 * 
 * @return Mesh* Pointer to the loaded mesh.
 */
Mesh* loadCubeMeshData(void);

/**
 * @brief Loads mesh data from an OBJ file.
 *
 * @param filename The name of the OBJ file to load.
 * @return Mesh* Pointer to the loaded mesh, or NULL if loading failed.
 */
Mesh* loadOBJ(const char* filename);

/**
 * @brief Frees the memory allocated for a mesh.
 *
 * @param mesh Pointer to the mesh to be freed.
 */
void freeMesh(Mesh* mesh);

#endif /* MESH_H */
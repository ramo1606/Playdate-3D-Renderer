#include "global.h"
#include "mesh.h"
#include "patterns.h"
#include "logging.h"
#include "memory.h"
#include "stb_ds.h"

/* Constants for cube mesh */
#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) /* 6 cube faces, 2 triangles per face */

/* Cube vertices */
Vector3D cubeVertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1 }, /* 1 */
    {.x = -1, .y = 1, .z = -1 }, /* 2 */
    {.x = 1, .y = 1, .z = -1 }, /* 3 */
    {.x = 1, .y = -1, .z = -1 }, /* 4 */
    {.x = 1, .y = 1, .z = 1 }, /* 5 */
    {.x = 1, .y = -1, .z = 1 }, /* 6 */
    {.x = -1, .y = 1, .z = 1 }, /* 7 */
    {.x = -1, .y = -1, .z = 1 }  /* 8 */
};

/* Cube faces */
Face cubeFaces[N_CUBE_FACES] = {
    /* front */
    {.a = 1, .b = 2, .c = 3, .pattern = BayerDither02 },
    {.a = 1, .b = 3, .c = 4, .pattern = BayerDither02 },
    /* right */
    {.a = 4, .b = 3, .c = 5, .pattern = BayerDither05 },
    {.a = 4, .b = 5, .c = 6, .pattern = BayerDither05 },
    /* back */
    {.a = 6, .b = 5, .c = 7, .pattern = BayerDither08 },
    {.a = 6, .b = 7, .c = 8, .pattern = BayerDither08 },
    /* left */
    {.a = 8, .b = 7, .c = 2, .pattern = BayerDither11 },
    {.a = 8, .b = 2, .c = 1, .pattern = BayerDither11 },
    /* top */
    {.a = 2, .b = 7, .c = 5, .pattern = BayerDither14 },
    {.a = 2, .b = 5, .c = 3, .pattern = BayerDither14 },
    /* bottom */
    {.a = 6, .b = 8, .c = 1, .pattern = BayerDither16 },
    {.a = 6, .b = 1, .c = 4, .pattern = BayerDither16 }
};

// Custom readline function
int readline(SDFile* file, char* buffer, int maxLength)
{
    int bytesRead = 0;
    char ch;
    while (bytesRead < maxLength - 1)
    {
        if (pd->file->read(file, &ch, 1) != 1)
        {
            break;  // End of file or error
        }

        if (ch == '\n')
        {
            if (bytesRead == 0)
            {
                // Skip empty line
                continue;
            }
            break;
        }
        buffer[bytesRead++] = ch;
    }
    buffer[bytesRead] = '\0';

    return bytesRead;
}

Mesh* loadCubeMeshData(void)
{
    Mesh* mesh = (Mesh*)pdMalloc(sizeof(Mesh));
	if (!mesh)
	{
		LOG_ERROR("Failed to allocate memory for mesh");
		return NULL;
	}

    mesh->vertices = NULL; // Initialize vertices array
    mesh->faces = NULL;    // Initialize faces array
	mesh->rotation = (Vector3D){ 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        Vector3D cubeVertex = cubeVertices[i];
        arrput(mesh->vertices, cubeVertex);
    }
    for (int i = 0; i < N_CUBE_FACES; i++)
    {
        Face cubeFace = cubeFaces[i];
        arrput(mesh->faces, cubeFace);
    }

	return mesh;
}

Mesh* loadOBJ(const char* filename)
{
    SDFile* file = pd->file->open(filename, kFileRead);
    if (!file)
    {
        LOG_ERROR("Failed to open file: %s", filename);
        return NULL;
    }

    Mesh* mesh = (Mesh*)pdMalloc(sizeof(Mesh));
    if (!mesh)
    {
        LOG_ERROR("Failed to allocate memory for mesh");
        pd->file->close(file);
        return NULL;
    }

    mesh->vertices = NULL;
    mesh->faces = NULL;
    mesh->rotation = (Vector3D) { 0.0f, 0.0f, 0.0f };

    char line[256];
    while (readline(file, line, sizeof(line)) > 0)
    {
        if (strncmp(line, "v ", 2) == 0)
        {
            // Vertex
            Vector3D vertex;
            if (pd->system->parseString(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z) != 3)
            {
                LOG_ERROR("Error parsing vertex: %s", line);
                pd->file->close(file);
                return NULL;
            }
            arrput(mesh->vertices, vertex);
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            int vertexIndices[3];
            int textureIndices[3];
            int normalIndices[3];
            if (pd->system->parseString(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertexIndices[0], &textureIndices[0], &normalIndices[0],
                &vertexIndices[1], &textureIndices[1], &normalIndices[1],
                &vertexIndices[2], &textureIndices[2], &normalIndices[2]) != 9)
            {
                LOG_ERROR("Error parsing face: %s", line);
                pd->file->close(file);
                return NULL;
            }
            // OBJ indices are 1-based, convert to 0-based
            Face face =
            {
                .a = vertexIndices[0],
                .b = vertexIndices[1],
                .c = vertexIndices[2]
            };
            arrput(mesh->faces, face);
        }
    }

    pd->file->close(file);

    if (arrlen(mesh->vertices) == 0 || arrlen(mesh->faces) == 0)
    {
        LOG_ERROR("Failed to load any vertices or faces from file: %s", filename);
        freeMesh(mesh);
        return NULL;
    }

    LOG_INFO("Loaded mesh with %d vertices and %d faces", arrlen(mesh->vertices), arrlen(mesh->faces));

    return mesh;
}

void freeMesh(Mesh* mesh)
{
    if (mesh)
    {
        arrfree(mesh->vertices);
        arrfree(mesh->faces);
        pdFree(mesh);
    }
    LOG_INFO("Mesh data freed.");
}
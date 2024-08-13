#include "global.h"
#include "mesh.h"
#include "logging.h"
#include "memory.h"
#include "stb_ds.h"

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
                .a = vertexIndices[0] - 1,
                .b = vertexIndices[1] - 1,
                .c = vertexIndices[2] - 1
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
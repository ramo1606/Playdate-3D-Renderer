#include "global.h"

#include <stdio.h>
#include <stdbool.h>

#include "pd_api.h"
#include "memory.h"
#define STBDS_REALLOC(c,p,s) pdRealloc(p,s)
#define STBDS_FREE(c,p)      pdFree(p)
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "mesh.h"
#include "display.h"
#include "vector.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define MAX_VERTICES 1000

/* Playdate API instance */
PlaydateAPI* pd = NULL;

/* Playdate Font */
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

/* Mesh to be loaded from file */
static Mesh* mesh = NULL;
static float rotationX = 0.f, rotationY = 0.f, rotationZ = 0.f;

/* Camera position and cube rotation angles */
Vector3D cameraPosition = { .x = 0.f, .y = 0.f, .z = 0.f };
float fovFactor = 256.f;

/* Array of triangles that should be rendered frame by frame */
Triangle2D* trianglesToRender = NULL;

static void initialize(void);
static int update(void* userdata);

/* Application setup and initialization */
void setup(void)
{
    initDisplay();

    mesh = loadOBJ("assets/obj/cube.obj");
    if (mesh == NULL)
    {
        LOG_ERROR("Failed to load cube mesh");
        return;
    }
}

/* Process input from the user */
void processInput(void)
{
    PDButtons current;
    PDButtons released;
    pd->system->getButtonState(&current, NULL, &released);

    if (current & kButtonLeft)
        rotationY -= 0.05f;
    if (current & kButtonRight)
        rotationY += 0.05f;
    if (current & kButtonUp)
        rotationX -= 0.05f;
    if (current & kButtonDown)
        rotationX += 0.05f;
    if (current & kButtonA)
        rotationZ -= 0.05f;
    if (current & kButtonB)
        rotationZ += 0.05f;

    if (released & kButtonLeft)
        rotationY = 0.f;
    if (released & kButtonRight)
        rotationY = 0.f;
    if (released & kButtonUp)
        rotationX = 0.f;
    if (released & kButtonDown)
        rotationX = 0.f;
    if (released & kButtonA)
        rotationZ = 0.f;
    if (released & kButtonB)
        rotationZ = 0.f;
}

/* Project a 3D point to 2D space */
Vector2D project(Vector3D point)
{   
    return (Vector2D) { .x = (fovFactor * point.x) / point.z, 
                        .y = (fovFactor * point.y) / point.z };
}

void gameUpdate(void)
{
    // Update cube rotation angles
    mesh->rotation.x += rotationX;
    mesh->rotation.y += rotationY;
    mesh->rotation.z += rotationZ;

    // Process each face of the mesh (cube)
    for (int i = 0; i < arrlen(mesh->faces); i++)
    {
        // Get the vertices that make up the current face
        Face meshFace = mesh->faces[i];
        Vector3D faceVertices[3];
        faceVertices[0] = mesh->vertices[meshFace.a];
        faceVertices[1] = mesh->vertices[meshFace.b];
        faceVertices[2] = mesh->vertices[meshFace.c];

        Vector3D transformedVertices[3];

        // Process each vertex of the face
        for (int j = 0; j < 3; j++)
        {
            Vector3D transformedVertex = faceVertices[j];

            // Apply rotation transformations
            transformedVertex = vector3DRotateX(transformedVertex, mesh->rotation.x);
            transformedVertex = vector3DRotateY(transformedVertex, mesh->rotation.y);
            transformedVertex = vector3DRotateZ(transformedVertex, mesh->rotation.z);

            // Translate vertex relative to camera
            transformedVertex.z += 5.f;

            // Save transformed vertex in the array of transformed vertices
            transformedVertices[j] = transformedVertex;
        }

        // Check backface culling
        Vector3D vectorA = transformedVertices[0]; /*   A   */
        Vector3D vectorB = transformedVertices[1]; /*  / \  */
        Vector3D vectorC = transformedVertices[2]; /* C---B */

        // Get the vector subtraction of B-A and C-A
        Vector3D vectorAB = vector3DSub(vectorB, vectorA);
        Vector3D vectorAC = vector3DSub(vectorC, vectorA);
        vectorAB = vector3DNormalize(vectorAB);
        vectorAC = vector3DNormalize(vectorAC);

        // Compute the face normal (using cross product to find perpendicular)
        Vector3D normal = vector3DCross(vectorAB, vectorAC);
        normal = vector3DNormalize(normal);

        // Find the vector between a point in the triangle and the camera origin
        Vector3D cameraRay = vector3DSub(cameraPosition, vectorA);

        // Calculate how aligned the camera ray is with the face normal (using dot product)
        float dotNormalCamera = vector3DDot(normal, cameraRay);

        // Bypass the triangles that are looking away from the camera
        if (dotNormalCamera < 0)
        {
            continue;
        }

        Triangle2D projectedTriangle;

        for (int j = 0; j < 3; j++)
        {
            // Project the current vertex
            Vector2D projectedPoint = project(transformedVertices[j]);

            // Scale and translate the projected points to the middle of the screen
            projectedPoint.x += (pd->display->getWidth() * 0.5f);
            projectedPoint.y += (pd->display->getHeight() * 0.5f);

            projectedTriangle.points[j] = projectedPoint;
        }

        // Save the projected triangle in the array of triangles to render
        arrput(trianglesToRender, projectedTriangle);
    }
}

void render(void)
{
    clearFramebuffer(kColorBlack);
    drawGrid(kColorWhite);

    for (int i = 0; i < arrlen(trianglesToRender); i++)
    {
        // Extract vertices for the current triangle
        Triangle2D triangle = trianglesToRender[i];

        // Draw the triangle
        drawTriangle(triangle, kColorWhite);
    }

    // Update the Playdate display
    renderBuffer();

    // Free the array of triangles to render
    if (trianglesToRender != NULL)
    {
        arrfree(trianglesToRender);
        trianglesToRender = NULL;
    }
}

static void initialize(void)
{
    setup();
}

static int update(void* userdata)
{
    processInput();
    gameUpdate();
    render();

    pd->system->drawFPS(0, 0);

    return 1;
}

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    (void)arg; // unused

    if ( event == kEventInit )
    {
        pd = playdate;
        font = pd->graphics->loadFont(fontpath, NULL);

        if (font == NULL)
        {
            LOG_ERROR("Could not load font %s", fontpath);
            return 1; // Indicate an error occurred
        }
        else if (event == kEventTerminate)
        {
            //free_mesh();
            arrfree(trianglesToRender);
        }

        initialize();
        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

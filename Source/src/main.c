#include "global.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
static float rotationX = 0.02f, rotationY = 0.02f, rotationZ = 0.04f;

/* Camera position and cube rotation angles */
Vector3D cameraPosition = { .x = 0.f, .y = 0.f, .z = 0.f };
float fovFactor = 256.f;

/* Array of triangles that should be rendered frame by frame */
Triangle2D* trianglesToRender = NULL;

static void initialize(void);
static int update(void* userdata);

/* Compare function for qsort to sort triangles by average depth */
int triangleAvgDepthCompare(const void* a, const void* b)
{
    const Triangle2D* triangleA = (const Triangle2D*)a;
    const Triangle2D* triangleB = (const Triangle2D*)b;

    if (triangleA->avgDepth < triangleB->avgDepth) return -1;
    if (triangleA->avgDepth > triangleB->avgDepth) return 1;
    return 0;
}

/* Application setup and initialization */
void setup(void)
{
    initDisplay();

    renderMode = kRenderWireframe;
    cullingMode = kCullingBackface;

    mesh = loadCubeMeshData();
    /*mesh = loadOBJ("assets/obj/cube.obj");
    if (mesh == NULL)
    {
        LOG_ERROR("Failed to load cube mesh");
        return;
    }*/
}

/* Process input from the user */
void processInput(void)
{
    PDButtons current;
    PDButtons released;
    pd->system->getButtonState(&current, NULL, &released);

    if (released & kButtonRight)
    {
        if (renderMode == kRenderSolidWireframe) 
        {
			renderMode = kRenderWireframe;
		}
		else
		{
			renderMode++;
        }
    }
    if (released & kButtonLeft)
    {
        if (renderMode == kRenderWireframe)
        {
            renderMode = kRenderSolidWireframe;
		}
		else
		{
			renderMode--;
		}
    }

	if (released & kButtonDown)
	{
		if (cullingMode == kCullingNone)
		{
			cullingMode = kCullingBackface;
		}
		else
		{
			cullingMode--;
		}
	}

    if (released & kButtonUp)
	{
		if (cullingMode == kCullingBackface)
		{
			cullingMode = kCullingNone;
		}
		else
		{
			cullingMode++;
		}
	}
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
        faceVertices[0] = mesh->vertices[meshFace.a - 1];
        faceVertices[1] = mesh->vertices[meshFace.b - 1];
        faceVertices[2] = mesh->vertices[meshFace.c - 1];

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

        if (cullingMode == kCullingBackface)
        {
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
        }

        Vector2D projectedPoints[3];

        for (int j = 0; j < 3; j++)
        {
            // Project the current vertex
            projectedPoints[j] = project(transformedVertices[j]);

            // Scale and translate the projected points to the middle of the screen
            projectedPoints[j].x += (pd->display->getWidth() * 0.5f);
            projectedPoints[j].y += (pd->display->getHeight() * 0.5f);

        }

        Triangle2D projectedTriangle = {
            .points = {
                { projectedPoints[0].x, projectedPoints[0].y },
                { projectedPoints[1].x, projectedPoints[1].y },
                { projectedPoints[2].x, projectedPoints[2].y }
            },
			.pattern = meshFace.pattern,
			.avgDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3
        };

        // Save the projected triangle in the array of triangles to render
        arrput(trianglesToRender, projectedTriangle);
    }

    /* Sort the triangles to render by average depth */
    qsort(trianglesToRender, arrlen(trianglesToRender), sizeof(Triangle2D), triangleAvgDepthCompare);
}

void render(void)
{
    clearFramebuffer(kColorBlack);
    drawGrid(kColorWhite);

    for (int i = 0; i < arrlen(trianglesToRender); i++)
    {
        // Extract vertices for the current triangle
        Triangle2D triangle = trianglesToRender[i];

        if (renderMode == kRenderSolid || renderMode == kRenderSolidWireframe)
        {
            // Draw the triangle
            drawFilledTriangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                kColorWhite);
        }
        
        if (renderMode == kRenderWireframe || renderMode == kRenderSolidWireframe || renderMode == kRenderWireframeVertex) 
        {
			LCDColor color = kColorWhite;
            if (renderMode == kRenderSolidWireframe)
            {
				color = kColorBlack;
            }

            drawTriangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                color);
        }

		if (renderMode == kRenderWireframeVertex)
		{
			drawRect(triangle.points[0].x - 2, triangle.points[0].y - 2, 4, 4, kColorWhite);
			drawRect(triangle.points[1].x - 2, triangle.points[1].y - 2, 4, 4, kColorWhite);
			drawRect(triangle.points[2].x - 2, triangle.points[2].y - 2, 4, 4, kColorWhite);
		}
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

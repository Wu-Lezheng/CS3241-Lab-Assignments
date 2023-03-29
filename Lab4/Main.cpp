//============================================================
// STUDENT NAME: Wu Lezheng
// NUS User ID.: E0774554
// COMMENTS TO GRADER:
//
// ============================================================

#include "Util.h"
#include "Vector3d.h"
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Scene.h"
#include "Raytrace.h"
#include <string>


// Constants for Scene 1.
static constexpr int imageWidth1 = 640;
static constexpr int imageHeight1 = 480;
static constexpr int reflectLevels1 = 2;  // 0 -- object does not reflect scene.
static constexpr int hasShadow1 = true;
static constexpr std::string_view outImageFile1 = "img_r2s.png";

// Constants for Scene 2.
static constexpr int imageWidth2 = 640;
static constexpr int imageHeight2 = 480;
static constexpr int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static constexpr int hasShadow2 = true;
static constexpr std::string_view outImageFile2 = "img_scene2.png";



///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage( const std::string &imageFilename, const Scene &scene, 
                  int reflectLevels, bool hasShadow )
{
    int imgWidth = scene.camera.getImageWidth();
    int imgHeight = scene.camera.getImageHeight();

    Image image( imgWidth, imgHeight ); // To store the result of ray tracing.

    double startTime = Util::GetCurrRealTime();
    double startCPUTime = Util::GetCurrCPUTime();

    // Generate image, rendering in parallel on Windows and Linux.
    #ifndef __APPLE__
    #pragma warning( push )
    #pragma warning( disable : 6993 )
    #pragma omp parallel for
    #endif
    for ( int y = 0; y < imgHeight; y++ )
    {
        double pixelPosY = y + 0.5;

        for ( int x = 0; x < imgWidth; x++ )
        {
            double pixelPosX = x + 0.5;
            Ray ray = scene.camera.getRay( pixelPosX, pixelPosY );
            Color pixelColor = Raytrace::TraceRay( ray, scene, reflectLevels, hasShadow );
            pixelColor.clamp();
            image.setPixel( x, y, pixelColor );
        }
    }
    #ifndef __APPLE__
    #pragma warning( pop )
    #endif

    double cpuTimeElapsed = Util::GetCurrCPUTime() - startCPUTime;
    double realTimeElapsed = Util::GetCurrRealTime() - startTime;
    std::cout << "CPU time taken = " << cpuTimeElapsed << "sec" << std::endl;
    std::cout << "Real time taken = " << realTimeElapsed << "sec" << std::endl;

    // Write image to file.
    if ( !image.writeToFile( imageFilename ) ) return;
    else Util::ErrorExit("File: %s could not be written.\n", imageFilename.c_str() );
}



// Forward declarations. These functions are defined later in the file.
void DefineScene1( Scene &scene, int imageWidth, int imageHeight );
void DefineScene2( Scene &scene, int imageWidth, int imageHeight );



int main()
{
// Define Scene 1.

    Scene scene1;
    DefineScene1( scene1, imageWidth1, imageHeight1 );

// Render Scene 1.

    std::cout << "Render Scene 1..." << std::endl;
    RenderImage( std::string(outImageFile1), scene1, reflectLevels1, hasShadow1 );
    std::cout << "Scene 1 completed." << std::endl;

// Delete Scene 1 surfaces.

    for (auto& surface : scene1.surfaces)
    {
        delete surface;
    }


// Define Scene 2.

    Scene scene2;
    DefineScene2( scene2, imageWidth2, imageHeight2 );

// Render Scene 2.

    std::cout << "Render Scene 2..." << std::endl;
    RenderImage( std::string(outImageFile2), scene2, reflectLevels2, hasShadow2 );
    std::cout << "Scene 2 completed." << std::endl;

// Delete Scene 2 surfaces.

    for (auto& surface : scene2.surfaces)
    {
        delete surface;
    }

    std::cout << "All done. Press Enter to exit." << std::endl;
    std::cin.get();
    return 0;
}



///////////////////////////////////////////////////////////////////////////
// Modelling of Scene 1.
///////////////////////////////////////////////////////////////////////////

void DefineScene1( Scene &scene, int imageWidth, int imageHeight )
{
    scene.backgroundColor = Color( 0.2f, 0.3f, 0.5f );

    scene.amLight.I_a = Color( 1.0f, 1.0f, 1.0f ) * 0.25f;

// Define materials.

    // Light red.
    Material lightRed = Material();
    lightRed.k_d = Color( 0.8f, 0.4f, 0.4f );
    lightRed.k_a = lightRed.k_d;
    lightRed.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    lightRed.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    lightRed.n = 64.0f;

    // Light green.
    Material lightGreen = Material();
    lightGreen.k_d = Color( 0.4f, 0.8f, 0.4f );
    lightGreen.k_a = lightGreen.k_d;
    lightGreen.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    lightGreen.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    lightGreen.n = 64.0f;

    // Light blue.
    Material lightBlue = Material();
    lightBlue.k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
    lightBlue.k_a = lightBlue.k_d;
    lightBlue.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    lightBlue.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
    lightBlue.n = 64.0f;

    // Yellow.
    Material yellow = Material();
    yellow.k_d = Color( 0.6f, 0.6f, 0.2f );
    yellow.k_a = yellow.k_d;
    yellow.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    yellow.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    yellow.n = 64.0f;

    // Gray.
    Material gray = Material();
    gray.k_d = Color( 0.6f, 0.6f, 0.6f );
    gray.k_a = gray.k_d;
    gray.k_r = Color( 0.6f, 0.6f, 0.6f );
    gray.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    gray.n = 128.0f;

    // Insert into scene materials vector.
    scene.materials = { lightRed, lightGreen, lightBlue, yellow, gray };


// Define point light sources.

    scene.ptLights.resize(2);

    PointLightSource light0 = { Vector3d(100.0, 120.0, 10.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };
    PointLightSource light1 = { Vector3d(5.0, 80.0, 60.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };

    scene.ptLights = { light0, light1 };


// Define surface primitives.

    scene.surfaces.resize(15);

    auto horzPlane = new Plane( 0.0, 1.0, 0.0, 0.0, scene.materials[2] ); // Horizontal plane.
    auto leftVertPlane = new Plane( 1.0, 0.0, 0.0, 0.0, scene.materials[4] ); // Left vertical plane.
    auto rightVertPlane = new Plane( 0.0, 0.0, 1.0, 0.0, scene.materials[4] ); // Right vertical plane.
    auto bigSphere =  new Sphere( Vector3d( 40.0, 20.0, 42.0 ), 22.0, scene.materials[0] ); // Big sphere.
    auto smallSphere = new Sphere( Vector3d( 75.0, 10.0, 40.0 ), 12.0, scene.materials[1] ); // Small sphere.

    // Cube +y face.
    auto cubePosYTri1 = new Triangle( Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      scene.materials[3] );
    auto cubePosYTri2 = new Triangle( Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      scene.materials[3] );

    // Cube +x face.
    auto cubePosXTri1 = new Triangle( Vector3d( 50.0, 0.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 90.0 ),
                                      scene.materials[3]);
    auto cubePosXTri2 = new Triangle( Vector3d( 50.0, 0.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 50.0, 0.0, 90.0 ),
                                      scene.materials[3] );

    // Cube -x face.
    auto cubeNegXTri1 = new Triangle( Vector3d( 30.0, 0.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      scene.materials[3]);
    auto cubeNegXTri2 = new Triangle( Vector3d( 30.0, 0.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      Vector3d( 30.0, 0.0, 70.0 ),
                                      scene.materials[3] );

    // Cube +z face.
    auto cubePosZTri1 = new Triangle( Vector3d( 50.0, 0.0, 90.0 ),
                                      Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      scene.materials[3]);
    auto cubePosZTri2 = new Triangle( Vector3d( 50.0, 0.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 0.0, 90.0 ),
                                      scene.materials[3] );

    // Cube -z face.
    auto cubeNegZTri1 = new Triangle( Vector3d( 30.0, 0.0, 70.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      scene.materials[3] );
    auto cubeNegZTri2 = new Triangle( Vector3d( 30.0, 0.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      Vector3d( 50.0, 0.0, 70.0 ),
                                      scene.materials[3] );

    scene.surfaces = { horzPlane, leftVertPlane, rightVertPlane, 
                       bigSphere, smallSphere,
                       cubePosXTri1, cubePosXTri2,
                       cubePosYTri1, cubePosYTri2,
                       cubePosZTri1, cubePosZTri2,
                       cubeNegXTri1, cubeNegXTri2,
                       cubeNegZTri1, cubeNegZTri2 };


// Define camera.

    scene.camera = Camera( Vector3d( 150.0, 120.0, 150.0 ),  // eye
                           Vector3d( 45.0, 22.0, 55.0 ),  // lookAt
                           Vector3d( 0.0, 1.0, 0.0 ),  //upVector
                           (-1.0 * imageWidth) / imageHeight,  // left
                           (1.0 * imageWidth) / imageHeight,  // right
                           -1.0, 1.0, 3.0,  // bottom, top, near
                           imageWidth, imageHeight );  // image_width, image_height
}



///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////

void DefineScene2( Scene &scene, int imageWidth, int imageHeight )
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

    scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

    // Define materials.

        // Light red.
    Material lightRed = Material();
    lightRed.k_d = Color(0.8f, 0.4f, 0.4f);
    lightRed.k_a = lightRed.k_d;
    lightRed.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    lightRed.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    lightRed.n = 64.0f;

    // Light green.
    Material lightGreen = Material();
    lightGreen.k_d = Color(0.4f, 0.8f, 0.4f);
    lightGreen.k_a = lightGreen.k_d;
    lightGreen.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    lightGreen.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    lightGreen.n = 64.0f;

    // Light blue.
    Material lightBlue = Material();
    lightBlue.k_d = Color(0.4f, 0.4f, 0.8f) * 0.9f;
    lightBlue.k_a = lightBlue.k_d;
    lightBlue.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    lightBlue.k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
    lightBlue.n = 64.0f;

    // Yellow.
    Material yellow = Material();
    yellow.k_d = Color(0.6f, 0.6f, 0.2f);
    yellow.k_a = yellow.k_d;
    yellow.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    yellow.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    yellow.n = 64.0f;

    // Gray.
    Material gray = Material();
    gray.k_d = Color(0.6f, 0.6f, 0.6f);
    gray.k_a = gray.k_d;
    gray.k_r = Color(0.6f, 0.6f, 0.6f);
    gray.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    gray.n = 128.0f;

    // Steel blue
    Material steelBlue = Material();
    steelBlue.k_d = Color(0.69f, 0.77f, 0.87f);
    steelBlue.k_a = steelBlue.k_d;
    steelBlue.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    steelBlue.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    steelBlue.n = 64.0f;

    // Sienna
    Material sienna = Material();
    sienna.k_d = Color(0.60f, 0.30f, 0.16f);
    sienna.k_a = sienna.k_d;
    sienna.k_r = Color(0.3f, 0.3f, 0.3f);
    sienna.k_rg = Color(0.6f, 0.6f, 0.6f) / 3.0f;
    sienna.n = 128.0f;

    // Glass
    Material glass = Material();
    glass.k_d = Color(0.90f, 0.90f, 0.90f);
    glass.k_a = glass.k_d;
    glass.k_r = Color(0.6f, 0.6f, 0.6f) / 1.5f;
    glass.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    glass.n = 64.0f;

    // Snow
    Material snow = Material();
    snow.k_d = Color(1.00f, 0.98f, 0.98f);
    snow.k_a = snow.k_d;
    snow.k_r = Color(0.1f, 0.1f, 0.1f);
    snow.k_rg = Color(0.1f, 0.1f, 0.1f);
    snow.n = 64.0f;

    // Black
    Material black = Material();
    black.k_d = Color(0.0f, 0.0f, 0.0f);
    black.k_a = black.k_d;
    black.k_r = Color(0.0f, 0.0f, 0.0f);
    black.k_rg = Color(0.0f, 0.0f, 0.0f);
    black.n = 60.0f;

    // Insert into scene materials vector.
    scene.materials = { lightRed, lightGreen, lightBlue, yellow, gray, steelBlue, sienna, glass, snow, black };


    // Define point light sources.

    scene.ptLights.resize(2);

    PointLightSource light0 = { Vector3d(150.0, 120.0, 10.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };
    PointLightSource light1 = { Vector3d(5.0, 80.0, 160.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };

    scene.ptLights = { light0, light1 };


    // Define surface primitives.

    scene.surfaces.resize(15);

    auto horzPlane = new Plane(0.0, 1.0, 0.0, 0.0, scene.materials[1]); // Horizontal plane.
    auto leftVertPlane = new Plane(1.0, 0.0, 0.0, 0.0, scene.materials[2]); // Left vertical plane.
    auto rightVertPlane = new Plane(0.0, 0.0, 1.0, 0.0, scene.materials[2]); // Right vertical plane.

    // Draw house base
    auto houseBase1 = new Triangle(Vector3d(20.0, 2.0, 20.0),
                                   Vector3d(100.0, 2.0, 20.0),
                                   Vector3d(20.0, 2.0, 70.0),
                                   scene.materials[5]);

    auto houseBase2 = new Triangle(Vector3d(20.0, 2.0, 70.0),
                                   Vector3d(100.0, 2.0, 20.0),
                                   Vector3d(100.0, 2.0, 70.0),
                                   scene.materials[5]);

    // Draw walls
    auto houseLeftWall1 = new Triangle(Vector3d(20.0, 2.0, 70.0),
                                       Vector3d(20.0, 62.0, 20.0),
                                       Vector3d(20.0, 2.0, 20.0),
                                       scene.materials[5]);
    
    auto houseLeftWall2 = new Triangle(Vector3d(20.0, 62.0, 70.0),
                                       Vector3d(20.0, 62.0, 20.0),
                                       Vector3d(20.0, 2.0, 70.0),
                                       scene.materials[5]);

    auto houseBackWall1 = new Triangle(Vector3d(20.0, 62.0, 20.0),
                                       Vector3d(100.0, 2.0, 20.0),
                                       Vector3d(20.0, 2.0, 20.0),
                                       scene.materials[5]);

    auto houseBackWall2 = new Triangle(Vector3d(20.0, 62.0, 20.0),
                                       Vector3d(100.0, 62.0, 20.0),
                                       Vector3d(100.0, 2.0, 20.0),
                                       scene.materials[5]);

    auto houseFrontWall1 = new Triangle(Vector3d(20.0, 2.0, 70.0),
                                        Vector3d(20.0, 62.0, 70.0),
                                        Vector3d(100.0, 2.0, 70.0),
                                        scene.materials[5]);

    auto houseFrontWall2 = new Triangle(Vector3d(20.0, 62.0, 70.0),
                                        Vector3d(100.0, 62.0, 70.0),
                                        Vector3d(100.0, 2.0, 70.0),
                                        scene.materials[5]);

    auto houseRightWall1 = new Triangle(Vector3d(100.0, 2.0, 70.0),
                                        Vector3d(100.0, 62.0, 70.0),
                                        Vector3d(100.0, 2.0, 20.0),
                                        scene.materials[5]);

    auto houseRightWall2 = new Triangle(Vector3d(100.0, 62.0, 70.0),
                                        Vector3d(100.0, 62.0, 20.0),
                                        Vector3d(100.0, 2.0, 20.0),
                                        scene.materials[5]);

    // Draw house top
    auto houseTop1 = new Triangle(Vector3d(15.0, 62.0, 15.0),
                                  Vector3d(15.0, 62.0, 75.0),
                                  Vector3d(105.0, 62.0, 15.0),
                                  scene.materials[6]);

    auto houseTop2 = new Triangle(Vector3d(15.0, 62.0, 75.0),
                                  Vector3d(105.0, 62.0, 75.0),
                                  Vector3d(105.0, 62.0, 15.0),
                                  scene.materials[6]);
    // Draw roof
    auto houseRoof1 = new Triangle(Vector3d(105.0, 62.0, 75.0),
                                   Vector3d(105.0, 62.0, 15.0),
                                   Vector3d(105.0, 72.0, 45.0),
                                   scene.materials[6]);

    auto houseRoof2 = new Triangle(Vector3d(15.0, 62.0, 75.0),
                                   Vector3d(15.0, 62.0, 15.0),
                                   Vector3d(15.0, 72.0, 45.0),
                                   scene.materials[6]);

    auto frontRoof1 = new Triangle(Vector3d(15.0, 62.0, 75.0),
                                   Vector3d(105.0, 72.0, 45.0),
                                   Vector3d(15.0, 72.0, 45.0),
                                   scene.materials[0]);

    auto frontRoof2 = new Triangle(Vector3d(15.0, 62.0, 75.0),
                                   Vector3d(105.0, 62.0, 75.0),
                                   Vector3d(105.0, 72.0, 45.0),
                                   scene.materials[0]);

    auto backRoof1 = new Triangle(Vector3d(15.0, 62.0, 15.0),
                                  Vector3d(15.0, 72.0, 45.0),
                                  Vector3d(105.0, 72.0, 45.0),
                                  scene.materials[0]);

    auto backRoof2 = new Triangle(Vector3d(15.0, 62.0, 15.0),
                                  Vector3d(105.0, 72.0, 45.0),
                                  Vector3d(105.0, 62.0, 15.0),
                                  scene.materials[0]);

    // Draw door
    auto doorTop1 = new Triangle(Vector3d(70.0, 22.0, 70.0),
                                 Vector3d(70.0, 22.0, 75.0),
                                 Vector3d(90.0, 22.0, 70.0),
                                 scene.materials[6]);

    auto doorTop2 = new Triangle(Vector3d(70.0, 22.0, 75.0),
                                 Vector3d(90.0, 22.0, 75.0),
                                 Vector3d(90.0, 22.0, 70.0),
                                 scene.materials[6]);

    auto doorBottom1 = new Triangle(Vector3d(70.0, 2.0, 70.0),
                                    Vector3d(70.0, 2.0, 75.0),
                                    Vector3d(90.0, 2.0, 70.0),
                                    scene.materials[6]);

    auto doorBottom2 = new Triangle(Vector3d(70.0, 2.0, 75.0),
                                    Vector3d(90.0, 2.0, 75.0),
                                    Vector3d(90.0, 2.0, 70.0),
                                    scene.materials[6]);

    auto doorFront1 = new Triangle(Vector3d(70.0, 22.0, 75.0),
                                   Vector3d(70.0, 2.0, 75.0),
                                   Vector3d(90.0, 22.0, 75.0),
                                   scene.materials[6]);

    auto doorFront2 = new Triangle(Vector3d(70.0, 2.0, 75.0),
                                   Vector3d(90.0, 2.0, 75.0),
                                   Vector3d(90.0, 22.0, 75.0),
                                   scene.materials[6]);

    auto doorSide1 = new Triangle(Vector3d(70.0, 22.0, 70.0),
                                  Vector3d(70.0, 2.0, 70.0),
                                  Vector3d(70.0, 22.0, 75.0),
                                  scene.materials[6]);

    auto doorSide2 = new Triangle(Vector3d(70.0, 2.0, 70.0),
                                  Vector3d(70.0, 2.0, 75.0),
                                  Vector3d(70.0, 22.0, 75.0),
                                  scene.materials[6]);

    // Draw window
    auto windowTop1 = new Triangle(Vector3d(30.0, 52.0, 70.0),
        Vector3d(30.0, 52.0, 72.0),
        Vector3d(45.0, 52.0, 70.0),
        scene.materials[7]);

    auto windowTop2 = new Triangle(Vector3d(30.0, 52.0, 72.0),
        Vector3d(45.0, 52.0, 72.0),
        Vector3d(45.0, 52.0, 70.0),
        scene.materials[7]);

    auto windowBottom1 = new Triangle(Vector3d(30.0, 32.0, 70.0),
        Vector3d(30.0, 32.0, 72.0),
        Vector3d(45.0, 32.0, 70.0),
        scene.materials[6]);

    auto windowBottom2 = new Triangle(Vector3d(30.0, 32.0, 72.0),
        Vector3d(45.0, 32.0, 72.0),
        Vector3d(45.0, 32.0, 70.0),
        scene.materials[6]);

    auto windowFront1 = new Triangle(Vector3d(30.0, 52.0, 72.0),
        Vector3d(30.0, 32.0, 72.0),
        Vector3d(45.0, 52.0, 72.0),
        scene.materials[7]);

    auto windowFront2 = new Triangle(Vector3d(30.0, 32.0, 72.0),
        Vector3d(45.0, 32.0, 72.0),
        Vector3d(45.0, 52.0, 72.0),
        scene.materials[7]);

    auto windowLeft1 = new Triangle(Vector3d(30.0, 52.0, 70.0),
        Vector3d(30.0, 32.0, 70.0),
        Vector3d(30.0, 52.0, 72.0),
        scene.materials[7]);

    auto windowLeft2 = new Triangle(Vector3d(30.0, 32.0, 70.0),
        Vector3d(30.0, 32.0, 72.0),
        Vector3d(30.0, 52.0, 72.0),
        scene.materials[7]);

    auto windowRight1 = new Triangle(Vector3d(45.0, 52.0, 70.0),
        Vector3d(45.0, 52.0, 72.0),
        Vector3d(45.0, 32.0, 70.0),
        scene.materials[7]);

    auto windowRight2 = new Triangle(Vector3d(45.0, 32.0, 70.0),
        Vector3d(45.0, 52.0, 72.0),
        Vector3d(45.0, 32.0, 72.0),
        scene.materials[7]);

    // Draw snowman
    auto body = new Sphere(Vector3d(50.0, 16.0, 120.0), 16.0, scene.materials[8]);
    auto head = new Sphere(Vector3d(50.0, 38.0, 120.0), 8.0, scene.materials[8]);
    auto leftEye = new Sphere(Vector3d(56.0, 42.0, 116.0), 1.5, scene.materials[9]);
    auto RightEye = new Sphere(Vector3d(56.0, 42.0, 123.0), 1.5, scene.materials[9]);

    auto stick1 = new Triangle(Vector3d(56.0, 16.0, 120.0),
        Vector3d(56.0, 18.0, 120.0),
        Vector3d(56.0, 36.0, 144.0),
        scene.materials[6]);

    auto stick2 = new Triangle(Vector3d(56.0, 16.0, 120.0),
        Vector3d(56.0, 34.0, 144.0),
        Vector3d(56.0, 36.0, 144.0),
        scene.materials[6]);

    scene.surfaces = { horzPlane, leftVertPlane, rightVertPlane, houseBase1, houseBase2, 
        houseLeftWall1, houseLeftWall2, houseBackWall1, houseBackWall2, houseFrontWall1, houseFrontWall2, houseRightWall1, houseRightWall2, 
        houseTop1, houseTop2, houseRoof1, houseRoof2, frontRoof1, frontRoof2, backRoof1, backRoof2, 
        doorTop1, doorTop2, doorBottom1, doorBottom2, doorFront1, doorFront2, doorSide1, doorSide2, 
        windowTop1, windowTop2, windowBottom1, windowBottom2, windowFront1, windowFront2, windowLeft1, windowLeft2, windowRight1, windowRight2, 
        body, head, leftEye, RightEye, stick1, stick2};

    // Define camera.

    scene.camera = Camera(Vector3d(200.0, 150.0, 200.0),  // eye
        Vector3d(45.0, 22.0, 55.0),  // lookAt
        Vector3d(0.0, 1.0, 0.0),  //upVector
        (-1.0 * imageWidth) / imageHeight,  // left
        (1.0 * imageWidth) / imageHeight,  // right
        -1.0, 1.0, 3.0,  // bottom, top, near
        imageWidth, imageHeight);  // image_width, image_height

}

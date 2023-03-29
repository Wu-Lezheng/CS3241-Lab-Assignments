//============================================================
// STUDENT NAME: Wu Lezheng
// NUS User ID.: E0774554
// COMMENTS TO GRADER:
//
// ============================================================

#include <cmath>
#include "Sphere.h"

using namespace std;



bool Sphere::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    Vector3d o = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2 * dot(r.direction(), o);
    double c = dot(o, o) - radius * radius;
    double discriminant = (b * b) - (4 * a * c);
    double t;

    if (discriminant < 0) {
        return false;
    }
    else if (discriminant == 0) {
        t = - b / (2 * a);
    }
    else {
        double t1 = (-b + sqrt(discriminant)) / (2 * a);
        double t2 = (-b - sqrt(discriminant)) / (2 * a);
        if (t2 > 0) {
            t = t2;
        }
        else if (t1 > 0) {
            t = t1;
        }
        else {
            return false;
        }
    }

    if (t >= tmin && t <= tmax)
    {
        // We have a hit -- populat hit record.
        rec.t = t;
        rec.p = r.pointAtParam(t);
        rec.normal = r.pointAtParam(t) - center;
        rec.material = material;
        return true;
    }

    return false;  // YOU CAN REMOVE/CHANGE THIS IF NEEDED.
}



bool Sphere::shadowHit( const Ray &r, double tmin, double tmax ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    Vector3d o = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2 * dot(r.direction(), o);
    double c = dot(o, o) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    double t;

    if (discriminant < 0) {
        return false;
    }
    else if (discriminant == 0) {
        t = -b / (2 * a);
    }
    else {
        double t1 = (-b + sqrt(discriminant)) / (2 * a);
        double t2 = (-b - sqrt(discriminant)) / (2 * a);
        if (t2 > 0) {
            t = t2;
        }
        else if (t1 > 0) {
            t = t1;
        }
        else {
            return false;
        }
        
    }

    return t >= tmin && t <= tmax;
}

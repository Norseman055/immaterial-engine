#include "DEBUGGING.h"
#include "MathEngine.h"
#include "BoundingSphere.h"
#include <math.h>

void MostSeparatedPointsOnAABB(int &min, int &max, Vect *pt, int numPts)
{
    // First find most extreme points along principal axes
	auto minx = 0;
	auto maxx = 0;
	auto miny = 0;
	auto maxy = 0;
	auto minz = 0;
	auto maxz = 0;

    for (int i = 1; i < numPts; i++) 
	{
        if (pt[i][x] < pt[minx][x]) 
			minx = i;
        if (pt[i][x] > pt[maxx][x]) 
			maxx = i;
        if (pt[i][y] < pt[miny][y]) 
			miny = i;
        if (pt[i][y] > pt[maxy][y]) 
			maxy = i;
        if (pt[i][z] < pt[minz][z]) 
			minz = i;
        if (pt[i][z] > pt[maxz][z]) 
			maxz = i;
    }

    // Compute the squared distances for the three pairs of points
	float dist2x = (pt[maxx] - pt[minx]).dot(pt[maxx] - pt[minx]);
    float dist2y = (pt[maxy] - pt[miny]).dot(pt[maxy] - pt[miny]);
    float dist2z = (pt[maxz] - pt[minz]).dot(pt[maxz] - pt[minz]);
    // Pick the pair (min,max) of points most distant
    min = minx;
    max = maxx;
    if (dist2y > dist2x && dist2y > dist2z) 
	{
        max = maxy;
        min = miny;
    }
    if (dist2z > dist2x && dist2z > dist2y) 
	{
        max = maxz;
        min = minz;
    }
}


void SphereOfSphereAndPt(Sphere &s, Vect &p)
{
    // Compute squared distance between point and sphere center
    Vect d = p - s.cntr;

    float dist2 = d.dot( d);
    // Only update s if point p is outside it
    if (dist2 > s.rad * s.rad) 
	{
        float dist = sqrtf(dist2);
        float newRadius = (s.rad + dist) * 0.5f;
        float k = (newRadius - s.rad) / dist;
        s.rad = newRadius;
        s.cntr += d * k;
    }
}

void SphereFromDistantPoints(Sphere &s, Vect *pt, int numPts)
{
    // Find the most separated point pair defining the encompassing AABB
    int min, max;
    MostSeparatedPointsOnAABB(min, max, pt, numPts);

    // Set up sphere to just encompass these two points
    s.cntr = (pt[min] + pt[max]) * 0.5f;
    s.rad = (pt[max] - s.cntr).dot(pt[max] - s.cntr);
    s.rad = sqrtf(s.rad);
}


void RitterSphere(Sphere &s, Vect *pt, int numPts)
{
    // Get sphere encompassing two approximately most distant points
    SphereFromDistantPoints(s, pt, numPts);

    // Grow sphere to include all points
    for (int i = 0; i < numPts; i++)
	{
        SphereOfSphereAndPt(s, pt[i]);
	}
}
#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

class Intersection
{
public:
	Intersection() {};
	~Intersection() {};



	bool Box(float t, Ray R)
	{
		return true;
	}


	bool Sphere(float &t, Ray R)
	{
		float A = (R.getV().x * R.getV().x) + (R.getV().y * R.getV().y) + (R.getV().z * R.getV().z);
		float B = (2 * R.getP().x * R.getV().x) + (2 * R.getP().y * R.getV().y) + (2 * R.getP().z * R.getV().z);
		float C = (R.getP().x * R.getP().x) + (R.getP().y * R.getP().y) + (R.getP().z * R.getP().z) - 1;
		float determinate = (B * B) - (4 * A * C);

		if (determinate < 0) return false;
		// we've hit the edge of the sphere and there is only 1 intersection
		if (determinate == 0.0f)
		{
			t = (-B) / (2 * A);
		}
		// find smaller t
		else
		{
			float t1 = ((-B) + sqrtf(determinate)) / (2 * A);
			float t2 = ((-B) - sqrtf(determinate)) / (2 * A);
			if (t1 < t2) t = t1;
			else t = t2;
		}
		return true;
	}
};

#endif
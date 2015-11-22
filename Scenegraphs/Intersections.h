#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

class Intersection
{
public:
	Intersection() {};
	~Intersection() {};

	bool Box(float &t, Ray R)
	{
		float tnear, tfar, txmin, txmax, tymin, tymax, tzmin, tzmax;
		tnear = -1 * numeric_limits<float>::max();
		tfar = numeric_limits<float>::max();
		float bbmin = -0.5f;
		float bbmax = 0.5f;
		float x = R.getV().x;
		float y = R.getV().y;
		float z = R.getV().z;

		// Check if any direction vector is zero in any plane,
		// Return false if it is not within the bounds
		if ((x == 0 && (R.getP().x < bbmin || R.getP().x > bbmax)) ||
			(y == 0 && (R.getP().y < bbmin || R.getP().y > bbmax)) ||
			(z == 0 && (R.getP().z < bbmin || R.getP().z > bbmax)))
		{
			return false;
		}

		// Find tmin and max for x, compare to overall min and max
		if(x >= 0)
		{
			txmin = (bbmin - R.getP().x) / x;
			txmax = (bbmax - R.getP().x) / x;
		}
		else
		{
			txmin = (bbmax - R.getP().x) / x;
			txmax = (bbmin - R.getP().x) / x;
		}

		// Find tmin and max for y, compare to overall min and max
		if(y >= 0)
		{
			tymin = (bbmin - R.getP().y) / y;
			tymax = (bbmax - R.getP().y) / y;
		}
		else
		{
			tymin = (bbmax - R.getP().y) / y;
			tymax = (bbmin - R.getP().y) / y;
		}

		// Find tmin and max for z, compare to overall min and max
		if(z >= 0)
		{
			tzmin = (bbmin - R.getP().z) / z;
			tzmax = (bbmax - R.getP().z) / z;
		}
		else
		{
			tzmin = (bbmax - R.getP().z) / z;
			tzmax = (bbmin - R.getP().z) / z;
		}

		float tmin = max(txmin, max(tymin, tzmin));
		float tmax = min(txmax, min(tymax, tzmax));

		if (tmin >tmax) return false;
		else 
		{
			t = tmin;
			return true;
		}
	}
		/*if(txmin > tnear) tnear = txmin;
		if(txmax < tfar) tfar = txmax;
		if(tnear > tfar) return false;
		if(tfar < 0) return false;*/


	/*	tymin = (-0.5 - R.getP().y) / y;
		tymax = (0.5 - R.getP().y) / y;
		if(tymin > tymax) swap(tymin, tymax);

		if(tymin > tnear) tnear = tymin;
		if(tymax < tfar) tfar = tymax;
		if(tnear > tfar) return false;
		if(tfar < 0) return false;


		tzmin = (-0.5 - R.getP().z) / z;
		tzmax = (0.5 - R.getP().z) / z;
		if(tzmin > tzmax) swap(tzmin, tzmax);

		if(tzmin > tnear) tnear = tzmin;
		if(tzmax < tfar) tfar = tzmax;
		if(tnear > tfar) return false;
		if(tfar < 0) return false;

		if(tnear < tfar)
			t = tnear;
		else
			t = tfar;
		return true;
		*/
		/*if(abs(R.getV().x) < 0.0000001f)
		{
			tmin = -1 * numeric_limits<float>::max();
			tmax = numeric_limits<float>::max();
			cout << "poop3" << endl;
		}
		else if(x >= 0)
		{
			tmin = (-0.5 - R.getP().x) * x;
			tmax = (0.5 - R.getP().x) * x;
		}
		else
		{
			tmin = (0.5 - R.getP().x) * x;
			tmax = (-0.5 - R.getP().x) * x;
		}

		if(abs(R.getV().y) < 0.0000001f)
		{
			tymin = -1 * numeric_limits<float>::max();
			tymax = numeric_limits<float>::max();
			cout << "poop2" << endl;
		}
		else if(y >= 0)
		{
			tymin = (-0.5 - R.getP().y) * y;
			tymax = (0.5 - R.getP().y) * y;
		}
		else
		{
			tymin = (0.5 - R.getP().y) * y;
			tymax = (-0.5 - R.getP().y) * y;
		}

		if ((tmin > tymax) || (tymin > tmax)) 
			return false; 
		if (tymin > tmin) 
			tmin = tymin;	
		if (tymax < tmax) 
			tmax = tymax; 

		if(abs(R.getV().z) < 0.0000001f)
		{
			tzmin = -1 * numeric_limits<float>::max();
			tzmax = numeric_limits<float>::max();
		}
		else if(z >= 0)
		{
			tzmin = (-0.5 - R.getP().z) * z;
			tzmax = (0.5 - R.getP().z) * z;
		}
		else
		{
			tzmin = (-0.5 - R.getP().z) * z;
			tzmax = (0.5 - R.getP().z) * z;
		}

		if ((tmin > tzmax) || (tzmin > tmax)) 
			return false; 
		if (tzmin > tmin) 
			tmin = tzmin; 
		if (tzmax < tmax) 
			tmax = tzmax; 
			*/


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
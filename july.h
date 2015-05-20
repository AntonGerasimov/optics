#include <iostream>
#include <vector>
#include "math.h"

using namespace std;

#define PI 3.14159265
#define NUMBER 13            // HERE YOU CAN CHANGE NUMBER OF CREATED RAYS {360 : (NUMBER - 1)}


//3) Опционально параболическое зеркало (XY – координаты; lenght – длина зеркала; f – фокус).
class Parabolic	:	public Device {
public:
    float len, p;
	point *F;
	point *OC;
	Parabolic( float x, float y, float _len, float len_f)
	{       
		OC = new point();		//deg from vertical 0 <= deg <= 90  against hour ;
		F = new point();
		OC->x = x;					//	вершина параболы
		OC->y = y;
		F->x = OC->x - len_f;		//	точка фокуса параболы
		F->y = OC->y;
		p = 2 * len_f;
		len = _len;
        x1 = OC->x - len;
        x2 = OC->x - len;
		y1 = OC->y + sqrt(2 * p * fabs(x1 - OC->x));
        y2 = OC->y - sqrt(2 * p * fabs(x2 - OC->x));
		std::cout << "\nCreate Parabolic x: " << x << " y: " << y << "  len: " << len << " focus: " << len_f << "\n";
		std::cout << "X1:" << x1 << " Y1: " << y1 << " X2: " << x2 << " Y2: " << y2 << "\n";
    }

	point *cross_point(RAY *r) const {					//	точка пересечения параболы и луча
        point *p = new point();

		float xn, yn, k_ray, b_ray;
		float xc, yc;
		float dd, bb;
		point p1, p2;

		if (r->deg < 90 || r->deg > 270) {
			if (r->deg == 0) 
			{
				p1.y = r->y - OC->y;
				p1.x = OC->x - Sqr(p1.y) / (2 * this->p);
			}
			else {
				bb = 2 * this->p / tan(GradToRad(r->deg));
				dd = sqrt( Sqr(bb)  + 8 * this->p * (OC->x - r->x) );
				p1.y = (-bb + dd) / 2;  
				p2.y = (-bb - dd) / 2;
				p1.x = OC->x - Sqr(p1.y) / (2 * this->p); //p1.y / fabs( tan(GradToRad(r->deg))) + r->x;
				p2.x = p1.x;
				p->x = p1.x; 
				p->y = OC->y - p1.y;
			}

/*			if (sin (GradToRad(r->deg)) >= 0) {
				p->x = p1.x; 
				p->y = OC->y - p1.y;
			}
			else { 
				p->x = p1.x; 
				p->y = OC->y + p1.y;
			}*/

			std::cout << "p->x: " << p->x << " p->y: " << p->y << " r->deg: " << r->deg << "\n";
			return p;
		} else {
			;
		}

        return NULL;
    }


    void change_direction(RAY *r, point *p) const			//	Параболическое зеркало
    {
		std::cout << "\nPrev r->deg: " << r->deg << "\n";	//	исходный угол луча
		float a1 = p->x - r->x;
		float b1 = r->y - p->y;
		float a2 = -1;
		float b2 = p->y - OC->y;
		float deg_f = RadToGrad(atan((a1 * b2 - a2 * b1) / (a1 * a2 + b1 * b2)));		//	угол между лучом и нормалью
		std::cout << "Deg ray-normal: " << deg_f << "\n";

		if (r->deg > 180 && r->deg <= 360) {
			r->deg = 360 - r->deg + 2 * fabs(deg_f);
		} 
		if (r->deg >= 0 && r->deg <= 180) {
			r->deg = r->deg + (180 + 2 * fabs(deg_f));
		}
		r->deg = r->Deg360(r->deg);
		std::cout << "New r->deg: " << r->deg << "\n";
    }

    const char *getID() const {
        return "Parabolic";
    }
  
    ~Parabolic()
    {
        cout << "Destructure of the " << this->getID() << "\n";
    }
};


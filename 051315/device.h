#include <iostream>
#include <vector>
#include "math.h"
#include "ray.h"
using namespace std;

#define PI 3.14159265


float LenVect(point *p1, point *p2) {
        return sqrt(Sqr(p2->x - p1->x) + Sqr(p2->y - p1->y));
}

float Scalar(point *p1, point *p2) {
        return p1->x * p2->x + p1->y * p2->y;
}

point *SumVect(point *p1, point *p2) {
        point *sum_p = new point;
        sum_p->x = p1->x + p2->x;
        sum_p->y = p1->y + p2->y;
        return sum_p;
}
point *SubVect(point *p1, point *p2) {
        point *sub_p = new point;
        sub_p->x = p2->x - p1->x;
        sub_p->y = p2->y - p1->y;
        return sub_p;
}
class Prism;

int orient (float x1, float y1, float x2, float y2, float x3, float y3, float this_deg) {// x1,y1; x2,y2 - line   x3,y3 - point
    // cout << "x1 = " << x1 << " y1 = " << y1 << " x2 = " << x2 << " y2 = " << y2 << " x3 = " << x1 << " y3 = " << y3 << "\n";
    if (this_deg == 0){
        if (y3 <= y2){
            
            // cout << "trying 1 \n";
            return 1;
        }
        else{
            // cout << "trying -1 \n";
            return -1;
        }
    }
    else {
        if (((x3 -x1) * (y2 - y1) - (y3 -y1) * (x2 - x1)) >= 0)
            return 1;
        else
            return -1;
    }
}

class Device{
public:
    float x1, x2, y1, y2, deg, f ;      // y1 - up y2 - down x1 - left x2 - right
    virtual void change_direction(RAY * r, point * p) const = 0;
    //virtual const char *getID() const = 0;
    virtual int getID()
    {
        return -1;
    }
    virtual point * cross_point (RAY * r) const = 0;
};

class Disc : public Device{     // n > 1 !!!
public:
    float  w,l,n, x3,y3, x4, y4, deg; // w - width
    
public:
    Disc ( float x, float y, float l_0, float w_0, float deg_0, float n_0){
        float x_1 = x - (float)(l_0/2) * sin (deg_0 * PI / 180) - (float) w_0 / 2 * cos (deg_0 * PI / 180) ;
        float x_2 = x + (float)(l_0/2) * sin (deg_0 * PI / 180) - (float) w_0 / 2 * cos (deg_0 * PI / 180) ;
        float y_1 = y - (float)(l_0/2) * cos (deg_0 * PI / 180) + (float) w_0 / 2 * sin (deg_0 * PI / 180) ;
        float y_2 = y + (float)(l_0/2) * cos (deg_0 * PI / 180) + (float) w_0 / 2 * sin (deg_0 * PI / 180) ;
        
        float x_3 = x_1 + (float)(w_0) * cos (deg_0 * PI / 180);
        float x_4 = x_2 + (float)(w_0) * cos (deg_0 * PI / 180);
        float y_3 = y_1 - (float)(w_0) * sin (deg_0 * PI / 180);
        float y_4 = y_2 - (float)(w_0) * sin (deg_0 * PI / 180);
        
        deg = deg_0;
        w = w_0;
        l = l_0;
        n = n_0;
        
        x1 = x_1 * cos (deg * PI / 180) - y_1 * sin (deg * PI / 180);
        x2 = x_2 * cos (deg * PI / 180) - y_2 * sin (deg * PI / 180);
        x3 = x_3 * cos (deg * PI / 180) - y_3 * sin (deg * PI / 180);
        x4 = x_4 * cos (deg * PI / 180) - y_4 * sin (deg * PI / 180);
        
        
        y1 = x_1 * sin (deg * PI / 180) + y_1 * cos (deg * PI / 180);
        y2 = x_2 * sin (deg * PI / 180) + y_2 * cos (deg * PI / 180);
        y3 = x_3 * sin (deg * PI / 180) + y_3 * cos (deg * PI / 180);
        y4 = x_4 * sin (deg * PI / 180) + y_4 * cos (deg * PI / 180);
        
        
        cout << "x_1 = " << x_1 << " x_2 = " << x_2 << " x_3 = " << x_3 << " x_4 = " << x_4 << " y_1 = " << y_1 << " y_2 = " << y_2 << " y_3 = " << y_3 << " y_4 = " << y_4 << "\n";

        cout << "x1 = " << x1 << " x2 = " << x2 << " x3 = " << x3 << " x4 = " << x4 << " y1 = " << y1 << " y2 = " << y2 << " y3 = " << y3 << " y4 = " << y4 << "\n";
        
        

    }
    int getID()
    {
        return 4;
    }
    point * cross_point (RAY * r) const {

        point * p = new point  ();
        float p_x, p_y;
        float r_x, r_y;
        r_x = r->x;
        r_y = r->y;
        r->x = r_x * cos (this->deg * PI / 180) - r_y * sin (this->deg * PI / 180);
        r->y = r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
        if (r->deg >= this->deg)
            r->deg = r->deg - this->deg;
        else
            r->deg = 360 + r->deg -this->deg;
        cout << "r->x = " << r->x << " r->y = " << r->y << " r->deg = " << r->deg << "\n";

        
        
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) && ((r->deg <= 90) || (r->deg >= 270))) {
            cout << "h_0\n";
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            cout << "det = " << det << "\n";
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            cout << "det_1 = " << det_1 << "\n";

            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            cout << "det_2 = " << det_2 << "\n";

            
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            
            cout << "p->x = " << p->x << " p->y = " << p->y << "\n";
            cout << "x1 = " << x1 << " x2 = " << x2 << " y1 = " << y1 << " y2 = " << y2  << "\n";


            if ( ( p->y >= y1 ) && ( p->y <= y2 ))
            {
                p_x = p->x;
                p_y = p->y;
                p->x = p_x * cos (this->deg * PI / 180) + p_y * sin (this->deg * PI / 180);
                p->y = - p_x * sin (this->deg * PI / 180) + p_y * cos (this->deg * PI / 180);
                
                r_x = r->x;
                r_y = r->y;
                r->x = r_x * cos (this->deg * PI / 180) + r_y * sin (this->deg * PI / 180);
                r->y = - r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
                r->deg = deg + this->deg;
                if (r->deg > 360)
                    r->deg = r->deg - 360;
                
                return p;
            }
        }
        if ((orient (this->x3,this->y3,this->x4,this->y4,r->x,r->y,1) > 0) && ((r->deg >= 90) && (r->deg <= 270))) {
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x3 - this->x4);
            float det_1 = this->y2 * this->x3 - this->y1 * this->x4 - (this->x3 - this->x4) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x3 - this->y1 * this->x4) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->y >= y1 ) && ( p->y <= y2 ))
            {
                p_x = p->x;
                p_y = p->y;
                p->x = p_x * cos (this->deg * PI / 180) + p_y * sin (this->deg * PI / 180);
                p->y = - p_x * sin (this->deg * PI / 180) + p_y * cos (this->deg * PI / 180);
                
                r_x = r->x;
                r_y = r->y;
                r->x = r_x * cos (this->deg * PI / 180) + r_y * sin (this->deg * PI / 180);
                r->y = - r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
                r->deg = deg + this->deg;
                if (r->deg > 360)
                    r->deg = r->deg - 360;
                
                return p;
            }
        }
        
        if ((orient (this->x1,this->y1,this->x3,this->y3,r->x,r->y,1) > 0) && ((r->deg >= 180) && (r->deg <= 360))) {
            //cout << "this->y1 = "<<  this->y1 << "\n";
            float det = this->x3 - this->x1 - (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180) * (this->y1 - this->y3);
            //cout << "det = " << det << "\n";
            float det_1 = this->x3 * this->y1 - this->x1 * this->y3 - (this->y1 - this->y3) * (r->x +  (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180) * r->y);
            //cout << "det_1 = " << det_1 << "\n";
            float det_2 = (this->x3 - this->x1) * (r->x +  (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180) * r->y) - (this->x3 * this->y1 - this->x1 * this->y3) *  (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180);
            cout << "det_2 = " << det_2 << "\n";
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            float t = p->y;
            p->y = p->x;
            p->x = t;
            //cout << "p->x = " << p->x << "\n";
            //cout << "p->y = " << p->y << "\n";
            if (( p->x >= x1 ) && ( p->x <= x3 ))
            {
                p_x = p->x;
                p_y = p->y;
                p->x = p_x * cos (this->deg * PI / 180) + p_y * sin (this->deg * PI / 180);
                p->y = - p_x * sin (this->deg * PI / 180) + p_y * cos (this->deg * PI / 180);
                
                r_x = r->x;
                r_y = r->y;
                r->x = r_x * cos (this->deg * PI / 180) + r_y * sin (this->deg * PI / 180);
                r->y = - r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
                r->deg = deg + this->deg;
                if (r->deg > 360)
                    r->deg = r->deg - 360;
                
                return p;
            }
        }
        
        if ((orient (this->x2,this->y2,this->x4,this->y4,r->x,r->y,1) < 0) && ((r->deg >= 0) && (r->deg <= 180))) {
            
            float det = this->x4 - this->x2 - (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180) * (this->y2 - this->y4);
            float det_1 = this->x4 * this->y2 - this->x2 * this->y4 - (this->y2 - this->y4) * (r->x +  (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180) * r->y);
            float det_2 = (this->x4 - this->x2) * (r->x +  (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180) * r->y) - (this->x4 * this->y2 - this->x2 * this->y4) *  (float) cos (r->deg * PI / 180) / sin (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            float t = p->y;
            p->y = p->x;
            p->x = t;
            if (( p->x >= x2 ) && ( p->x <= x4 ))
            {
                p_x = p->x;
                p_y = p->y;
                p->x = p_x * cos (this->deg * PI / 180) + p_y * sin (this->deg * PI / 180);
                p->y = - p_x * sin (this->deg * PI / 180) + p_y * cos (this->deg * PI / 180);
                
                r_x = r->x;
                r_y = r->y;
                r->x = r_x * cos (this->deg * PI / 180) + r_y * sin (this->deg * PI / 180);
                r->y = - r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
                r->deg = deg + this->deg;
                if (r->deg > 360)
                    r->deg = r->deg - 360;
                
                return p;
            }
        }
        
        r_x = r->x;
        r_y = r->y;
        r->x = r_x * cos (this->deg * PI / 180) + r_y * sin (this->deg * PI / 180);
        r->y = - r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
        r->deg = deg + this->deg;
        if (r->deg > 360)
            r->deg = r->deg - 360;
        
        return NULL;
    }
    
    
    void change_direction(RAY * r, point * p ) const {
        //cout<< "this->n = " << this->n << "\n";
        float p_x, p_y, r_x, r_y;
        p_x = p->x;
        p_y = p->y;
        p->x = p_x * cos (this->deg * PI / 180) - p_y * sin (this->deg * PI / 180);
        p->y = p_x * sin (this->deg * PI / 180) + p_y * cos (this->deg * PI / 180);
        
        r_x = r->x;
        r_y = r->y;
        r->x = r_x * cos (this->deg * PI / 180) - r_y * sin (this->deg * PI / 180);
        r->y = r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
        if (r->deg >= this->deg)
            r->deg = r->deg - this->deg;
        else
            r->deg = 360 + r->deg -this->deg;

        point * new_p = p;
        float deg = 0;
        
        if ((p->y <= this->y2) && (p->y >= this->y1)) {
            float beta = asin (fabs (sin (r->deg * PI / 180) / this->n) )* 180 / PI;
            if ((((fabs((this->y1 - p->y)) * tan ((90-beta) * PI / 180)) >= this->w) && (r->y >= p->y)) || (((fabs((this->y2 - p->y)) * tan ((90-beta) * PI / 180)) >= this->w) && (r->y <= p->y))) {
                if (r->x < p->x){
                    if (r->y >= p->y)  {
                        new_p->x = p->x + this->w;
                        new_p->y = p->y - this->w * tan ( beta * PI / 180);
                    }
                    else {
                        beta = fabs (beta);
                        new_p->x = p->x + this->w;
                        new_p->y = p->y + this->w * tan ( beta * PI / 180);
                    }
                }
                
                else {
                    if (r->y >= p->y)  {
                        new_p->x = p->x - this->w;
                        new_p->y = p->y - this->w * tan ( beta * PI / 180);
                    }
                    else {
                        beta = fabs (beta);
                        new_p->x = p->x - this->w;
                        new_p->y = p->y + this->w * tan ( beta * PI / 180);
                    }
                }
                deg = r->deg;
            }
            else {
                //cout << "yes \n" ;
                if (r->x < p->x){
                    if (r->y >= p->y)  {
                        new_p->x = p->x + fabs((this->y1 - p->y)) * tan ((90-beta) * PI / 180);
                        new_p->y = this->y1;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 90 - asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 0;
                    }
                    else {
                        beta = fabs (beta);
                        new_p->x = p->x + fabs((this->y2 - p->y)) * tan ((90-beta) * PI / 180);
                        new_p->y = this->y2;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 270 + asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 360;
                    }
                }
                
                else {
                    if (r->y >= p->y)  {
                        new_p->x = p->x - fabs((this->y1 - p->y)) * tan ((90-beta) * PI / 180);
                        new_p->y = this->y1;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 90 + asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 180;
                    }
                    else {
                        beta = fabs (beta);
                        new_p->x = p->x - fabs((this->y2 - p->y)) * tan ((90-beta) * PI / 180);
                        new_p->y = this->y2;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 270 - asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 180;
                    }
                }
            }
        }
        else {
            float beta = asin (fabs (cos (r->deg * PI / 180) / this->n) )* 180 / PI;
            if ((((fabs((this->x1 - p->x)) * tan ((90-beta) * PI / 180)) >= this->l) && (r->x >= p->x)) || (((fabs((this->x3 - p->x)) * tan ((90-beta) * PI / 180)) >= this->l) && (r->x <= p->x))) {
                float beta = asin (fabs (cos (r->deg * PI / 180) / this->n) )* 180 / PI;
                if (r->y < p->y){
                    if (r->x >= p->x)  {
                        new_p->y = p->y + this->l;
                        new_p->x = p->x - this->l * tan ( beta * PI / 180);
                    }
                    else {
                        beta = fabs (beta);
                        new_p->y = p->y + this->l;
                        new_p->x = p->x + this->l * tan ( beta * PI / 180);
                    }
                }
                
                else {
                    if (r->x >= p->x)  {
                        new_p->y = p->y - this->l;
                        new_p->x = p->x - this->l * tan ( beta * PI / 180);
                    }
                    else {
                        //cout << beta ;
                        beta = fabs (beta);
                        new_p->y = p->y - this->l;
                        new_p->x = p->x + this->l * tan ( beta * PI / 180);
                    }
                }
                deg = r->deg;
            }
            else {
                //cout << "yes \n" ;
                if (r->y < p->y){
                    if (r->x >= p->x)  {
                        new_p->y = p->y + fabs((this->x1 - p->x)) * tan ((90-beta) * PI / 180);
                        new_p->x = this->x1;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 180 + asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 270;
                    }
                    else {
                        beta = fabs (beta);
                        new_p->y = p->y + fabs((this->x3 - p->x)) * tan ((90-beta) * PI / 180);
                        new_p->x = this->x3;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 360 - asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 270;
                    }
                }
                // HERE YOU STOPPED
                else {
                    if (r->x >= p->x)  {
                        new_p->y = p->y - fabs((this->x1 - p->x)) * tan ((90-beta) * PI / 180);
                        new_p->x = this->x1;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = 180 - asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 90;
                    }
                    else {
                        beta = fabs (beta);
                        new_p->y = p->y - fabs((this->x3 - p->x)) * tan ((90-beta) * PI / 180);
                        new_p->x = this->x3;
                        if ((cos (beta * PI / 180) * this->n) <= 1)
                            deg = asin (cos (beta * PI / 180) * this->n) * 180 / PI;
                        else
                            deg = 90;
                    }
                }
            }
        }
    //    cout << "beta = " << beta << "\n";
        r->x = new_p->x * cos (this->deg * PI / 180) + new_p->y * sin (this->deg * PI / 180) ;
        r->y = new_p->y * cos (this->deg * PI / 180) - new_p->x * sin (this->deg * PI / 180);
        r->deg = deg + this->deg;
        if (r->deg > 360)
            r->deg = r->deg - 360;

    }
    
    
    ~Disc ()
    {
        cout << "Destructure of the Disc" << "\n";
    }
    
    
};




/*
class Lens_ras : public Device{
public:
    float deg, f, l;
    
public:
    Lens_ras ( float x, float y, float l_0, float deg_0, float f_0){       //deg from vertical 0 <= deg < 90 !!! against hour ;
        x1 = x - (float)(l_0/2) * sin (deg_0 * PI / 180) ;
        x2 = x + (float)(l_0/2) * sin (deg_0 * PI / 180);
        y1 = y - (float)(l_0/2) * cos (deg_0 * PI / 180);
        y2 = y + (float)(l_0/2) * cos (deg_0 * PI / 180);
        l = l_0;
        f = f_0;
        deg = deg_0;
    }
    point * cross_point (RAY * r) const {
        point * p = new point  ();
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) && ((r->deg <= 90) || (r->deg >= 270))){
            cout << "c_here_1 \n";
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) > 0) && ((r->deg >= 90) && (r->deg <= 270))) {
            cout << "c_here_2 \n";
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        return NULL;
    }
    
    void change_direction(RAY * r, point * p ) const
    {
        float l_1 = 0;
        float alpha = 0;
        float alpha_r = 0;
        float c = 0 ;
        float length = (float) sqrt ((r->x - (this->x1 + this->x2)/2) * (r->x - (this->x1 + this->x2)/2)+ (r->y - (this->y1 + this->y2)/2) * (r->y - (this->y1 + this->y2)/2)) ;
        float line_tg = fabs ((float)((r->y - (this->y1 + this->y2)/2) / ((this->x1 + this->x2)/2 - r->x))) ;
        //cout << " line_tg_prev = " << line_tg << "\n";
        float line_deg = atan (line_tg) * 180 / PI;
        cout << "line_deg_prev = " << line_deg <<"\n";
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) {
            if (r->y <= (this->y1 + this->y2)/2)
                line_deg = this->deg + line_deg;
            else
                line_deg = fabs (line_deg - this->deg);
        }
        else {
            if (r->y >= (this->y1 + this->y2)/2)
                line_deg = this->deg + line_deg;
            else
                line_deg = fabs (line_deg - this->deg);
        }
        
        cout << "line_deg = " << line_deg << "\n";
        line_tg = tan (line_deg * PI / 180);
        
        //cout << " line_tg = " << line_tg << "\n";
        float a = length * cos (atan (line_tg));
        //cout << " a = " << a << "\n";
        float b = (float) (this->f * a / (a + this->f));
        //cout << " b = " << b << "\n";
        c =  sqrt ((p->x - (this->x1 + this->x2)/2) * (p->x - (this->x1 + this->x2)/2)+ (p->y - (this->y1 + this->y2)/2) * (p->y - (this->y1 + this->y2)/2)) ;
        
        if (((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0))  || ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0))) {   // точка пересечения и координата луча по разные стороны от опт.оси
            l_1 = c + (float) b * line_tg;
            cout << "here \n";
            alpha = atan ((float) (l_1 / b)) * 180 / PI;
            alpha_r = 180 -alpha ;
            if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2/cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2/cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) { //точка координата луча выше точка пересечения ниже
                alpha_r = alpha_r + 2 * alpha;
                alpha = 360 - alpha;
                cout << " here_0 \n";
            }
            
        }
        else {   //точка персечкения и полоожение луча по одну сторону от опт.оси
            l_1 = fabs (c - (float) b * line_tg);
            alpha = atan ((float) (l_1 / b)) * 180 / PI;
            alpha_r = 180 -alpha ;
            cout << "here_here \n";
            if ((orient (this->x1, this->y1 + this->l / 2/cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)){ // луч выше опт.оси точка пересечения под всмогат.линией сверху
                alpha_r = alpha_r + 2 * alpha;
                alpha = 360 - alpha;
                cout << "here_1 \n";
                
            }
            
            
            if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) {  // луч ниже опт.оси точка пересечения над всмогат.линией снизу
                alpha_r = alpha_r + 2 * alpha;
                alpha = 360 - alpha;
                cout << "alpha =" << alpha << "\n";
                cout << "here_2 \n";
            }
            
        }
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
            r->deg = alpha ;
        else
            r->deg = alpha_r;
        
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) {
            if ((r->deg + this->deg - 360) > 0)
                r->deg = (r->deg + this->deg - 360);
            else
                r->deg = r->deg + this->deg;
        }
        else {
            
            r->deg = r->deg + this->deg;
        }
        
        r->x = p->x;
        r->y = p->y;
    }
    const char *getID() const{
        return "Lens_ras";
    }
    
    ~Lens_ras ()
    {
        cout << "Destructure of the Lens_ras" << "\n";
    }
    
    
};
*/


class Lens : public Device{
public:
    float deg, f, l;
    
public:
    Lens( float x, float y, float l_0, float deg_0, float f_0){       //deg from vertical 0 <= deg < 90 !!! against hour ;
        x1 = x - (float)(l_0/2) * sin (deg_0 * PI / 180) ;
        x2 = x + (float)(l_0/2) * sin (deg_0 * PI / 180);
        y1 = y - (float)(l_0/2) * cos (deg_0 * PI / 180);
        y2 = y + (float)(l_0/2) * cos (deg_0 * PI / 180);
        l = l_0;
        f = f_0;
        deg = deg_0;
    }
    point * cross_point (RAY * r) const {
        point * p = new point  ();
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) && ((r->deg <= 90) || (r->deg >= 270))){
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) > 0) && ((r->deg >= 90) && (r->deg <= 270))) {
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            cout << "p->x = " << p->x << "\n" ;
            p->y = (float)det_2/det;
            cout << "p->x = " << p->x << "\n" ;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        return NULL;
    }
    
    void change_direction(RAY * r, point * p ) const
    {
        float l_1 = 0;
        float alpha = 0;
        float alpha_r = 0;
        float c = 0 ;
        float length = (float) sqrt ((r->x - (this->x1 + this->x2)/2) * (r->x - (this->x1 + this->x2)/2)+ (r->y - (this->y1 + this->y2)/2) * (r->y - (this->y1 + this->y2)/2)) ;
        float line_tg = fabs ((float)((r->y - (this->y1 + this->y2)/2) / ((this->x1 + this->x2)/2 - r->x))) ;
        //cout << " line_tg_prev = " << line_tg << "\n";
        float line_deg = atan (line_tg) * 180 / PI;
        cout << "line_deg_prev = " << line_deg <<"\n";
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) {
            if (r->y <= (this->y1 + this->y2)/2)
                line_deg = this->deg + line_deg;
            else
                line_deg = fabs (line_deg - this->deg);
        }
        else {
            if (r->y >= (this->y1 + this->y2)/2)
                line_deg = this->deg + line_deg;
            else
                line_deg = fabs (line_deg - this->deg);
        }
        
        cout << "line_deg = " << line_deg << "\n";
        line_tg = tan (line_deg * PI / 180);
        
        //cout << " line_tg = " << line_tg << "\n";
        float a = length * cos (atan (line_tg));
        //cout << " a = " << a << "\n";
        if (a != this->f){
            if (a > this->f) {
                float b = (float) (this->f * a / (a - this->f));
                cout << " b = " << b << "\n";
                c =  sqrt ((p->x - (this->x1 + this->x2)/2) * (p->x - (this->x1 + this->x2)/2)+ (p->y - (this->y1 + this->y2)/2) * (p->y - (this->y1 + this->y2)/2)) ;
                
                if (((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - l / 2 / cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)) || ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0))) {   // точка пересечения и координата луча выше или ниже опт.оси
                    l_1 = c + (float) b * line_tg;
                    cout << "here \n";
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    alpha_r = 180 -alpha ;
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2/cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2/cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)) { //точка пересечения и координата луча выше
                        
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        
                        cout << " here_0 \n";
                    }
                    
                }
                else {   //точка персечкения и полоожение луча по разные стороны от опт.оси
                    l_1 = fabs (c - (float) b * line_tg);
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    cout << "here \n";
                    if ((orient (this->x1, this->y1 + this->l / 2/cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)){ // луч выше опт.оси точка пересечения под всмогат.линией снизу
                        
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_1 \n";
                        
                    }
                    
                    
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)) {  // луч ниже опт.оси точка пересечения за всмогат.линией сверху
                        
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "alpha =" << alpha << "\n";
                        cout << "here_2 \n";
                    }
                    
                }
                if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
                    r->deg = alpha ;
                else
                    r->deg = alpha_r;
            }
            else {  // a < f
                cout << "here_3\n";
                float b = fabs((float) (this->f * a / (a - this->f)));
                //cout << " b = " << b << "\n";
                c =  sqrt ((p->x - (this->x1 + this->x2)/2) * (p->x - (this->x1 + this->x2)/2)+ (p->y - (this->y1 + this->y2)/2) * (p->y - (this->y1 + this->y2)/2)) ;
                cout << "c = " << c << "\n";
                
                if (((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) || ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0))) { // точка пересечения и координата луча по разные стороны от опт.оси
                    cout << "here_4\n";
                    l_1 = c + (float) b * line_tg;
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    alpha_r = 180 - alpha;
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)){ //  координата луча выше опт.оси точка персечения ниже опт.оси
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_5\n";
                        cout << "alpha = " << alpha << "\n";
                        
                    }
                    
                }
                else {  // по одну сторону
                    cout << "here_6\n";
                    l_1 = fabs (c - (float) b * line_tg);
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    alpha_r = 180 -alpha ;
                    /*cout << " x1 = " << this->x1 << "\n";
                     cout << " y1 = " << this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) << "\n";
                     cout << " x2 = " << this->x2 << "\n";
                     cout << " y2 = " << this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) << "\n";
                     cout << " x3 = " << p->x << "\n";
                     cout << " y3 = " << p->y << "\n";*/
                    
                    
                    
                    
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) {  // луч выше опт.оси  пересечение под вспомагат.линии сверху
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_7\n";
                        
                    }
                    
                    
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) {  // луч ниже опт.оси  пересечение за вспомагат.линии снизу
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_8\n";
                        
                    }
                }
                if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
                    r->deg = alpha ;
                else
                    r->deg = alpha_r;
            }
        }
        else { // a = f
            alpha = atan (line_tg) * 180 / PI;
            alpha_r = alpha;
            if (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0){
                alpha_r = alpha_r + 2 * alpha;
                alpha = 360 - alpha;
            }
            
            if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
                r->deg = alpha ;
            else
                r->deg = alpha_r;
            
        }
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) {
            if ((r->deg + this->deg - 360) > 0)
                r->deg = (r->deg + this->deg - 360);
            else
                r->deg = r->deg + this->deg;
        }
        else {
            
            r->deg = r->deg + this->deg;
        }
        
        r->x = p->x;
        r->y = p->y;
    }
    const char *getID() const{
        return "Lens";
    }
    
    ~Lens ()
    {
        cout << "Destructure of the Lens" << "\n";
    }
};



class Lens_wide : public Device{
public:
    float deg, l,r1,r2,n,d;
    
public:
    Lens_wide( float x, float y, float l_0, float deg_0, float r1_0, float r2_0, float n_0, float d_0){       //deg from vertical 0 <= deg < 90 !!! against hour ;
        x1 = x - (float)(l_0/2) * sin (deg_0 * PI / 180) ;
        x2 = x + (float)(l_0/2) * sin (deg_0 * PI / 180);
        y1 = y - (float)(l_0/2) * cos (deg_0 * PI / 180);
        y2 = y + (float)(l_0/2) * cos (deg_0 * PI / 180);
        l = l_0;
        r1 = r1_0;
        r2 = r2_0;
        n = n_0;
        d = d_0;
        deg = deg_0;
    }
    point * cross_point (RAY * r) const {
        point * p = new point  ();
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) && ((r->deg <= 90) || (r->deg >= 270))){
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) > 0) && ((r->deg >= 90) && (r->deg <= 270))) {
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            cout << "p->x = " << p->x << "\n" ;
            p->y = (float)det_2/det;
            cout << "p->x = " << p->x << "\n" ;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        return NULL;
    }
    
    void change_direction(RAY * r, point * p ) const
    {
        float l_1 = 0;
        float alpha = 0;
        float alpha_r = 0;
        float c = 0 ;
        float length = (float) sqrt ((r->x - (this->x1 + this->x2)/2) * (r->x - (this->x1 + this->x2)/2)+ (r->y - (this->y1 + this->y2)/2) * (r->y - (this->y1 + this->y2)/2)) ;
        float line_tg = fabs ((float)((r->y - (this->y1 + this->y2)/2) / ((this->x1 + this->x2)/2 - r->x))) ;
        //cout << " line_tg_prev = " << line_tg << "\n";
        float line_deg = atan (line_tg) * 180 / PI;
        cout << "line_deg_prev = " << line_deg <<"\n";
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) {
            if (r->y <= (this->y1 + this->y2)/2)
                line_deg = this->deg + line_deg;
            else
                line_deg = fabs (line_deg - this->deg);
        }
        else {
            if (r->y >= (this->y1 + this->y2)/2)
                line_deg = this->deg + line_deg;
            else
                line_deg = fabs (line_deg - this->deg);
        }
        
        cout << "line_deg = " << line_deg << "\n";
        line_tg = tan (line_deg * PI / 180);
        
        //cout << " line_tg = " << line_tg << "\n";
        float a = length * cos (atan (line_tg));
        //cout << " a = " << a << "\n";
        float f = 0;
        if (orient (this->x1, this->y1, this->x2, this->y2, r->x, r->y,1) < 0)
            f = (float) 1 / ((this->n -1) * ((float)1/this->r1 - (float)1/this->r2 + (float) ((this->n-1) * this->d) / (this->n * this->r1 *this->r2))) ;
        else
            f = (float) 1 / ((this->n -1) * ((float)1/this->r2 - (float)1/this->r1 + (float) ((this->n-1) * this->d) / (this->n * this->r1 *this->r2))) ;
        if (a != this->f){
            if (a > f) {
                float b = (float) (f * a / (a - f));
                cout << " b = " << b << "\n";
                c =  sqrt ((p->x - (this->x1 + this->x2)/2) * (p->x - (this->x1 + this->x2)/2)+ (p->y - (this->y1 + this->y2)/2) * (p->y - (this->y1 + this->y2)/2)) ;
                
                if (((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - l / 2 / cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)) || ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0))) {   // точка пересечения и координата луча выше или ниже опт.оси
                    l_1 = c + (float) b * line_tg;
                    cout << "here \n";
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    alpha_r = 180 -alpha ;
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2/cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2/cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)) { //точка пересечения и координата луча выше
                        
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        
                        cout << " here_0 \n";
                    }
                    
                }
                else {   //точка персечкения и полоожение луча по разные стороны от опт.оси
                    l_1 = fabs (c - (float) b * line_tg);
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    cout << "here \n";
                    if ((orient (this->x1, this->y1 + this->l / 2/cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)){ // луч выше опт.оси точка пересечения под всмогат.линией снизу
                        
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_1 \n";
                        
                    }
                    
                    
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0)) {  // луч ниже опт.оси точка пересечения за всмогат.линией сверху
                        
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "alpha =" << alpha << "\n";
                        cout << "here_2 \n";
                    }
                    
                }
                if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
                    r->deg = alpha ;
                else
                    r->deg = alpha_r;
            }
            else {  // a < f
                cout << "here_3\n";
                float b = fabs((float) (f * a / (a - f)));
                //cout << " b = " << b << "\n";
                c =  sqrt ((p->x - (this->x1 + this->x2)/2) * (p->x - (this->x1 + this->x2)/2)+ (p->y - (this->y1 + this->y2)/2) * (p->y - (this->y1 + this->y2)/2)) ;
                cout << "c = " << c << "\n";
                
                if (((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) || ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) >= 0))) { // точка пересечения и координата луча по разные стороны от опт.оси
                    cout << "here_4\n";
                    l_1 = c + (float) b * line_tg;
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    alpha_r = 180 - alpha;
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)){ //  координата луча выше опт.оси точка персечения ниже опт.оси
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_5\n";
                        cout << "alpha = " << alpha << "\n";
                        
                    }
                    
                }
                else {  // по одну сторону
                    cout << "here_6\n";
                    l_1 = fabs (c - (float) b * line_tg);
                    alpha = atan ((float) (l_1 / b)) * 180 / PI;
                    alpha_r = 180 -alpha ;
                    /*cout << " x1 = " << this->x1 << "\n";
                     cout << " y1 = " << this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) << "\n";
                     cout << " x2 = " << this->x2 << "\n";
                     cout << " y2 = " << this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) << "\n";
                     cout << " x3 = " << p->x << "\n";
                     cout << " y3 = " << p->y << "\n";*/
                    
                    
                    
                    
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0) && (orient (this->x1, this->y1 + (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) {  // луч выше опт.оси  пересечение под вспомагат.линии сверху
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_7\n";
                        
                    }
                    
                    
                    if ((orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) <= 0) && (orient (this->x1, this->y1 + (this->l / 2 + b * line_tg) / cos (this->deg * PI / 180) ,this->x2, this->y2 - (this->l / 2 - b * line_tg) / cos (this->deg * PI / 180), p->x, p->y,this->deg) <= 0)) {  // луч ниже опт.оси  пересечение за вспомагат.линии снизу
                        alpha_r = alpha_r + 2 * alpha;
                        alpha = 360 - alpha;
                        cout << "here_8\n";
                        
                    }
                }
                if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
                    r->deg = alpha ;
                else
                    r->deg = alpha_r;
            }
        }
        else { // a = f
            alpha = atan (line_tg) * 180 / PI;
            alpha_r = alpha;
            if (orient (this->x1, this->y1 + this->l / 2 /cos (this->deg * PI / 180),this->x2, this->y2 - this->l / 2 /cos (this->deg * PI / 180), r->x, r->y,this->deg) >= 0){
                alpha_r = alpha_r + 2 * alpha;
                alpha = 360 - alpha;
            }
            
            if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0)
                r->deg = alpha ;
            else
                r->deg = alpha_r;
            
        }
        if (orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) {
            if ((r->deg + this->deg - 360) > 0)
                r->deg = (r->deg + this->deg - 360);
            else
                r->deg = r->deg + this->deg;
        }
        else {
            
            r->deg = r->deg + this->deg;
        }
        
        r->x = p->x;
        r->y = p->y;
    }
    const char *getID() const{
        return "Lens_wide";
    }
    
    ~Lens_wide ()
    {
        cout << "Destructure of the Lens" << "\n";
    }
};
//1) Плоское зеркало (XY – координаты; lenght – длина зеркала; deg_0 – угол поворота).
class PlainRefl :       public Device {
        public:
    float deg, f;

public:
    PlainRefl( float x, float y, float l, float deg_0){       //deg from vertical 0 <= deg <= 90  against hour ;
        x1 = x - (float)(l/2) * sin (GradToRad(deg_0)) ;
        x2 = x + (float)(l/2) * sin (GradToRad(deg_0));
        y1 = y - (float)(l/2) * cos (GradToRad(deg_0));
        y2 = y + (float)(l/2) * cos (GradToRad(deg_0));
        deg = deg_0;
                std::cout << "Create PlainRefl (" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ") deg: " << deg << "\n";
    }
    point *cross_point(RAY *r) const {
        point *p = new point();
        if ((( this->x1 + this->x2 )/2 > r->x) && ((r->deg <= 90) || (r->deg > 270))){
            float det = this->y2 - this->y1 - tan (GradToRad(r->deg)) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1
                                - this->y1 * this->x2
                                - (this->x1 - this->x2) * (r->y + tan (GradToRad(r->deg)) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (GradToRad(r->deg)) * r->x)
                                - (this->y2 * this->x1 - this->y1 * this->x2) * tan (GradToRad(r->deg));
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if ((( p->x >= x1 ) && ( p->x <= x2 ) && (x1 <= x2)) && (( p->y >= y1 ) && ( p->y <= y2 ) && (y1 <= y2))) return p;
            if ((( p->x <= x1 ) && ( p->x >= x2 ) && (x1 >= x2)) && (( p->y >= y1 ) && ( p->y <= y2 ) && (y1 <= y2))) return p;
            if ((( p->x >= x1 ) && ( p->x <= x2 ) && (x1 <= x2)) && (( p->y <= y1 ) && ( p->y >= y2 ) && (y1 >= y2))) return p;
            if ((( p->x <= x1 ) && ( p->x >= x2 ) && (x1 >= x2)) && (( p->y <= y1 ) && ( p->y >= y2 ) && (y1 >= y2))) return p;
        }
        if ((( this->x1 + this->x2 )/2 < r->x) && ((r->deg >= 90) && (r->deg <= 270))) {
            float det = this->y2 - this->y1 - tan (GradToRad(r->deg)) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (GradToRad(r->deg)) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (GradToRad(r->deg)) * r->x)
                                - (this->y2 * this->x1 - this->y1 * this->x2) * tan (GradToRad(r->deg));
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if ((( p->x >= x1 ) && ( p->x <= x2 ) && (x1 <= x2)) && (( p->y >= y1 ) && ( p->y <= y2 ) && (y1 <= y2))) return p;
            if ((( p->x <= x1 ) && ( p->x >= x2 ) && (x1 >= x2)) && (( p->y >= y1 ) && ( p->y <= y2 ) && (y1 <= y2))) return p;
            if ((( p->x >= x1 ) && ( p->x <= x2 ) && (x1 <= x2)) && (( p->y <= y1 ) && ( p->y >= y2 ) && (y1 >= y2))) return p;
            if ((( p->x <= x1 ) && ( p->x >= x2 ) && (x1 >= x2)) && (( p->y <= y1 ) && ( p->y >= y2 ) && (y1 >= y2))) return p;
        }
        return NULL;
    }

    void change_direction(RAY *r, point *p) const                       //      Плоское зеркало
    {
		r->x = p->x;
		r->y = p->y;
                r->deg = (90 + this->deg) * 2 - r->deg;

                r->deg = r->Deg360(r->deg);
    }
    const char *getID() const {
        return "PlainRefl";
    }

    ~PlainRefl ()
    {
        cout << "Destructure of the PlainRefl" << "\n";
    }
};


//2) Сферическое зеркало (
//      XY – координаты; 
//      lenght – длина зеркала; 
//      R – радиус кривизны; 
//      deg_1, deg_2 – углы, под которыми видны нижняя и верхняя границы зеркала).

class SphereRefl        :       public Device {
        public:
    float deg1, deg2, R;
        point *OptC;
        SphereRefl( float x, float y, float R0, float deg_1, float deg_2)
        {
                OptC = new point();             //deg from vertical 0 <= deg <= 90  against hour ;
                OptC->x = x;
                OptC->y = y;
                R = R0;                                                         //      Радиус кривизны сферы
        x1 = OptC->x - R * sin (GradToRad(deg_1)) ;
        x2 = OptC->x - R * sin (GradToRad(deg_2));
        y1 = OptC->y - R * cos (GradToRad(deg_1));
        y2 = OptC->y - R * cos (GradToRad(deg_2));
                this->f = fabs(this->R);                        //      фокусное расстояние
        deg1 = deg_1;
        deg2 = deg_2;
                std::cout << "\nCreate SphereRefl x: " << x << " y: " << y << "  R: " << R << " deg1:" << deg1 << " deg2: " << deg2 << "\n";
                std::cout << "X1:" << x1 << " Y1: " << y1 << " X2: " << x2 << " Y2: " << y2 << "\n";
    }

        point *cross_point(RAY *r) const {                                      //      точка пересечения сферы и луча
        point *p = new point();

                float xn, yn, k_ray, b_ray;
                float xc, yc;
                if (r->deg != 0) {
                        xn = r->y / tan (GradToRad(r->deg)) + r->x;
                        yn = r->x * tan (GradToRad(r->deg)) + r->y;

                        k_ray = -yn/xn;                                                                                                                         //      уравнение луча
                        b_ray = yn;
                                                                                                                                                                        //      пересечение луча и нормали, проходящей через центр сферы
                        xc = (OptC->y - OptC->x/k_ray - yn) / (k_ray + 1/k_ray);                        //      k_ray = 0 ?
                        yc = -(xc + OptC->x)/ k_ray + OptC->y;
                } else {
                        xc = OptC->x;
                        yc = r->y;
                }
                point pc;
                pc.x = xc;
                pc.y = yc;
                point p_ray;
                p_ray.x = r->x;
                p_ray.y = r->y;
                float l_ray_optc = LenVect(&p_ray, OptC);
                float l_ray_pc = LenVect(&p_ray, &pc);
                float l_d = sqrt(l_ray_optc * l_ray_optc - l_ray_pc * l_ray_pc);
                float l_ray;
                if (this->R >= l_d) {
                        l_ray = fabs(l_ray_pc - sqrt(R * R - l_d * l_d));
                        p->x = r->x + l_ray * cos(GradToRad(r->deg));
                        p->y = r->y - l_ray * sin(GradToRad(r->deg));

                        float delta = Sqr(p->x - OptC->x) + Sqr(p->y - OptC->y) - Sqr(R);
                        std::cout << "1) p->x: " << p->x << " p->y: " << p->y << " r->deg: " << r->deg << " delta:" << delta << "\n";
                        //      Проверка принадлежности сегменту
                        //      Находим угол от вертикали
                        float deg_t;
                        if (fabs(delta) < 0.1) {
                                deg_t  = RadToGrad(atan((OptC->x - p->x) / (OptC->y - p->y)));
      //                          if (OptC->x - p->x<0)deg_t +=180;
                                deg_t = r->Deg360(deg_t);

                                std::cout << "Sphere Point 1: x=" << p->x << " y=" << p->y << " Sphere Deg=" << deg_t << " deg1=" << deg1 << " deg2=" << deg2 << "\n";
                                if (deg1 <= deg_t && deg_t <= deg2 ) return p;
                        }

                        //      Второй корень
                        l_ray = fabs(l_ray_pc + sqrt(R * R - l_d * l_d));
                        p->x = r->x + l_ray * cos(GradToRad(r->deg));
                        p->y = r->y - l_ray * sin(GradToRad(r->deg));

                        delta = Sqr(p->x - OptC->x) + Sqr(p->y - OptC->y) - Sqr(R);
                        std::cout << "2) p->x: " << p->x << " p->y: " << p->y << " r->deg: " << r->deg << " delta:" << delta << "\n";

                        if (fabs(delta) < 0.1) {
                                deg_t  = RadToGrad(atan((OptC->x - p->x) / (OptC->y - p->y)));
                                deg_t = r->Deg360(deg_t);

                                std::cout << "Sphere Point 2: x=" << p->x << " y=" << p->y << " Sphere Deg=" << deg_t << " deg1=" << deg1 << " deg2=" << deg2 << "\n";
                                if (deg1 <= deg_t && deg_t <= deg2 ) return p;
                        }
                }

        return NULL;
    }

    void change_direction(RAY *r, point *p) const                       //      Сферическое зеркало
    {
                std::cout << "\nPrev r->deg: " << r->deg << "\n";
                float a1 = p->x - r->x;
                float b1 = r->y - p->y;
                float a2 = p->x - OptC->x;
                float b2 = OptC->y - p->y;
                float deg_f = RadToGrad(atan((a1 * b2 - a2 * b1) / (a1 * a2 + b1 * b2)));               //      угол между лучом и нормалью к сфере
                std::cout << "Deg ray-normal: " << deg_f << "\n";


		r->x = p->x;
		r->y = p->y;
                r->deg = r->deg + (180 - 2 * fabs(deg_f));
                r->deg = r->Deg360(r->deg);
                std::cout << "New r->deg: " << r->deg << "\n";
    }

    const char *getID() const {
        return "SphereRefl";
    }

    ~SphereRefl()
    {
        cout << "Destructure of the " << this->getID() << "\n";
    }
};
class Gran {
public:
        Prism *prism;
        point *p1,*p2;
        point *normal;
        Gran(Prism *_prism, point *_p1, point *_p2)
        {
                prism = _prism;
                p1 = new point;
                p2 = new point;
                p1->x = _p1->x;
                p1->y = _p1->y;
                p2->x = _p2->x;
                p2->y = _p2->y;
                normal = new point();
                normal->x = (p2->y - p1->y);
                normal->y = (p1->x - p2->x);
        }

        float CheckPointCosA (point *pp) const                          //      точка pp внутри [p1,p2]
        {
                return Scalar(SubVect(p1, pp), SubVect(p2, pp)) / (LenVect(pp, p1) * LenVect(pp, p2));
        }

        bool CheckPoint (point *pp) const                               //      точка pp внутри [p1,p2]
        {
                float cos_a = Scalar(SubVect(p1, pp), SubVect(p2, pp)) / (LenVect(pp, p1) * LenVect(pp, p2));
                if (cos_a < 0) return true;
                return false;
        }

        point *cross_point(RAY *r) const
        {                                                                       //      точка пересечения грани призмы и луча
        point *p = new point();
                float p_check = (r->x - p1->x) * (p2->y - p1->y) - (r->y - p1->y) * (p2->x - p1->x);    //      источник луча находится на грани призмы
                if (fabs(p_check) < 0.1) return NULL;
                float x1, y1, x2, y2, r1, r2;
                float det, det1, det2;
                float xp, yp;

                if (r->deg != 90 && r->deg != 270) {            //      tan 90
                        x1 = p2->y - p1->y;
                        y1 = -(p2->x - p1->x);
                        r1 = p1->x * x1 + p1->y * y1;

                        x2 = tan(GradToRad(r->deg));
                        y2 = 1;
                        r2 = r->x * x2 + r->y;

                        det = x1 * y2 - x2 * y1;                                //      определитель - метод Крамера
                        det1 = r1 * y2 - r2 * y1;
                        det2 = x1 * r2 - x2 * r1;

                        if (det == 0) return NULL;                              //      прямые параллельны
                        p->x = det1 / det;
                        p->y = det2 / det;
                }
                else                                                                            //      луч параллелен оси y
                {
                        x1 = p2->y - p1->y;
                        y1 = -(p2->x - p1->x);
                        r1 = p1->x * x1 + p1->y * y1;

                        p->x = r->x;
                        p->y = (r1 - r->x * x1) / y1;
                }
                if (CheckPoint (p)) return p;


        return NULL;
    }
};
//5) Призма (n - № объекта; XY– координаты; n – показатель преломления).
class   Prism   : public Device {
public:
        int N;
    float len, p, a , b, n_coeff;
    vector <Gran *> gran_vect;
        vector <point *> P;
        float deg1, deg2;
        point *OC;

        Prism(int _N, float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _n_coeff)
        {
                N = _N;
                n_coeff = _n_coeff;
                Gran *gran_t;
                point *p_t = new point;
                p_t->x = _x1;
                p_t->y = _y1;
                P.push_back(p_t);
                p_t = new point;
                p_t->x = _x2;
                p_t->y = _y2;
                P.push_back(p_t);
                p_t = new point;
                p_t->x = _x3;
                p_t->y = _y3;
                P.push_back(p_t);
                gran_t = new Gran((Prism *)this, P[0], P[1]);
                gran_vect.push_back(gran_t);
                gran_t = new Gran((Prism *)this, P[1], P[2]);
                gran_vect.push_back(gran_t);
                gran_t = new Gran((Prism *)this, P[2], P[0]);
                gran_vect.push_back(gran_t);

                std::cout << "\n        Create Prism N: " << N << "\n";
                for (unsigned i = 0; i < P.size();i++) {
                        std::cout << "Point " << i << " x: " << P[i]->x << " y: " << P[i]->y << "\n";
                }
                std::cout << "Coeff_N: " << n_coeff << "\n";
    }

        point *cross_point(RAY *r) const {                                      //      точка пересечения призмы и луча
        point *p = NULL;
                float len_min = 1e10;

                for (unsigned i = 0; i < gran_vect.size(); i++) {
                        point *p_t = gran_vect[i]->cross_point(r);      //      проверка грани призмы и луча
                        if (p_t != NULL) {
                                if (r->CheckRayPoint(p_t)) {
                                        point *pr = new point();
                                        pr->x = r->x;
                                        pr->y = r->y;
                                        float len_t = LenVect(p_t, pr);                 //      минимальное расстояние до источника
                                        if (len_min > len_t) {
                                                len_min = len_t;
                                                p = new point();
                                                p->x = p_t->x;
                                                p->y = p_t->y;
                                        }
                                }
                        }
                }
                if (p != NULL) {
                        std::cout << "Prism intersection  \n";
                        std::cout << "p->x: " << p->x << " p->y: " << p->y << " r->deg: " << r->deg << "\n";
                        return p;
                }

        return NULL;
    }

        Gran *FindGran(point *pp) const
        {
                float cos_a = 0;
                for (unsigned i=0; i < gran_vect.size(); i++) {                      //      нашли грань для точки p
                        cos_a = gran_vect[i]->CheckPointCosA(pp);
                        if (fabs(cos_a + 1) < 0.1)
                        {
                                return gran_vect[i];
                                break;
                        }
                }
                return NULL;
        }

    void change_direction(RAY *r, point *p) const                       //      Призма
    {
                std::cout << "\nPrev r->deg: " << r->deg << "\n";       //      исходный угол луча
                std::cout << "Point_in:   x: " << p->x << " y: " << p->y << " \n";
                std::cout << "Coeff n : " << n_coeff << " \n";
                float cos_a = 0;
                Gran *gran = FindGran(p);

                point *r0 = new point();
                r0->x = r->x;
                r0->y = r->y;

                cos_a = Scalar(SubVect(p, r0), gran->normal) / (LenVect(p, r0) * sqrt(Sqr(gran->normal->x) + Sqr(gran->normal->y)));
                float deg_f = RadToGrad(acos(cos_a));
                std::cout << "Deg_in: " << deg_f << "\n";
                while (fabs(deg_f) > 90 ) {
                        deg_f = 180 - deg_f;
                        std::cout << "Deg_in: " << deg_f << "  180-a \n";
                }
                float deg_r = RadToGrad(asin(sin(GradToRad(deg_f)) / n_coeff));
                std::cout << "Deg_r_in: " << deg_r << "\n";
                while (fabs(deg_r) > 90 ) {
                        deg_r = 180 - deg_r;
                        std::cout << "Deg_r_in: " << deg_r << "  180-a \n";
                }

                float deg_r_new = r->deg - deg_f + deg_r;

                std::cout << "Deg_ray_new " << deg_r_new << "\n";

                RAY *ray_in = new RAY;
                ray_in->set_ray_pos (p->x, p->y, ray_in->Deg360(deg_r_new));

                point *p_new = cross_point(ray_in);                     //      выходная точка
                if (p_new != NULL)
                {
                        Gran *gran_out = FindGran(p_new);
                        std::cout << "\n        Point_out: x: " << p_new->x << " y: " << p_new->y << "\n";

                        cos_a = Scalar(SubVect(p_new, r0), gran_out->normal) / (LenVect(p_new, r0) * sqrt(Sqr(gran_out->normal->x) + Sqr(gran_out->normal->y)));
                        float deg_r_out = RadToGrad(acos(cos_a));                                                       //      угол между лучом и выходной нормалью
                        std::cout << "  Deg_r_out: " << deg_r_out << "\n";
                        while (fabs(deg_r_out) > 90 ) {
                                deg_r_out = 180 - deg_r_out;                    //      0..90
                                std::cout << "  Deg_r_out: " << deg_r_out << "  180-a \n";
                        }
                        if (sin(GradToRad(deg_r_out)) * n_coeff > 1) {
                                std::cout << "  sin(Deg_i_out) * n > 1: " << sin(GradToRad(deg_r_out)) * n_coeff << "\n";
                                deg_r_out = 90 - deg_r_out;
                        }
                        float deg_i_out = RadToGrad(asin(sin(GradToRad(deg_r_out)) * n_coeff));         //      sin(i_out) = sin(r_out) * n
                        std::cout << "  Deg_i_out: " << deg_i_out << "\n";
                        while (fabs(deg_i_out) > 90 ) {
                                deg_i_out = 180 - deg_i_out;                    //      0..90
                                std::cout << "  Deg_i_out: " << deg_i_out << " 180-a \n";
                        }

                        float deg_i_out_new = ray_in->deg - deg_i_out + deg_r_out;
                        std::cout << "Deg_i_out_new: " << deg_i_out_new << "\n";

                        r->deg = r->Deg360(deg_i_out_new);
                        r->x = p_new->x;
                        r->y = p_new->y;
                }
		r->x = p->x;
		r->y = p->y;
                r->deg = r->Deg360(r->deg);                                                             //      угол луча в [0..360]
                std::cout << "New r->deg: " << r->deg << "\n";
    }


    const char *getID() const {
        return "Prism";
    }

    ~Prism()
    {
        cout << "Destructure of the " << this->getID() << "\n";
    }
};



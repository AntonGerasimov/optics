#include <iostream>
#include <vector>
#include "math.h"
using namespace std;

#define PI 3.14159265
#define NUMBER 10            // HERE YOU CAN CHANGE NUMBER OF CREATED RAYS {360 : (NUMBBER - 1)}


struct point  {
    float x ;
    float y ;
};

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

class RAY
{
public:
    float x, y;
    float deg;
    
public:
    void set_ray_pos (float x_0, float y_0, float deg_0)
    { x = x_0; y = y_0; deg = deg_0;}
    void show_ray_pos(int i)
    { cout <<i << ":" <<" x = " << x << " y = " << y << " deg = " << deg << "\n";}
    ~RAY ()
    {
        cout << "Destructure of the ray" << "\n";
    }
};


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
                r->y = -r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
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
                r->y = -r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
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
                r->y = -r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
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
                r->y = -r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);
                return p;
            }
        }
        
        
        
        return NULL;
    }
    
    
    void change_direction(RAY * r, point * p ) const {
        float p_x, p_y, r_x,r_y;
        p_x = p->x;
        p_y = p->y;
        p->x = p_x * cos (this->deg * PI / 180) - p_y * sin (this->deg * PI / 180);
        p->y = p_x * sin (this->deg * PI / 180) + p_y * cos (this->deg * PI / 180);
        r_x = r->x;
        r_y = r->y;
        r->x = r_x * cos (this->deg * PI / 180) - r_y * sin (this->deg * PI / 180);
        r->y = r_x * sin (this->deg * PI / 180) + r_y * cos (this->deg * PI / 180);

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
        //cout << "beta = " << beta << "\n";
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
            f = (float) 1 / ((this->n -1) * ((float)1/this->r1 - (float)1/this->r2 + (float) ((this->n -1) * this->d) / (this->n * this->r1 *this->r2))) ;
        else
            f = (float) 1 / ((this->n -1) * ((float)1/this->r2 - (float)1/this->r1 + (float) ((this->n - 1)* this->d) / (this->n * this->r1 *this->r2))) ;
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




class SCREEN
{
    public :
    float x1, x2, y1, y2,deg;
public:
    SCREEN (float x, float y, float l_0, float deg_0) {
        x1 = x - (float)(l_0/2) * sin (deg_0 * PI / 180);
        x2 = x + (float)(l_0/2) * sin (deg_0 * PI / 180);
        y1 = y - (float)(l_0/2) * cos (deg_0 * PI / 180);
        y2 = y + (float)(l_0/2) * cos (deg_0 * PI / 180);
        deg = deg_0;
    }
    void screen_pos()
    { cout << " x1 = " << x1 << " y1 = " << y1  << " x2 = " << x2 << " y2 = " << y2  <<"\n";}
    
    point * cross_point (RAY * r)  {
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
        }        return NULL;
    }
    
    ~SCREEN ()
    {
        cout << "Destructure of the screen" << "\n";
    }
};

class LASER
{
private:
    float x, y, deg;
    
public:
    LASER (float x_0, float y_0, float deg_0) :
    x(x_0), y(y_0), deg (deg_0)
    { }
    void laser_pos()
    { cout << " x = " << x << " y = " << y  << "\n";}
    RAY * rays_create ()
    {
        RAY * ray = new RAY ();
        ray->set_ray_pos (x, y, deg);
        ray->show_ray_pos(0);
        return ray;
    }
    ~LASER ()
    {
        cout << "Destructure of the source" << "\n";
    }
};




class SOURCE
{
private:
    float x, y;
    
public:
    SOURCE (float x_0, float y_0) :
    x(x_0), y(y_0)
    { }
    void source_pos()
    { cout << " x = " << x << " y = " << y  << "\n";}
    RAY** rays_create ()
    {   const int num_of_rays = NUMBER;
        float deg_step = (float)360 / (num_of_rays - 1);
        RAY ** ray = new RAY *[num_of_rays];
        float deg_i = 0;
        for (int i = 0; i < num_of_rays; i++) {
            ray[i]=new RAY();
            ray[i]->set_ray_pos (x, y, deg_i);
            ray[i]->show_ray_pos(i);
            deg_i = deg_i + deg_step;
        }
        return ray;
    }
    ~SOURCE ()
    {
        cout << "Destructure of the source" << "\n";
    }
};

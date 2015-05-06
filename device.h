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
    virtual const char *getID() const = 0;
    virtual point * cross_point (RAY * r) const = 0;
};

class Disc : public Device{     // n > 1 !!!
public:
    float  w, n, x3,y3, x4, y4; // w - width
    
public:
    Disc ( float x, float y, float l, float w_0, float n_0){
        x1 = x  ;
        x2 = x  ;
        x3 = x + w;
        x4 =x3;
        y1 = y - (float)(l/2) ;
        y2 = y + (float)(l/2) ;
        y3 = y1;
        y4 = y2;
        w = w_0;
        n = n_0;
    }
    point * cross_point (RAY * r) const {
        point * p = new point  ();
        if ((orient (this->x1,this->y1,this->x2,this->y2,r->x,r->y,1) < 0) && ((r->deg <= 90) || (r->deg >= 270))) {
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x1 - this->x2);
            float det_1 = this->y2 * this->x1 - this->y1 * this->x2 - (this->x1 - this->x2) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x1 - this->y1 * this->x2) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->x >= x1 ) && ( p->x <= x2 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        if ((orient (this->x3,this->y3,this->x4,this->y4,r->x,r->y,1) > 0) && ((r->deg >= 90) && (r->deg <= 270))) {
            float det = this->y2 - this->y1 - tan (r->deg * PI / 180) * (this->x3 - this->x4);
            float det_1 = this->y2 * this->x3 - this->y1 * this->x4 - (this->x3 - this->x4) * (r->y + tan (r->deg * PI / 180) * r->x);
            float det_2 = (this->y2 - this->y1) * (r->y + tan (r->deg * PI / 180) * r->x) - (this->y2 * this->x3 - this->y1 * this->x4) * tan (r->deg * PI / 180);
            p->x = (float)det_1/det;
            p->y = (float)det_2/det;
            if (( p->x >= x3 ) && ( p->x <= x4 ) && ( p->y >= y1 ) && ( p->y <= y2 ))
                return p;
        }
        
        
        return NULL;
    }

    
    void change_direction(RAY * r, point * p ) const {
        point * new_p = p;
        float deg = 0;
        float beta = asin (fabs (sin (r->deg * PI / 180) / this->n) )* 180 / PI;
        
        if ((fabs((this->y1 - p->y)) * tan ((90-beta) * PI / 180)) >= this->w) {
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
                    deg = 90 - asin (sin (beta * PI / 180) * this->n) * 180 / PI;
                }
                else {
                    beta = fabs (beta);
                    new_p->x = p->x + fabs((this->y2 - p->y)) * tan ((90-beta) * PI / 180);
                    new_p->y = this->y2;
                    deg = 270 + asin (sin (beta * PI / 180) * this->n) * 180 / PI;
                    //cout << " deg = " << deg << "\n";
                }
            }
            
            else {
                if (r->y >= p->y)  {
                    new_p->x = p->x - fabs((this->y1 - p->y)) * tan ((90-beta) * PI / 180);
                    new_p->y = this->y1;
                    deg = 90 + asin (sin (beta * PI / 180) * this->n) * 180 / PI;
                }
                else {
                    beta = fabs (beta);
                    new_p->x = p->x - fabs((this->y2 - p->y)) * tan ((90-beta) * PI / 180);
                    new_p->y = this->y2;
                    deg = 270 - asin (sin (beta * PI / 180) * this->n) * 180 / PI;
                }
            }
            
        }
        //cout << "beta = " << beta << "\n";
        r->x = new_p->x;
        r->y = new_p->y;
        r->deg = deg;
    }

    const char *getID() const{
        return "Disc";
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
    RAY * rays_create ()
    {   const int num_of_rays = NUMBER;
        float deg_step = (float)360 / (num_of_rays - 1);
        RAY * ray = new RAY [num_of_rays];
        float deg_i = 0;
        for (int i = 0; i < num_of_rays; i++) {
            ray[i].set_ray_pos (x, y, deg_i);
            ray[i].show_ray_pos(i);
            deg_i = deg_i + deg_step;
        }
        return ray;
    }
    ~SOURCE ()
    {
        cout << "Destructure of the source" << "\n";
    }
};




/*int main ()
{
    vector <Device *> my_device;
    
    Device * d_1 = new Lens (50, 100 , 100,  0 , 50);
    my_device.push_back(d_1 );
    
    Device * d_2 = new Lens (125, 100 , 100,  0 , 100);
    my_device.push_back(d_2 );
    
    //Device * d_3 = new Lens_ras (100, 400 , 100,  50 , 2);
    //my_device.push_back(d_3 );
    
    
    
    SCREEN * my_screen = new SCREEN (130, 100, 100, 0);
    //my_screen->screen_pos();
    //SOURCE my_source (0, 100);
    LASER my_laser (0, 100, 15);
    RAY * my_laser_ray = my_laser.rays_create();
    //RAY * my_rays = my_source.rays_create ();
    
    point * cross = NULL;
    // LASER
       for (int i =0; i < 2; i++) {
        cross = my_device[i]->cross_point (  my_laser_ray );
        if ( cross != NULL ) {
            cout << "laser_ray crossed device " << i << " at the point x = " << cross->x << " y = " << cross->y << "\n";
            cout << "prev r->x = " <<  my_laser_ray->x  << "\n"<< "prev r->y = " <<  my_laser_ray->y << " \n prev r->deg = " <<  my_laser_ray->deg  << "\n" ;
            my_device[i]->change_direction(   my_laser_ray,  cross);
            cout << " new r->x = " << my_laser_ray->x << " \n" << " new r->y = " << my_laser_ray->y << " \n new r->deg = " <<  my_laser_ray->deg << " \n" ;
        }
        
    }
       cross = my_screen->cross_point (  my_laser_ray );
    if ( cross != NULL ) {
        cout << "laser_ray crossed screen " << " at the point x = " << cross->x << " y = " << cross->y << "\n";
    }
    
    // SOURCE
    for ( int j = 0; j < NUMBER; j++ ){
        cross = my_screen->cross_point ( & my_rays[j] );
        if ( cross != NULL ) {
            cout << "ray " << j << " crossed screen " << " at the point x = " << cross->x << " y = " << cross->y << "\n";
        }
    }

    for ( int j = 1; j < 2; j++ ) {
        for (int i = 0; i < my_device.size(); i++) {
             cross = my_device[i]->cross_point ( & my_rays[j] );
             if ( cross != NULL ) {
                cout << "ray " << j << " crossed device " << i << " at the point x = " << cross->x << " y = " << cross->y << "\n";
                 cout << "prev r->x = " <<  my_rays[j].x  << "\n"<< "prev r->y = " <<  my_rays[j].y  << "\n" ;
                 my_device[i]->change_direction(  & my_rays[j],  cross);
                 cout << " new r->x = " << my_rays[j].x << " \n" << " new r->y = " << my_rays[j].y << " \n" ;
            

             }
        }
        if (cross == NULL) {
            cout << "ray number " << j << " did not cross devices "<< "\n";
        }
    }

    delete [] my_rays;
    return 0;
}*/


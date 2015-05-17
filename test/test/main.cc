#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
//#include "ray.h"    //was included in "device.h"
//#include "device.h" //was included in "sort.h"
#include "sort.h"
#include <string.h>

int h;
int cs;

void handler(int nsig){
    	if(nsig==SIGINT){
        	close(cs);
        	close(h);
        	printf("Shutting down\n");
        	exit(0);
    	}
}

int main() 
{
    	(void)signal(SIGINT, handler);
    	h = socket(PF_INET, SOCK_STREAM, 0);
    	char buf[512];
    	if (h < 0){
        	perror("Creating socket");
    	}
    	const int PORT = 5678;
    	struct sockaddr_in local;
    	local.sin_family = PF_INET;
    	local.sin_port = htons(PORT);
    	if(bind(h, (sockaddr *)&local, sizeof(local)) < 0){
        	perror("Binding");
        	close(h);
        	return 1;
    	}
    	if (listen(h, 10) < 0){
        	perror("Listening"); 
        	close(h); 
        	return 2;
    	}
    	struct sockaddr_in remote;
    	unsigned remoteLen=sizeof(remote);
    	if((cs=accept(h, (sockaddr *)&remote, &remoteLen))<0){
        	perror("Accepting");
        	return 3;
    	}
    	int rd;
        vector <Device*> my_device;
        SCREEN *my_screen;
        Laser *my_laser;

    	sendto(cs, "1", 1, 0, (sockaddr *)&remote, remoteLen);
    	while((rd=recvfrom(cs, buf, sizeof(buf), 0, (sockaddr *)&remote, &remoteLen))>0){
        	buf[rd]=0;
        	printf("%s\n", buf);
                int check = buf[0] - '0';
                printf("check = %d\n", check);
                switch(check){
                        case 1: //screen
                                {
                                float a1, x1, y1, x2, y2;
                                sscanf(buf, "%f %f %f %f %f", &a1, &x1, &y1, &x2, &y2);
                                my_screen = new SCREEN (x1,y1, x2,y2);
                                printf("Screen was created\n");
                                break;
                                }
                        case 2: //lens f>0
                                {
                                float a1, x, y, l, deg, f;
                                sscanf(buf, "%f %f %f %f %f %f",&a1,&x, &y, &l, &deg, &f);
                                Device  *d = new Lens(x, y, l, deg, f);
                                my_device.push_back(d);
                                printf("New lens f>0 was created\n");
                                break;
				}
			case 3:	//lens f<0
				{
                                float a1, x, y, l, deg, f;
                                sscanf(buf, "%f %f %f %f %f %f",&a1,&x, &y, &l, &deg, &f);
//                                Device  *d = new Lens(x, y, l, deg, f);
//                                my_device.push_back(d);
                                printf("New lens f<0 was created\n");
				break;
				}
			case 4:	//ploskoparallell plastinka
				{
                                float a1, x, y, len, wid, n;
                                sscanf(buf, "%f %f %f %f %f %f",&a1,&x, &y, &len, &wid, &n);
//                                Device  *d = new Lens(x, y, l, deg, f);
//                                my_device.push_back(d);
                                printf("New ploskoparallell plastinka was created\n");
				break;
				}
                        case 5: //Laser
                                {
                                float a1, x, y, deg;
                                sscanf(buf, "%f %f %f %f", &a1,&x, &y, &deg);
                                my_laser = new Laser(x, y, deg);
				printf("New laser was created\n");
                                break;
                                }
			case 6:	//sphere mirror
				{
                                float a1, x, y, r, deg1, deg2;
                                sscanf(buf, "%f %f %f %f %f %f",&a1,&x, &y, &r, &deg1, &deg2);
//                                Device  *d = new Lens(x, y, l, deg, f);
//                                my_device.push_back(d);
                                printf("New sphere mirror was created\n");
				break;
				}
			case 7:	//mirror
				{
                                float a1, x, y, deg;
                                sscanf(buf, "%f %f %f %f",&a1,&x, &y, &deg);
//                                Device  *d = new Lens(x, y, l, deg, f);
//                                my_device.push_back(d);
                                printf("New mirror was created\n");
				break;
				}
			case 8:	//triangle prism
				{
                                float a1, x1, y1, x2, y2, x3, y3, n;
                                sscanf(buf, "%f %f %f %f %f %f %f %f",&a1,&x1, &y1, &x2, &y2, &x3, &y3, &n);
//                                Device  *d = new Lens(x, y, l, deg, f);
//                                my_device.push_back(d);
                                printf("New triangle prism was created\n");
				break;
				}
                }
		
        	fflush(stdout);
            if(strcmp(buf, "FINISH\0")!=0)
            {
        	buf[0]=0;
        	sendto(cs, "1", 1, 0, (sockaddr *)&remote, remoteLen);
            }
            else
            {
                break;
            }
    	}
//	Let's imagine we have done it
//first: пробегаем весь отсортированный массив девайсов. Первое пересечение -> break. Пусть пересекло первым второй девайс
//Тогда запускаем новый пробег for 3 to n. И так далее пока не дошли до конца. Если мы успешно прошли весь цикл или сделали брейк на энтом, то
//Запускаем проверку для выходного луча. Пересечет ли он экран? Если да, то в какой точке????. 
//После нахождения каждой из точек отправляем Лене запись. write(wr, "точка1, точка 2", 3).

//	Here we need to sort vector my_device by x
	sort_(my_device);	


 	point *cross = NULL;
	int k = 0; //номер девайса
	bool q = false; //true, если пересечения есть
	char buf_[32];
//let's work with laser first
	while (k < my_device.size()){
		for (int i = k; i < my_device.size(); i++){	
			//cross device;
			cross = my_device[i]-> cross_point(my_laser->ray);
			if (cross != NULL){
				sprintf(buf_, "%f %f %f %f %c", my_laser->ray->x, my_laser->ray->y, cross->x, cross->y, '\0');//new dot
				sendto (cs, buf_, strlen(buf_)+1, 0, (sockaddr *)&remote, remoteLen);
				k = i + 1;
				q = true;
				my_laser->ray->x = cross->x;
				my_laser->ray->y = cross->y;
				break;
			}
		}
		if (q == false){
			break;
		}
	}
	//cross screen
	cross = NULL;
	cross = my_screen->cross_point(my_laser->ray);
	if (cross != NULL){
		sprintf(buf_, "%f %f %f %f %c", my_laser->ray->x, my_laser->ray->y, cross->x, cross->y, '\0');
		sendto(cs, buf_, strlen(buf_)+1, 0, (sockaddr *)&remote, remoteLen);
	}
	else{
		//find граница, куда дойдет луч
//		sprintf(buf_, "%f %f", );
//		sprintf(buf_, "\0");		
		sendto(cs, buf_, strlen(buf_)+1, 0, (sockaddr *)&remote, remoteLen);
	}



/*
        point *cross = NULL;
	char bu[512];
	sprintf("%f %f ", my_laser->ray->x, my_laser->ray->y);
       	cross = my_device[0]->cross_point ( my_laser -> ray );
        if ( cross != NULL ) {
        	printf("ray crossed device  in point x = %f, y = %f\n",  cross->x, cross->y);
//                                cout << "prev r->deg = " <<  my_laser->ray->deg << "\n" << "device_number =" << i << "\n";
//                                my_device[i]->change_direction(my_laser->ray,  cross);
//                                cout << " new r->deg = " << my_laser->ray->deg << " \n";
                my_laser->ray->x = cross->x;
		my_laser->ray->y = cross->y;
		sprintf(bu,"%f %f", cross->x, cross->y);
//		sprintf("\0");
		printf("%s\n", bu);
		sendto(cs, bu, 10, 0, (sockaddr *)&remote, remoteLen);
		bu = "";
		sprintf("%f %f ", my_laser->ray->x, my_laser->ray->y);
        }  
        if (cross == NULL){
		printf("ray did not cross devices\n");
        }
        cross = my_screen->cross_point(my_laser -> ray);
        if (cross !=NULL){
        	printf("ray crossed screen at point x = %f, y = %f\n", cross->x, cross->y);
		sprintf("%f %f", cross-> x, cross -> y );	
//      	sprintf("\0");
//      	printf("%s\n", bu);
        	sendto(cs, bu, 10, 0, (sockaddr *)&remote, remoteLen);
        }
*/
//	bu = "";
    	close(cs);
    	close(h);
}  

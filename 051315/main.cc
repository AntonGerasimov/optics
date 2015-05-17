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
#include "./sort.h"
#include <string.h>
#include <pthread.h>

int h;

void handler(int nsig){
    	if(nsig==SIGINT){
        	close(h);
        	printf("\nShutting down\n");
        	exit(0);
    	}
}
void *func(void *arg);
int main() 
{
    	(void)signal(SIGINT, handler);
    	h = socket(PF_INET, SOCK_STREAM, 0);
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
        while(1)
        {
    	struct sockaddr_in remote;
    	unsigned remoteLen=sizeof(remote);
        int cs;
    	if((cs=accept(h, (sockaddr *)&remote, &remoteLen))<0){
        	perror("Accepting");
        }
        pthread_t thread;
        if(pthread_create(&thread, NULL, func, &cs)<0)
        {
            perror("Thread: ");
    	}
        }
    	close(h);
}

void *func(void* arg)
{
int rd;
char buf[512];
int cs=*(int *)arg;
	char buf_[512];

        vector <Device*> my_device;
	vector <SCREEN *> my_screen;
    /*SCREEN *scr1 = new SCREEN(2500, 0, 5000, 90);
	SCREEN *scr2 = new SCREEN(2500, 5000, 5000, 90); 
	SCREEN *scr3 = new SCREEN(0,2500, 5000, 0);
	SCREEN *scr4 = new SCREEN(5000, 2500, 5000, 0);
	my_screen.push_back(scr1);
	my_screen.push_back(scr2);
	my_screen.push_back(scr3);
	my_screen.push_back(scr4);	*/
        vector<LASER*> my_laser;
        vector<SOURCE*> my_source;
    	if(send(cs, "1", 1, MSG_NOSIGNAL)==-1)
        {
            perror("Can't send:");
            return NULL;
        }
    	while((rd=recv(cs, buf, sizeof(buf), 0))>0){
        	buf[rd]=0;
        	printf("%s\n", buf);
		int check;
		
		if ((buf[1]>='0')&&(buf[1]<='9')){
                	int check_10 = buf[0] - '0';
			check_10 = check_10 * 10;
			int check_1 = buf[1] - '0';
			check = check_10 + check_1;
                	printf("check = %d\n", check);
		}
		else{
			check = buf[0] - '0';
		}
                switch(check){
                    	case 0: //source
                    	{
                        	float a1,x,y;
                        	sscanf(buf, "%f %f %f", &a1, &x, &y);
                        	SOURCE* d=new SOURCE(x,y);
                        	my_source.push_back(d);
                        	printf("New source was created\n");
                        	break;
                    	}
                        case 1: //screen
                                {
                                float a1, x1, y1, x2, y2;
                                sscanf(buf, "%f %f %f %f %f", &a1, &x1, &y1, &x2, &y2);
				SCREEN *my_scr = new SCREEN(x1, y1, x2, y2);
				my_screen.push_back(my_scr);
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
                        case 3: //mirror == PlainRefl
                                {
                                float a1, x, y, l, deg;
                                sscanf(buf, "%f %f %f %f %f",&a1,&x, &y, &l, &deg);
                                Device  *d = new PlainRefl(x, y, l, deg);
                                my_device.push_back(d);
                                printf("New mirror was created\n");
                                break;
                                }

			case 4:	//ploskoparallell plastinka == disc
				{
                                float a1, x, y, len, wid, deg, n;
                                sscanf(buf, "%f %f %f %f %f %f %f",&a1,&x, &y, &len, &wid, &deg, &n);
                                Device  *d = new Disc(x, y, len, wid, deg, n);
                                my_device.push_back(d);
                                printf("New ploskoparallell plastinka was created\n");
				break;
				}
                        case 5: //Laser
                                {
                                float a1, x, y, deg;
                                sscanf(buf, "%f %f %f %f", &a1,&x, &y, &deg);
                                my_laser.push_back(new LASER(x,y,deg));
				printf("New laser was created\n");
                                break;
                                }
                        case 6: //triangle prism
                                {
                                float a1, x1, y1, x2, y2, x3, y3, n;
                                sscanf(buf, "%f %f %f %f %f %f %f %f",&a1,&x1, &y1, &x2, &y2, &x3, &y3, &n);
                                int num = 1;
                                Device  *d = new Prism(num,x1, y1, x2, y2, x3, y3, n);
                                my_device.push_back(d);
                                printf("New triangle prism was created\n");
                                break;
                                }

			case 7:	//sphere mirror
				{
                                float a1, x, y, r0, deg_1, deg_2;
                                sscanf(buf, "%f %f %f %f %f %f",&a1, &x, &y, &deg_1, &deg_2, &r0);
				Device  *d = new SphereRefl(x, y, r0, deg_1-90, deg_2-90);
                                my_device.push_back(d);
                                printf("New sphere mirror was created\n");
				break;
				}
			case 8:	//wide length
				{
                                float a1, x, y, l, deg, r1, r2, n, de;
                                sscanf(buf, "%f %f %f %f %f %f %f %f %f",&a1, &x, &y, &r1, &r2, &deg, &n, &l, &de);
                                Device  *d = new Lens_wide(x, y, l, deg, r1, r2, n, de);
                                my_device.push_back(d);
                                printf("New triangle prism was created\n");
				break;
				}
                }
		
        	fflush(stdout);
		if (strcmp(buf, "FINISH\0")!=0){
        	buf[0]=0;
        	if(send(cs, "1", 1, MSG_NOSIGNAL)==-1)
        {
            perror("Can't send:");
            return NULL;
        }
		}
		else{
			break;
		}
    	}
//	Let's imagine we have done it
//first: пробегаем весь отсортированный массив девайсов. Первое пересечение -> break. Пусть пересекло первым второй девайс
//Тогда запускаем новый пробег for 3 to n. И так далее пока не дошли до конца. Если мы успешно прошли весь цикл или сделали брейк на энтом, то
//Запускаем проверку для выходного луча. Пересечет ли он экран? Если да, то в какой точке????. 
//После нахождения каждой из точек отправляем Лёне запись. write(wr, "точка1, точка 2", 3).

//	Here we need to sort vector my_device by x
	sort_(my_device);	


 	point *cross = NULL;
	unsigned k = 0; //номер девайса
	bool q = false; //true, если пересечения есть
    char temp[1];
    vector<RAY*>my_laser_ray;
    for(unsigned i=0; i<my_laser.size(); i++)
    {
        my_laser_ray.push_back(my_laser[i]->rays_create());
    }
//RAY *my_laser_ray=my_laser->rays_create();
    for(unsigned i=0; i<my_source.size(); i++)
    {
        RAY** rt=my_source[i]->rays_create();
        for(unsigned j=0; j<NUMBER; j++)
        {
            my_laser_ray.push_back(rt[j]);
        }
    }
//let's work with laser first
for(unsigned I=0; I<my_laser_ray.size(); I++)
{
    cross=NULL;
    k=0;
	while (k < my_device.size()){
		for (unsigned i = k; i < my_device.size(); i++){	
			//cross device;
            			cross = my_device[i]-> cross_point(my_laser_ray[I]);
			if (cross != NULL)
            {
				sprintf(buf_, "%f %f %f %f %c", my_laser_ray[I]->x, my_laser_ray[I]->y, cross->x, cross->y, '\0');//new dot
				if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1)
                {
                    perror("Can't send:");
                    return NULL;
                }
                recv(cs, temp, 1, 0);
                k = i + 1;
				q = true;
				/*my_laser_ray->x = cross->x;
				my_laser_ray->y = cross->y;*/
                float tx=cross->x;
                float ty=cross->y;
                my_device[i]->change_direction(my_laser_ray[I], cross);
                if(my_device[i]->getID()==4)
                {
                    sprintf(buf_, "%f %f %f %f %c", tx, ty, my_laser_ray[I]->x, my_laser_ray[I]->y, 0);
				    if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1)
                    {
                        perror("Can't send:");
                        return NULL;
                    }
                    recv(cs, temp, 1, 0);
                }
                printf("I: %d; k: %d; i: %d.\n",I, k, i);
				break;
			}
            else
            {
                k=i+1;
            }
		}
		if (q == false){
            printf("%d\n",I);
			break;
		}
	}
}
	//cross screen
    point * crossing;
    float tx;
    float ty;
    float tdeg;
    for(unsigned I=0; I<my_laser_ray.size(); I++)
    {
//	my_screen->sort();
	int s_num = -1;
        tx=my_laser_ray[I]->x;
        ty=my_laser_ray[I]->y;
        tdeg=my_laser_ray[I]->deg;
	for (unsigned j = 0; j < my_screen.size(); j++){
		cross = NULL;

		cross = my_screen[j]->cross_point(my_laser_ray[I]);
		if (cross != NULL){
			s_num = j;		
            crossing=cross;
			break;
		}
	}
	if (s_num == -1){
                //find граница, куда дойдет луч
                float retx, rety;
                retx = my_laser_ray[I]->x + 2000 * cos(GradToRad(tdeg));
                rety = my_laser_ray[I]->y - 2000 * sin(GradToRad(tdeg)); 
                sprintf(buf_, "%f %f %f %f %c", my_laser_ray[I]->x, my_laser_ray[I]->y, retx, rety, '\0');
                if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1)
                {
            		perror("Can't send:");
            		return NULL;
        		}
		recv(cs, temp, 1, 0);

	}
	else{
		sprintf(buf_, "%f %f %f %f %c", tx, ty, crossing->x, crossing->y, '\0');
                if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1)
                {
            	perror("Can't send:");
            	return NULL;
        	}
        	recv(cs, temp, 1, 0);
	}
    }
    if(send(cs, "FINISH\0", 7, MSG_NOSIGNAL)==-1)
    {
            perror("Can't send:");
            return NULL;
        }
    while(recv(cs, buf_, sizeof(buf_)+1, 0)>0);
    	close(cs);
        return NULL;
        }

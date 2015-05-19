#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
//#include "ray.h"    //was included in "device.h"
#include <string.h>
#include <pthread.h>
#include "first.h"


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
	char buf_[32];

        vector <Device*> my_device;
	vector <SCREEN *> my_screen;
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
				Device  *d = new SphereRefl(x, y, r0, deg_1, deg_2);
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



 	point *cross = NULL;
	int k = 0; //номер девайса
	bool q = false; //true, если пересечения есть
    char temp[1];
    vector<RAY*>my_laser_ray;
    for(int i=0; i<my_laser.size(); i++)
    {
        my_laser_ray.push_back(my_laser[i]->rays_create());
    }
//RAY *my_laser_ray=my_laser->rays_create();
    for(int i=0; i<my_source.size(); i++)
    {
        RAY** rt=my_source[i]->rays_create();
        for(int j=0; j<NUMBER; j++)
        {
            my_laser_ray.push_back(rt[j]);
        }
    }
//let's work with laser first
for(int I=0; I<my_laser_ray.size(); I++)
{
    	
	while(1==1&&my_laser_ray[I]->TTL<50){
		int num = first(my_device, my_laser_ray[I]);
		if (num != -1){
			cross = my_device[num]->cross_point(my_laser_ray[I]);
                	sprintf(buf_, "%f %f %f %f %c", my_laser_ray[I]->x, my_laser_ray[I]->y, cross->x, cross->y, '\0');//new dot
                	if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1){
                    	perror("Can't send:");
                    	return NULL;
                	}
                	recv(cs, temp, 1, 0);
                	float tx=cross->x;
                	float ty=cross->y;
                	my_device[num]->change_direction(my_laser_ray[I], cross);

			my_laser_ray[I]->TTL++;
                	if(my_device[num]->getID()==4){
                    		sprintf(buf_, "%f %f %f %f %c", tx, ty, my_laser_ray[I]->x, my_laser_ray[I]->y, 0);
				my_laser_ray[I]->TTL++;
                                if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1){
                        		perror("Can't send:");
                        		return NULL;
                    		}
                    	recv(cs, temp, 1, 0);
                	}
			if (my_device[num]->getID()==5){
	                        cross = my_device[num]->cross_point(my_laser_ray[I]);
        	                sprintf(buf_, "%f %f %f %f %c", my_laser_ray[I]->x, my_laser_ray[I]->y, cross->x, cross->y, '\0');//new dot
                	        if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1){
                        		perror("Can't send:");
                        		return NULL;
                        	}
                        	recv(cs, temp, 1, 0);
                        	float tx=cross->x;
                        	float ty=cross->y;
                        	my_device[num]->change_direction(my_laser_ray[I], cross);
			}
		}
		else{
			break;
		}
	}
	//cross screen
 int s_num = first_s(my_screen, my_laser_ray[I]);
 if(my_laser_ray[I]->TTL<50)
 {
if (s_num == -1){
//find граница, куда дойдет луч
float retx, rety;
retx = my_laser_ray[I]->x + 2000 * cos(GradToRad(my_laser_ray[I]->deg));
rety = my_laser_ray[I]->y - 2000 * sin(GradToRad(my_laser_ray[I]->deg));
sprintf(buf_, "%f %f %f %f %c", my_laser_ray[I]->x, my_laser_ray[I]->y, retx, rety, '\0');
if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1)
{
perror("Can't send:");
return NULL;
}
recv(cs, temp, 1, 0);
}
else{
cross = my_screen[s_num]->cross_point(my_laser_ray[I]);
sprintf(buf_, "%f %f %f %f %c", my_laser_ray[I]->x, my_laser_ray[I]->y, cross->x, cross->y, '\0');
if(send(cs, buf_, strlen(buf_)+1, MSG_NOSIGNAL)==-1)
{
perror("Can't send:");
return NULL;
}
recv(cs, temp, 1, 0);
}
}
else
{
printf("RAY IS DIE AT %f %f\n", my_laser_ray[I]->x,my_laser_ray[I]->y);
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

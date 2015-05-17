#include <stdio.h>
#include "./device.h"

float min_(float x1, float x2){
	if (x1 < x2)
		return x1;
	else return x2;
}
void print_(vector <Device *> my_device){
        for (int i = 0; i < my_device.size(); i++){
                printf("%d:%f\n", i,min_(my_device[i]->x1, my_device[i]->x2));
        }
	printf("\n");
}
void sort_(vector <Device *> my_device){
//	print_(my_device);

	for (int i = 0; i < my_device.size()-1; i++){
		for (int j = i+1; j < my_device.size(); j++){
			if (min_(my_device[i]->x1, my_device[i]->x2) > min_(my_device[j]->x1, my_device[j]->x2)){
				Device *d = my_device[j];
				my_device[j] = my_device[i];
				my_device[i] = d;
			}
		}
	}	

//	print_(my_device);
}
/*int main(){
        vector <Device *> my_device;
        Device *d1 = new Lens(30, 0, 10, 5, 0);
        Device *d2 = new Lens(10, 0, 10, 10, 0);
        Device *d3 = new Lens(50, 0, 10, 20, 0);
        my_device.push_back(d1);
        my_device.push_back(d2);
        my_device.push_back(d3);

	sort_(my_device);
	return 0;
}*/

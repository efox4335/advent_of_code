//brute force for all offsets
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SEARCH_MIN -300
#define SEARCH_MAX 300

enum type{
	X_AND_Y,
	X_AND_Z,
	Y_AND_Z
};

typedef struct{
	long double start_x;
	long double start_y;
	long double start_z;

	long double x_vel;
	long double y_vel;
	long double z_vel;
}vec;

vec vec_array[1000];
int vec_num = 0;

int main(void)
{
	void do_vec(vec in_vec, long double , long double *);
	int does_match(long double *, long double *);
	void vec_cpy(vec *, vec *);
	int does_inter(vec *, vec *, long double *, long double *);
	void vec_mod(vec *, int, int, int);

	char *line = NULL;
	size_t lim = 0;
	char *token = NULL;
	vec mod_1;
	vec mod_2;
	long double pot_scale_1;
	long double pot_scale_2;
	long double new_cord[3];
	long double old_cord[3];

	while(getline(&line, &lim, stdin) > 1){
		token = strtok(line, " @,\n");
		vec_array[vec_num].start_x = atof(token);

		token = strtok(NULL, " @,\n");
		vec_array[vec_num].start_y = atof(token);

		token = strtok(NULL, " @,\n");
		vec_array[vec_num].start_z = atof(token);

		token = strtok(NULL, " @,\n");
		vec_array[vec_num].x_vel = atof(token);

		token = strtok(NULL, " @,\n");
		vec_array[vec_num].y_vel = atof(token);

		token = strtok(NULL, " @,\n");
		vec_array[vec_num].z_vel = atof(token);

		++vec_num;
	}

	//printf("%Lf %Lf %Lf\n", pot_x, pot_y, pot_z);

	//searches all modifications of vectors between search_min and search_max
	for(int i = SEARCH_MIN; i < SEARCH_MAX; ++i){
		for(int j = SEARCH_MIN; j < SEARCH_MAX; ++j){
			for(int k = SEARCH_MIN; k < SEARCH_MAX; ++k){
				vec_cpy(&mod_1, &vec_array[0]);
				vec_cpy(&mod_2, &vec_array[1]);

				vec_mod(&mod_1, i, j, k);
				vec_mod(&mod_2, i, j, k);

				//if 2 intersections are found checks the rest against the second hail
				if(does_inter(&mod_1, &mod_2, &pot_scale_1, &pot_scale_2)){
					do_vec(mod_2, pot_scale_2, old_cord);
					for(int h = 2; h < vec_num; ++h){
						vec_cpy(&mod_1, &vec_array[h]);

						vec_mod(&mod_1, i, j, k);

						does_inter(&mod_1, &mod_2, &pot_scale_1, &pot_scale_2);

						do_vec(mod_1, pot_scale_1, new_cord);

						if(!does_match(old_cord, new_cord)){
							break;
						}
						if(h == vec_num - 1){
							printf("starting point: %Lf %Lf %Lf\nangle thrown: %d %d %d\nsum: %Lf\n",
							new_cord[0], new_cord[1],
							new_cord[2], i, j, k,
							new_cord[0] + new_cord[1] + new_cord[2]);
							exit(0);
						}
					}
				}
			}
		}
	}

	return 0;
}

//get output
void do_vec(vec in_vec, long double sca, long double *cord_arr)
{
	cord_arr[0] = in_vec.start_x + (in_vec.x_vel * sca);
	cord_arr[1] = in_vec.start_y + (in_vec.y_vel * sca);
	cord_arr[2] = in_vec.start_z + (in_vec.z_vel * sca);
}

//https://www.youtube.com/watch?v=N-qUfr-rz_Y this algorithm
int does_inter(vec *vp1, vec *vp2, long double *scale_1, long double *scale_2)
{
	int get_scale(vec *, vec *, long double *, long double *, int);

	long double pot_scale_1;
	long double pot_scale_2;
	int is_good;

	if(fabs((vp2->x_vel * vp1->y_vel) - (vp2->y_vel * vp1->x_vel)) < 0.00000001){
		is_good = get_scale(vp1, vp2, &pot_scale_1, &pot_scale_2, X_AND_Z);
	}
	else
		is_good = get_scale(vp1, vp2, &pot_scale_1, &pot_scale_2, X_AND_Y);

	if(pot_scale_1 < 0 || pot_scale_2 < 0 || is_good != 1)
		return 0;

	*scale_1 = pot_scale_1;
	*scale_2 = pot_scale_2;
	return 1;
}


int get_scale(vec *vp1, vec *vp2, long double *sc_1, long double *sc_2, int mode)
{
	long double e1;
	long double e2;
	long double a1;
	long double a2;
	long double d1;
	long double d2;
	long double b1;
	long double b2;
	long double c1;
	long double c2;
	long double f1;
	long double f2;

	switch(mode){
		case X_AND_Y:
			e1 = vp1->y_vel;
			e2 = vp2->y_vel;
			a1 = vp1->start_x;
			a2 = vp2->start_x;
			d1 = vp1->x_vel;
			d2 = vp2->x_vel;
			b1 = vp1->start_y;
			b2 = vp2->start_y;
			c1 = vp1->start_z;
			c2 = vp2->start_z;
			f1 = vp1->z_vel;
			f2 = vp2->z_vel;

			*sc_1 = (((e2 * a1) - (e2 * a2) - (d2 * b1) + (d2 * b2)) / ((d2 * e1) - (e2 * d1)));

			break;

		case X_AND_Z:
			e1 = vp1->z_vel;
			e2 = vp2->z_vel;
			a1 = vp1->start_x;
			a2 = vp2->start_x;
			d1 = vp1->x_vel;
			d2 = vp2->x_vel;
			b1 = vp1->start_z;
			b2 = vp2->start_z;
			c1 = vp1->start_y;
			c2 = vp2->start_y;
			f1 = vp1->y_vel;
			f2 = vp2->y_vel;

			*sc_1 = (((e2 * a1) - (e2 * a2) - (d2 * b1) + (d2 * b2)) / ((d2 * e1) - (e2 * d1)));
			break;
		case Y_AND_Z:
			e1 = vp1->z_vel;
			e2 = vp2->z_vel;
			a1 = vp1->start_y;
			a2 = vp2->start_y;
			d1 = vp1->y_vel;
			d2 = vp2->y_vel;
			b1 = vp1->start_z;
			b2 = vp2->start_z;
			c1 = vp1->start_x;
			c2 = vp2->start_x;
			f1 = vp1->x_vel;
			f2 = vp2->x_vel;

			*sc_1 = (((e2 * a1) - (e2 * a2) - (d2 * b1) + (d2 * b2)) / ((d2 * e1) - (e2 * d1)));
			break;
	}

	*sc_2 = (a1 + (d1 * (*sc_1)) - a2) / d2;

	if(fabs(c1 - (c2 + (f2 * (*sc_2)) - (f1 * (*sc_1)))) < 1)
		return 1;
	return 0;
}

//mods vp by each val
void vec_mod(vec *vp, int x, int y, int z)
{
	vp->x_vel -= (long double) x;
	vp->y_vel -= (long double) y;
	vp->z_vel -= (long double) z;
}

//copies vp2 to vp1
void vec_cpy(vec *vp1, vec *vp2)
{
	vp1->start_x = vp2->start_x;
	vp1->start_y = vp2->start_y;
	vp1->start_z = vp2->start_z;
	vp1->x_vel = vp2->x_vel;
	vp1->y_vel = vp2->y_vel;
	vp1->z_vel = vp2->z_vel;
}

int does_match(long double *cord_1, long double *cord_2)
{
	for(int i = 0; i < 3; ++i)
		if(!(fabs(cord_1[i] - cord_2[i]) < 1)){
			printf("hi  %Lf %Lf\n", cord_1[i], cord_2[i]);
			return 0;
		}
	return 1;
}

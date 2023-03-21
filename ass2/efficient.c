#include <stdlib.h> // atoi, rand, qsort, malloc
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
// function declaration

// 직선(ax+by-c=0)과 주어진 점 p(x1, y1) 간의 거리
// distance = |ax1+by1-c| / sqrt(a^2 + b^2)
// 실제로는 sqrt는 계산하지 않음
// return value: 직선과 점 사이의 거리 (분모 제외)
float distance( float a, float b, float c, t_point p){
float d = abs(a*p.x+b*p.y-c);
		return d;
}
// 점들의 집합(points; 점의 수 num_point)에서 점 p1과 점 pn을 잇는 직선의 upper hull을 구하는 함수 (재귀호출)
// [output] lines: convex hull을 이루는 선들의 집합
// [output] num_line: 선의 수
// [output] capacity: lines에 할당된 메모리의 용량 (할당 가능한 선의 수)
// return value: 선들의 집합(lines)에 대한 포인터
//separate_points( points, num_point, points[0], points[num_point-1], s1, s2, &n1, &n2);
void separate_points( t_point *points, int num_point, t_point from, t_point to, t_point *s1, t_point *s2, int *n1, int *n2);
t_line *upper_hull( t_point *points, int num_point, t_point p1, t_point pn, t_line *lines, int *num_line, int *capacity){

	

	int q=0;

	

	float a= 0,b= 0,c=0;

	int n3=0, n4=0, n5=0, n6=0;

	float *maxdistance = (float *)malloc(num_point*sizeof(t_point));

	t_point *s3 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s3 != NULL);
	t_point *s4 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s4 != NULL);
	t_point *s5 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s5 != NULL);
	//t_point *s6 = (t_point *)malloc( sizeof(t_point) * num_point);
	//assert( s6 != NULL);

	
		if (num_point >0){

				a= pn.y-p1.y ;
				b= p1.x-pn.x ;
				c= (p1.x*pn.y) - (p1.y*pn.x);
				
				int maxIndex = 0;

				for (int i = 0; i < num_point; i++){

					maxdistance[i] = distance(a, b, c, points[i]);

								
									if (maxdistance[0] <= maxdistance[i]){			// 새로운 최대값 구하기
											// max에 새로운 최대값 저장
											maxIndex = i;
											maxdistance[0]=maxdistance[i];

									}


				}

				 separate_points( points, num_point, p1, points[maxIndex], s3, s4, &n3, &n4);	

				 upper_hull( s3, n3, p1, points[maxIndex], lines, num_line, capacity);
				 

				 separate_points( points, num_point, points[maxIndex], pn, s5, s4, &n5, &n4);

				 upper_hull( s5, n5, points[maxIndex], pn, lines, num_line, capacity);
				 

			
		}
	
		if (num_point == 0){

			q = (*num_line)++;

			


				lines[q].from = p1;
				lines[q].to = pn;

				q++;

				free(maxdistance);
				free(s3);
				free(s4);
				free(s5);
				//free(s6);
				
				
				

				

				 return lines;
		}
	
	

	




	

}


// float max_arr(float *distance, int size) {	// size개의실수 배열에서 최대값을 반환
// 	int max = distance[0];				// 최대값을 배열의 처음 수로 가정
// 	for (int i = 1; i < size; i++) {	// 두 번째 수부터 끝까지 반복
// 		if (max < distance[i])			// 새로운 최대값 구하기
// 			max = distance[i];		// max에 새로운 최대값 저장
// 	}
// 	return max;				// 최대값 반환
// }



// 두 점(from, to)을 연결하는 직선(ax + by - c = 0)으로 n개의 점들의 집합 s(점의 수 num_point)를 s1(점의 수 n1)과 s2(점의 수 n2)로 분리하는 함수
// [output] s1 : 직선의 upper(left)에 속한 점들의 집합 (ax+by-c < 0)
// [output] s2 : lower(right)에 속한 점들의 집합 (ax+by-c > 0)
// [output] n1 : s1 집합에 속한 점의 수
// [output] n2 : s2 집합에 속한 점의 수
void separate_points( t_point *points, int num_point, t_point from, t_point to, t_point *s1, t_point *s2, int *n1, int *n2){

			int a= 0,b= 0,c=0, num1=0, num2=0, num3= 0;
			a= to.y-from.y ;
	 		b =from.x-to.x ;
	  		c= (from.x*to.y) - (from.y*to.x) ;

		

			 for (int i = 0; i < num_point; i++){

				
				
				if (a*points[i].x+b*points[i].y-c <0){

					s1[num1] = points[i];
					num1++;
					
				}

			 	if (a*points[i].x+b*points[i].y-c >0){

			 		s2[num2] = points[i];
			 		num2++;					
			 	}

	
		
			 }

		*n1=num1;
		*n2=num2;


}

////////////////////////////////////////////////////////////////////////////////
void print_header(char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}

////////////////////////////////////////////////////////////////////////////////
void print_footer(void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
// qsort를 위한 비교 함수
int cmp_x( const void *p1, const void *p2)
{
	t_point *p = (t_point *)p1;
	t_point *q = (t_point *)p2;
	
	return p->x - q->x;
}

////////////////////////////////////////////////////////////////////////////////
void print_points( t_point *points, int num_point)
{
	int i;
	printf( "\n#points\n");
	
	for (i = 0; i < num_point; i++)
		printf( "points(%d,%d)\n", points[i].x, points[i].y);
}

////////////////////////////////////////////////////////////////////////////////
void print_line_segments( t_line *lines, int num_line)
{
	int i;

	printf( "\n#line segments\n");
	
	for (i = 0; i < num_line; i++)
		printf( "segments(%d,%d,%d,%d)\n", lines[i].from.x, lines[i].from.y, lines[i].to.x, lines[i].to.y);
}

////////////////////////////////////////////////////////////////////////////////
// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of lines
// return value : pointer of set of line segments that forms the convex hull
t_line *convex_hull( t_point *points, int num_point, int *num_line)
{
	int capacity = 10;

	t_line *lines = (t_line *) malloc( capacity * sizeof(t_line));
	*num_line = 0;

	// s1: set of points
	t_point *s1 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s1 != NULL);

	// s2: set of points
	t_point *s2 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s2 != NULL);

	int n1, n2; // number of points in s1, s2, respectively

	// x 좌표에 따라 정렬된 점들의 집합이 입력된 경우
	// points[0] : leftmost point (p1)
	// points[num_point-1] : rightmost point (pn)
	
	// // 점들을 분리
	  separate_points( points, num_point, points[0], points[num_point-1], s1, s2, &n1, &n2);

	// upper hull을 구한다.

	



	lines = (t_line *) realloc( lines, (capacity+n1*(n1-1)/2) * sizeof(t_line));
	 
			lines = upper_hull( s1, n1, points[0], points[num_point-1], lines, num_line, &capacity);

		

	 
	lines = (t_line *) realloc( lines, (capacity+n2*(n2-1)/2) * sizeof(t_line));
	
	lines = upper_hull( s2, n2, points[num_point-1], points[0], lines, num_line, &capacity);

	

	
	
	free( s1);
	free( s2);



	return lines;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	float x, y;
	int num_point=0; // number of points
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}



	num_point = atoi( argv[1]);
	
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points;
	points = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( points != NULL);
	
	// making points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
	
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);
	
	// sort the points by their x coordinate
	qsort( points, num_point, sizeof(t_point), cmp_x);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	// convex hull algorithm
	int num_line;
	t_line *lines = convex_hull( points, num_point, &num_line);
	
	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}




#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF	0x04
#define GOAT	0x02
#define CABBAGE	0x01

//!! 마지막 프린트가 1001로 나와야 하는데 1111로 나옴. 그외 작동 정상.
// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc( int state, int *p, int *w, int *g, int *c){
	*p = ((state & PEASANT) == PEASANT);
	*w = ((state & WOLF) == WOLF);
	*g = ((state & GOAT) == GOAT);
	*c = ((state & CABBAGE) == CABBAGE);
	
	return;
}

// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
// Goal-state found! 밑에 <0000> 쭉 쓰인 것
static void print_statename( FILE *fp, int state){
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	
	fprintf(fp, "<%d%d%d%d>", p, w, g, c);
}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end( int state){
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	int w_eats_g = p ^ w &&  p ^ g;
	int g_eats_c = p ^ g  && p ^ c;
	
	return w_eats_g || g_eats_c;
	
}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition( int state1,	int state2){
	
	// 상태검사
	if((is_dead_end(state2)) || is_dead_end(state1)) 
		return 0;
	
	int p1, w1, g1, c1, p2, w2, g2, c2;
	get_pwgc(state1, &p1, &w1, &g1, &c1);
	get_pwgc(state2, &p2, &w2, &g2, &c2);
	
	// 농부가 이동 가능한지 state1의 p != state2의 p
	if(p1 == p2) 
		return 0; 
	
	//농부와 아이템이 이동 가능한지
	int i, j, k , cnt;
	i = w1 ^ w2;
	j = g1 ^ g2;
	k = c1 ^ c2;

	if((i && p2 ^ w2) ||  (j && p2 ^ g2) || (k && p2 ^ c2))
		return 0;
	
	// 이동한 아이템 개수
	cnt = i + j + k;
	if(cnt >= 2) 
		return 0;
	
	return 1;
}

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP( int state){
	state ^= PEASANT;
	return state;
}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW( int state){
	int tmp = state ^ (PEASANT +WOLF);
	if( (!(is_dead_end(tmp))) && (is_possible_transition(state, tmp))) {
		return tmp;
	} 
	else return -1;
}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG( int state){
	int tmp = state ^ (PEASANT + GOAT);
	if( (!(is_dead_end(tmp))) && (is_possible_transition(state, tmp))) {
		return tmp;
	}
	else return -1;
}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC( int state){
	int tmp = state ^ (PEASANT + CABBAGE);
	if( (!(is_dead_end(tmp))) && (is_possible_transition(state, tmp))) {
		return tmp;

	}
	else return -1;
}

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited( int visited[], int depth, int state) {
	int i;
	for(i =0 ; i< depth ; i++){
		if(visited[i] == state) {
			return 1;
		}
	}
	return 0;
}

// 방문한 경로(상태들)을 차례로 화면에 출력
static void print_path( int visited[], int depth){
	int i;
	for(i = 0; i <= depth; i++){
		print_statename(stdout, visited[i]);
		printf("\n");
	}
	printf("\n");
}

// recursive function
static void dfs_main( int initial_state, int goal_state, int depth, int visited[]){
	
	printf("current state is ");
	print_statename(stdout, initial_state);
	printf(" (depth %d)\n", depth);
	visited[depth] = initial_state;
	
	if(initial_state == goal_state){
		printf("Goal-state found!\n");
		print_path(visited, depth);
		
		return;
	}
	else {
		int cp, cpw, cpg, cpc;
		cp = changeP(initial_state);
		cpw = changePW(initial_state);
		cpg = changePG(initial_state);
		cpc = changePC(initial_state);
		
		//P
		if (is_dead_end(cp)) {
			printf("\tnext state ");
			print_statename(stdout, cp);
			printf(" is dead-end\n");
		}
		else if (is_visited(visited, depth, cp)) {
			printf("\tnext state ");
			print_statename(stdout, cp);
			printf(" has been visited\n");
		}
		else {
			dfs_main(cp, goal_state, depth + 1, visited);
			printf("back to ");
			print_statename(stdout, initial_state);
			printf(" (depth %d)\n", depth);
		}
		//PW , PG, PC
		int change_with_item[3] = {cpw, cpg, cpc};
		int changed_item[3] = {WOLF, GOAT, CABBAGE};
		for(int i = 0; i< 3; i++){
			if((initial_state & PEASANT) >> 3 == (initial_state & changed_item[i]) >> 2-i){
				if(change_with_item[i] == -1){
					printf("\tnext state ");
					print_statename(stdout, change_with_item[i]);
					printf(" is dead-end\n");
				}
				else{
					if (is_visited(visited, depth, change_with_item[i])){
						printf("\tnext state ");
						print_statename(stdout, change_with_item[i]);
						printf(" has been visited\n");
					}
					else {
						dfs_main(change_with_item[i], goal_state, depth + 1, visited);
						printf("back to ");
						print_statename(stdout, initial_state);
						printf(" (depth %d)\n", depth);
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
// 방문한 상태 인접 행렬 요소에 1 변경
void make_adjacency_matrix( int graph[][16]){
	int i, j;
	for(i = 0; i< 16 ; i++){
		for(j =0; j< 16; j++){
			if(!is_dead_end(i)){
				graph[i][j] = is_possible_transition(i, j);
				graph[j][i] = graph[i][j];
			}
				
		}
	}		
}

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph( int graph[][16], int num){
	int i, j;
	for(i = 0 ; i< num ; i++){
		for(j =0 ; j< 16 ; j++){
			printf("%d\t", graph[i][j]);
		}
		printf("\n");
	}
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
// *Vertices 16 1~16개의 state
//*Edges 1(start) 11(end)
void save_graph( char *filename, int graph[][16], int num){
	FILE * fp = fopen(filename, "wt");
	
	fprintf(fp, "*Vertices %d\n", num);
	for(int i = 0; i< num ; i++){
		fprintf(fp, "%3d \"", i+1);
		print_statename(fp, i);
		fprintf(fp, "\"\n");
	}
	
	fprintf(fp, "*Edges\n");
	for(int i = 0; i< num; i++){
		for(int j = i+1; j<num ; j++){
			if(graph[i][j])
				fprintf(fp, "%3d%3d\n", i + 1, j + 1);
		}
	}
	fclose(fp);
}


////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int initial_state, int goal_state)
{
	int depth = 0;
	int visited[16] = {0,}; // 방문한 정점을 저장
	
	dfs_main( initial_state, goal_state, depth, visited); 
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int graph[16][16] = {0,};
	
	// 인접 행렬 만들기
	make_adjacency_matrix( graph);

	// 인접 행렬 출력 (only for debugging)
	//print_graph( graph, 16);
	
	// .net 파일 만들기
	save_graph( "pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search( 0, 15); // initial state, goal state
	
	return 0;
}

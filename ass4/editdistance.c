#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01
#define DELETE_OP      0x02
#define SUBSTITUTE_OP  0x04
#define MATCH_OP       0x08
#define TRANSPOSE_OP   0x10

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태의 연산자 정보가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
static void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8]);

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를 다음과 같은 기호로 표시한다. 삽입:I, 삭제:D, 교체:S, 일치:M, 전위:T
void print_matrix( int *op_matrix, int col_size, char *str1, char *str2, int n, int m);

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2);

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력
void print_alignment( char align_str[][8], int level)
{
	int i;
	
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str);
	
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	fprintf( stderr, "INSERT_COST = %d\n", INSERT_COST);
	fprintf( stderr, "DELETE_COST = %d\n", DELETE_COST);
	fprintf( stderr, "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	fprintf( stderr, "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}

static void backtrace_main(int* op_matrix, int col_size, char* str1, char* str2, int n, int m, int level, char align_str[][8]) {
	static int count;
	if (!level) count = 1;
	if (!n && !m ) {
		printf("\n[%d] ==============================\n",count++);
		print_alignment(align_str, level - 1);
		return;
	}
	int oper = op_matrix[col_size * n + m]; 

	// 일치
	if (oper & MATCH_OP ) {
		strcpy(align_str[level], "a - a");
		align_str[level][0] = str1[n-1];
		align_str[level][4] = str1[n-1];
		backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str);
	}
	// 교체
	if(oper & SUBSTITUTE_OP) {
		strcpy(align_str[level], "a - b"); 
		align_str[level][0] = str1[n - 1];
		align_str[level][4] = str2[m - 1];
		backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str);
	}
	
	//삽입
	if (oper & INSERT_OP) {
		strcpy(align_str[level], "* - a");
		align_str[level][4] = str2[m - 1];
		backtrace_main(op_matrix, col_size, str1, str2, n, m - 1, level + 1, align_str);
	}
	
	//삭제
	if (oper & DELETE_OP) {
		strcpy(align_str[level], "a - *");
		align_str[level][0] = str1[n-1];
		backtrace_main(op_matrix, col_size, str1, str2, n - 1, m, level + 1, align_str);
	}
	
	//전위
	if (oper & TRANSPOSE_OP) {
		strcpy(align_str[level], "ab - ba");
		align_str[level][0] = align_str[level][6] = str1[n - 2];
		align_str[level][1] = align_str[level][5] = str1[n - 1];
		backtrace_main(op_matrix, col_size, str1, str2, n - 2, m - 2, level + 1, align_str);
	}
	
}

// op_matrix 출력 : (좌하단(1,1) -> 우상단(n, m))
void print_matrix( int *op_matrix, int col_size, char *str1, char *str2, int n, int m) {
	int oper;
	char op[5];
	for (int i = n; i >= 1; --i) {
		 printf("%c\t", str1[i-1]);
		for (int j = 1; j <= m; ++j) {
			oper = op_matrix[col_size * i + j];
			op[0] = 0;
			if (oper & MATCH_OP) strcat(op, "M");
			if (oper & SUBSTITUTE_OP) strcat(op, "S");
			if (oper & INSERT_OP) strcat(op, "I");
			if (oper & DELETE_OP) strcat(op, "D");
			if (oper & TRANSPOSE_OP) strcat(op, "T");
			printf("%s\t", op);
		}
		printf("\n");
	}
	for (int i = 1; i <= m; i++) {
		printf("\t%c", str2[i - 1]);
	}
	printf("\n");
}


int min_editdistance(char * str1, char* str2){
	int n = strlen(str1);
	int m = strlen(str2);
	int d[n+1][m+1];
	int op_matrix[(n+1)*(m+1)]; 
	
	memset(op_matrix, 0, sizeof(op_matrix));

	// 가장 왼쪽 열 삭제연산으로 초기화
	for(int i=0; i<=n; ++i){
		d[i][0] = i;
		if (i) op_matrix[i * (m + 1)] = DELETE_OP;
	}
	// 가장 아래 행 삽입연산으로 초기화
	for (int j = 0; j <= m; ++j) {
		d[0][j] = j;
		if (j) op_matrix[j]=INSERT_OP;
	}
	
	int cost;
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			// 일치 또는 교체에 따라 거리값 할당
			cost = str1[i-1] == str2[j-1] ? 0 : SUBSTITUTE_COST;
			//전위 가능
			if ((i > 1 && j > 1) && (str1[i - 2] == str2[j-1] && str1[i-1] == str2[j - 2])) {
				d[i][j] = __GetMin4(d[i - 1][j] + DELETE_COST, d[i][j - 1] + INSERT_COST, d[i - 1][j - 1] + cost, d[i - 2][j - 2] + TRANSPOSE_COST);
				
				if (d[i][j] == d[i - 1][j - 1] + cost) op_matrix[i * (m + 1) + j] +=  cost ? SUBSTITUTE_OP : MATCH_OP;
				if (d[i][j] == d[i][j - 1] + INSERT_COST) op_matrix[i * (m + 1) + j] += INSERT_OP;
				if (d[i][j] == d[i - 1][j] + DELETE_COST) op_matrix[i*(m+1)+j] += DELETE_OP;	
				if (d[i][j] == d[i - 2][j - 2] + TRANSPOSE_COST) op_matrix[i * (m + 1) + j] += TRANSPOSE_OP;
			} 
			//전위 불가
			else { 
				d[i][j] = __GetMin3(d[i - 1][j] + DELETE_COST, d[i][j - 1] + INSERT_COST, d[i - 1][j - 1] + cost);
				
				if (d[i][j] == d[i - 1][j - 1] + cost) op_matrix[i * (m + 1) + j] += cost ? SUBSTITUTE_OP : MATCH_OP;
				if (d[i][j] == d[i][j - 1] + INSERT_COST) op_matrix[i * (m + 1) + j] += INSERT_OP;
				if (d[i][j] == d[i - 1][j] + DELETE_COST) op_matrix[i * (m + 1) + j] += DELETE_OP;

			}
		}
	}
	print_matrix(op_matrix, m + 1, str1, str2, n, m);
	backtrace(op_matrix, m+1, str1, str2, n, m);
	return d[n][m];
}
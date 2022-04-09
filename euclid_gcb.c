#include <stdio.h>
int gcd_cal(int m, int n)
{
	if(n==0) return m;
	gcd_cal(n, m%n);
}

int main()
{
	int i1, i2;
	int m, n, gcd;
	printf("서로 다른 두 자연수를 입력하세요.");
	scanf("%d %d", &i1, &i2);
	if (i1>i2) {
		m = i1; n= i2;	
	}
	else if (i1<i2) {
		m = i1; n=i2;
	}
	else gcd = i1;
	gcd = gcd_cal(m, n);
	printf("%d와 %d의 최대공약수 :%d\n", i1, i2, gcd);
}
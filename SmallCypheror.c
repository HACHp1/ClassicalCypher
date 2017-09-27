#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>//用到绝对值函数
#define MAXLENGTH 100

//其中，欧几里得求逆元的算法参考了网上来源

int ExtendedEuclid(long long int f, long long int d);
void zhalan_encrypt(char*b, const char*a);
void zhalan_decrypt(char*b, const char*a);
void caesar_encrypt(char*b, const char*a, int key);
void RSAencrypt(char* str2, const char* str, int n, int e);
void RSAdecrypt(char*str2, const char*str, int n, int d);

int main()
{
	int caesar_key;
	int rsa_p, rsa_q, rsa_e, rsa_d;
	int temp;//转换RSA的公钥和密钥
	int global_i,global_len;

	char original[MAXLENGTH];
	char encrypted[MAXLENGTH];


	printf("输入需要加密的字符串:\n");
	gets_s(original, MAXLENGTH);
	zhalan_encrypt(encrypted, original);
	zhalan_decrypt(original, encrypted);
	printf("栅栏加密：%s\n栅栏解密：%s\n", encrypted, original);
	printf("输入凯撒加密密钥：\n");
	fflush(stdin);
	scanf_s("%d", &caesar_key, 1);
	if (caesar_key < 0){
		caesar_key += (abs(caesar_key / 26) + 1) * 26;
	}//对密钥取正
	caesar_key %= 26;//经过DEBUG，发现需要先对密钥取模使之在有限域内；
	//printf("%d\n", caesar_key);
	caesar_encrypt(encrypted, original, caesar_key);
	caesar_encrypt(original, encrypted, -caesar_key + 26);//对负数先转变为正值
	printf("凯撒加密：%s\n凯撒解密：%s\n", encrypted, original);
	printf("实验部分(RSA加密)：\n");
	//printf("请输入两个质数，中间用一个空格分开:\n");
	//fflush(stdin);
	//scanf_s("%d %d", &rsa_p, &rsa_q);
	rsa_p = 113;
	rsa_q = 107;
	printf("请输入加密公钥（与112*106互素的正整数，暴露了：））：\n");
	fflush(stdin);
	scanf_s("%d", &rsa_e, 1);
	int rsa_n = rsa_p*rsa_q;
	int rsa_fi = (rsa_p - 1)*(rsa_q - 1);
	temp = ExtendedEuclid(rsa_e, rsa_fi);
	if (temp < 0){
		temp += (abs(temp / rsa_fi) + 1) * rsa_fi;
	}//对密钥取正
	rsa_d = temp;
	//printf("\n$test:%d %d\n", rsa_d, rsa_e);
	RSAencrypt(encrypted, original, rsa_n, rsa_e);
	RSAdecrypt(original, encrypted, rsa_n, rsa_d);
	global_len = strlen(original);
	printf("RSA加密：\n");
	for (global_i = 0; global_i <= global_len - 1; global_i++){
		printf("%llX ", original[global_i]);
	}
	printf("\n");
	printf("RSA解密：%s\n",  original);
	fflush(stdin);
	getchar();
	return 0;
}

void zhalan_encrypt(char*b, const char*a){
	char temp[MAXLENGTH];
	int i = 0, j = 0, k = 0, len;
	len = strlen(a);
	while (i <= len - 1){
		if (i % 2){
			temp[k] = a[i];
			i++;
			k++;
			temp[k] = '\0';//每次在赋值之后先向后一位填入终止符
		}
		else{
			b[j] = a[i];
			i++;
			j++;
			b[j] = '\0';//每次在赋值之后先向后一位填入终止符
		}
	}
	strcat(b, temp);
}

void zhalan_decrypt(char*b, const char*a){
	int len = strlen(a);
	int i = 0, j = 0;
	if (len % 2 == 0){//奇偶要分类
		while (i <= len - 1){
			if (i % 2){
				b[i] = a[len / 2 - 1 + j];
				i++;
			}
			else{
				b[i] = a[j];
				i++;
				j++;
			}
		}
	}
	else
		while (i <= len - 1){
			if (i % 2){
				b[i] = a[len / 2 + j];
				i++;
			}
			else{
				b[i] = a[j];
				i++;
				j++;
			}
		}
	b[i] = '\0';
}

void caesar_encrypt(char*b, const char*a, int key){
	int i, len;
	i = 0;
	len = strlen(a);
	while (i <= len - 1){
		if (a[i] > 'Z')
			b[i] = (a[i] + key - 'a') % 26 + 'a';
		else{
			b[i] = (a[i] + key - 'A') % 26 + 'A';
		}
		i++;
	}
}

void RSAencrypt(char* str2, const char* str, int n, int e){
	int len, i, j;
	len = strlen(str);
	for (i = 0; i <= len - 1; i++){
		str2[i] = str[i];
		for (j = 0; j <= e - 2; j++){
			str2[i] = (str2[i] * str2[i]) % n;
		}
	}
}


void RSAdecrypt(char*str2, const char*str, int n, int d){
	int len, i, j;
	len = strlen(str);
	for (i = 0; i <= len - 1; i++){
		str2[i] = str[i];
		for (j = 0; j <= d - 2; j++){
			str2[i] = (str2[i] * str2[i]) % n;
		}
	}
}

int ExtendedEuclid(long long int f, long long int d){
	long long int x1, x2, x3, y1, y2, y3, t1, t2, t3, q;
	x1 = y2 = 1;
	x2 = y1 = 0;
	x3 = (f >= d) ? f : d;
	y3 = (f >= d) ? d : f;
	while (1)
	{
		if (y3 == 1)
		{
			return y2;
		}
		q = x3 / y3;
		t1 = x1 - q*y1;
		t2 = x2 - q*y2;
		t3 = x3 - q*y3;
		x1 = y1;
		x2 = y2;
		x3 = y3;
		y1 = t1;
		y2 = t2;
		y3 = t3;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
void padding4byte(char n1[]);
void checksum(int n2[],char n4[]);

int main(void) {
	
	char info[SIZE]="";
	char n1[SIZE]=""; //공백, /없는 문자열
	int n2[SIZE];//문자열이 저장된 정수형배열 & 16진수로 변환된 문자열을 정수로 변환한 것이 저장됨
	char n3[10]="";//n2를 16진수로 변환하는 과정에 쓰는 문자열
	char n4[SIZE]="";//16진수로 변환된 문자열이 저장됨
	
	char *token = NULL;
	char delimiter[10] = " /";
	printf("입력 : 영문 이름과 주소(예 : HONG KILDONG/YONGIN) ");
	gets(info);
	token = strtok(info, delimiter); // 입력받은 이름과 주소에서 빈칸, / 제거
	while (token != NULL)//더이상 분리할 것이 없을때까지
	{
		strcat(n1, token);
		token = strtok(NULL, delimiter);
	}

	padding4byte(n1);//n행 4열로 출력
	
	for (int i = 0;i < strlen(n1);i++) {
		n2[i] = n1[i]; //문자열 한글자씩 int형 변수에 저장
		sprintf(n3, "%X", n2[i]); //한 글자씩 16진수로 변환 후 문자열로 저장 (정수->문자)
		strcat(n4, n3);//n4에 연결
	}
	for (int i = 0; i < strlen(n1) * 2;i++) { //16진수로 바꿨기때문에 길이 2배
		if (n4[i] >= 'A')//문자일경우 
			n2[i] = n4[i] - '7';//sprintf가 16진수를 대문자로 표현했기 때문에 '0'말고 '7'빼줘야함. (예 : 문자 A -> 10진수 = 65, 16진수 A -> 10진수 = 10 ==> 65-10=55 = '7')
		
		else
			n2[i] = n4[i] - '0'; //한 글자씩 정수로 변환해 n2에 저장 //예 : HONG -> 4,8,4,15,4,14,4,7
	}

	
	//checksum 계산
	checksum(n2,n4);

	return 0;
}

void padding4byte(char n1[]) {
	char(*str)[4]; //2차원 배열 동적메모리 할당
	int len = 0; //행 길이
	int z = 0, j = 0, x = 0;

	if (strlen(n1) % 4 != 0) // n*4행렬에 빈칸이 있는 경우
		len = strlen(n1) / 4 + 1;
	
	else //n*4행렬에 빈칸이 없는 경우
		len = strlen(n1) / 4;

	str = (char(*)[4])malloc(sizeof(char) * 4 * len); //len행 4열 배열
	if (str == NULL)
		printf("동적 할당 실패");

	for (int i = 0;i < len;i++) {
		if (i == len - 1) { //마지막 줄 채울때
			if (strlen(n1) % 4 != 0) {//n*4행렬을 다 채우지 못하는 경우
				j = strlen(n1) % 4;//마지막 문자의 열
				while (x < j) {//0부터 마지막 문자가 있는 열까지 
					str[i][x] = n1[(len - 1) * 4 + x]; //str에 저장
					x++;
				}
				while (j < 4) { //마지막 문자 다음 열부터 0으로 padding 
					str[i][j] = '0';
					j++;
				}
				break;
			}
		}
		for (int j = 0;j < 4;j++) {
			str[i][j] = n1[z];
			z++;
		}
	}
	printf("\n 4바이트로 Parsing ( 0 : Padding )\n\n ---------------\n");
	for (int i = 0;i < len;i++) {
		printf("|");
		for (j = 0;j < 4;j++)
			printf(" %c |", str[i][j]);
		printf("\n ---------------\n");
	}
	printf("\n\n");
	free(str);
}


void checksum(int n2[],char n4[]) {
	int(*str)[8]; //2차원 배열 동적메모리 할당
	int len = 0; // 행 길이
	int z = 0, j = 0, x = 0;
	int n[8] = { 0 }; //checksum계산용 문자열
	char n5[8] = ""; //검산때 사용할 백업용 문자열
	if (strlen(n4) % 8 != 0) {// n*8행렬에 빈칸이 있는 경우 //n2와 n4의 길이가 같으므로 편의를 위해 strlen(n4)사용
		len = strlen(n4) / 8 + 1;
	}
	else //n*8행렬에 빈칸이 없는 경우
		len = strlen(n4) / 8;

	str = (int(*)[8])malloc(sizeof(int) * 8 * (len + 2)); //len+2행 8열 배열 //checksum계산 결과 들어갈 배열 행(len, len+1) 추가 
	if (str == NULL)
		printf("동적 할당 실패");

	for (int i = 0;i < len;i++) {
		if (i == len - 1) { //마지막 줄 채울때
			if (strlen(n4) % 8 != 0) {//n*8행렬을 다 채우지 못하는 경우
				j = strlen(n4) % 8;//마지막 문자의 열
				while (x < j) {//0부터 마지막 문자가 있는 열까지 
					str[i][x] = n4[(len - 1) * 8 + x]; //str에 저장
					x++;
				}
				while (j < 8) { //마지막 문자 다음 열부터 0으로 padding
					str[i][j] = '0';
					j++;
				}
				break;
			}
		}
		for (int j = 0;j < 8;j++) {
			str[i][j] = n4[z];
			z++;
		}
	}

	//checksum 계산
	for (int i = 7;i >= 0;i--) { //열
		n[i] = 0;
		for (j = 0;j < len;j++) { //행
			if (str[j][i] >= 65) //A이상 일 때
				str[j][i] -= 55;//대문자를 정수형으로 표현했기 때문에 '0'말고 '7'값(정수이기때문에 55)빼줘야함.
		
			else 
				str[j][i] -= 48; //16진수를 그대로 10진수로 바꾸었기때문에 '0'(=48)값만큼 빼줌.

			n[i] += str[j][i];
		}
	}
	//올림 수 계산
	for (int i = 7;i >= 0;i--) {
		if (i == 0) {
			break;
		}
		n[i - 1] += (n[i] / 16); //올림수 더해주기
	}

	for (int i = 0;i < 8;i++) {
		str[len][i] = n[i] % 16; // 16으로 나눈 나머지 값 = 캐리 더하기 전 checksum 값 -> len행에 저장
		str[len + 1][i] = n[i] / 16;// 16으로 나눈 값 = 캐리 -> len+1행에 저장
	}
	

	printf("\n 16진수로 변환\n\n -------------------------------\n");
	for (int i = 0;i < len + 2;i++) {
		printf("|");
		for (j = 0;j < 8;j++) 
			printf(" %X |", str[i][j]);
		printf("\n -------------------------------\n");	
	}

	//SUM
	printf("\n SUM\n\n");
	//printf(" ");
	for (int i = 0;i < 8;i++)
		printf(" %X", str[len][i]);

	printf(" + %d(Carry) = ", str[len + 1][0]);
	str[len][7] += str[len+1][0];//마지막 자리에 캐리 더하기.
	if (str[len][7] > 15) { //F( = 15) 보다 커졌을 경우
		for (int i = 7;i >= 0;i--) {
			if (i == 0)
				break;
			str[len][i - 1] += (str[len][i] / 16);//캐리 더해주기.
			str[len][i] %= 15; 
		} 
	}
	for (int i = 0;i < 8;i++) {
		printf("%X ", str[len][i]);
		n5[i] = str[len][i]; //백업 (검산할 때 사용하기 위해)
	}
	printf("\n\n");

	printf("\n Checksum(1's Complement)\n\n");
	printf(" ~( ");
	for (int i = 0;i < 8;i++)
		printf("%X ", str[len][i]);
	printf(") = ");
	for (int i = 0;i < 8;i++) {
		str[len][i] -= 15;// 16진수의 1의보수는 곧 F(=15)를 빼주는 것과 같으므로 각 수에 15를 빼준다.
		if (str[len][i] < 0)
			str[len][i] = -str[len][i];//0이하일경우 절댓값 저장.

		printf("%X ", str[len][i]);
	}
	printf("\n\n");
	
	//검산
	for (int i = 0;i < 8;i++) {
		str[len + 1][i] = str[len][i] + n5[i];
	}
	printf("\n Verification\n\n -------------------------------\n");
	for (int i = 0;i < len + 2;i++) {
		printf("|");
		for (j = 0;j < 8;j++)
			printf(" %X |", str[i][j]);
		printf("\n -------------------------------\n");
	}
	free(str);
}

/*
Copyright (c) 2011, Cairo Caplan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Regras:
 *	1 - Examine o próximo elemento na entrada
	1.5 - Se for " ", ignore e volte pra 1
	2 - Se for um operando, copie-o na saída
	3 - Se for um "(", coloque-o no topo da pilha
	4 - Se for um operador, então:
		- Se o topo da pilha for um abre parênteses, empilhe o operador
		- Se esse operador tiver maior precedência
		que o operador do topo da pilha então empilhe o operador
		- Senão, enquanto o operador do topo da pilha tiver maior precedência, 
		retire a operação do topo da pilha, copiando-a na saída e coloque na saída a operação atual
	5 - Se for um ")", retire os operadores do topo da pilha, copiando-os na saída
	até que seja encontrado um "(". Retire o "(" e  descarte-o
	6 - Se ainda houver dados na entrada, volte ao passo 1
	7 - Se não houver dados na entrada desempilhe os operandos restantes
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char spilha[16][5];
short int topo=0;	

void s_empilha(char operador[5]){//string[10]
	short int i;
	for (i=0;i<5;i++){
		spilha[topo][i]=operador[i];
	}
	topo++;
}

void s_desempilha(char operador[5]){
	short int i;
	topo--;
	for (i=0;i<5;i++){
		operador[i]=spilha[topo][i];
	}
}

int precedencia(char operador[])
{

	if((operador[0]== '(')||(operador[0]== ')'))
		return 4;
	if(operador[0]=='+' || operador[0]=='-')
		return 3;
	if(operador[0]=='*' || operador[0]=='/')
		return 2;
	if(operador[0]=='^')
		return 1;
	return 0;
}

//char v[100][10]//[linha][caracter]

short int interpreta(char *entrada,char v[100][10]){
	short int saidaind=0;
	//char entrada[100];
	//le a linha para "entrada"
	//fgets(entrada,100,stdin);
	//printf("entrada = %s\n",entrada);
	short int i=0,snind=0,tipo_numero=1,espera_num_negativo=1;
	char stemp[5];
	
	//printf("tamanho da entrada = %d\n",tam_entrada);
	for(i=0;entrada[i]!='\0';i++){
		if ( ( (entrada[i]>='0')&&(entrada[i]<='9') )||(entrada[i]=='.')||(entrada[i]==',')
			||(entrada[i]=='-'&&(i==0||espera_num_negativo==1))){//
			tipo_numero=1;
			if (entrada[i]==','){
				v[saidaind][snind]='.';
			}
			else{			
				v[saidaind][snind]=entrada[i];
			}
			snind++;
			v[saidaind][snind]='\0';
			espera_num_negativo=0;
		}
		else{
			snind=0;
			stemp[0]=entrada[i];
			stemp[1]='\0';
			//if(entrada[i]==' '){ }
			if (tipo_numero==1){
				//printf("entrou = %s\n",v[saidaind]);
				tipo_numero=0;
				saidaind++;
			}
			if (entrada[i]=='('){
				s_empilha(stemp);
				//printf("empilhou %s\n",stemp);
				espera_num_negativo=1;
			}
			if (entrada[i]==')'){
				while ((strcmp(spilha[topo-1],"("))!=0){
					s_desempilha(v[saidaind]);
					saidaind++;
				}
				topo--;
			}
			if ((entrada[i]=='+')||(entrada[i]=='-')||(entrada[i]=='/')||(entrada[i]=='*')
				||(entrada[i]=='^')){
				if ((topo==0)||(precedencia(stemp)<precedencia(spilha[topo-1]))){
					s_empilha(stemp);
					//printf("empilhou %s\n",stemp);
				}
				else{
					while(precedencia(spilha[topo-1])<=precedencia(stemp)&&(topo>0)){
						s_desempilha(v[saidaind]);
					//	printf("desempilhou %s\n",v[saidaind]);
						saidaind++;
					}
					s_empilha(stemp);
					//printf("empilhou %s\n",stemp);
				}
			}
			else{
				saidaind--;
				//teste para verificar se temos uma variável t ou uma funcao tangente
				if(entrada[i]=='t'||entrada[i]=='T'){
					
					if  ((entrada[i+1]=='a'||entrada[i+1]=='A')&&(entrada[i+2]=='n'||entrada[i+2]=='N')){
						i+=2;
						s_empilha("tan");
					}
					else{
						s_empilha("t");
					}
					//saidaind++;
				}
				if(entrada[i]=='s'||entrada[i]=='S'){
					if  ((entrada[i+1]=='i'||entrada[i+1]=='I'||entrada[i+1]=='e'||entrada[i+1]=='E')
						&&(entrada[i+2]=='n'||entrada[i+2]=='N')){
						i+=2;
						s_empilha("sin");
					}
				
				}
				if(entrada[i]=='c'||entrada[i]=='C'){
					if  ((entrada[i+1]=='o'||entrada[i+1]=='O')&&(entrada[i+2]=='s'||entrada[i+2]=='S')){
						i+=2;
						s_empilha("cos");
					}
				}
				if (entrada[i]=='u'){
					s_empilha("u");
				}
				saidaind++;
			}
		}
	}
	while(topo>0){
		if (tipo_numero==1){
			saidaind++;
			tipo_numero=0;
		}
		s_desempilha(v[saidaind]);
		//printf("desempilhou %s\n",v[saidaind]);
		//v[saidaind][1]='\0';
		saidaind++;
	}
	saidaind--;
	printf("entrada = %s",entrada);
	printf("entrada posfixa = ");
	for (i=0;i<=saidaind;i++){
		printf("%s|",v[i]);
	}
	printf("\n");
	return (saidaind);
}


double expilha[16];
short int ex_topo=0;

void ex_empilha(double valor){
	expilha[ex_topo]=valor;
	ex_topo++;
}

double ex_desempilha(){
	ex_topo--;
	return(expilha[ex_topo]);
}

float executa(char vetor_posfixo[100][10],short int saidaind,float t){
	short int i;
	double v1,v2;
	char stemp[10];
	//printf("vetor_posfixo[0] = %s",vetor_posfixo[0]);
	//printf("saidaind = %d",saidaind);
	for(i=0;i<=saidaind;i++){
		if (v1=atof(vetor_posfixo[i])){
			//v1=atof(vetor_posfixo[i]);
			ex_empilha(v1);
			//printf("empilhou v1 = %f\n",v1);
			//printf("passou 0\n");
		}
		else{
			//printf("i=%d\n",i);
			if (strcmp(vetor_posfixo[i],"t")==0){
				ex_empilha(t);
			//	printf("empilhou t = %f\n",t);
			//	printf("passou 1\n");
			}
			else{
			if (strcmp(vetor_posfixo[i],"tan")==0){
				v1=ex_desempilha();
				v1=tan(v1);
				ex_empilha(v1);
			}
			else{
			if(strcmp(vetor_posfixo[i],"sin")==0){
				v1=ex_desempilha();
				v1=sin(v1);
				ex_empilha(v1);
			}
			else{ 
			if(strcmp(vetor_posfixo[i],"cos")==0){
				v1=ex_desempilha();
				v1=cos(v1);
				ex_empilha(v1);
			}
			else{
			if(strcmp(vetor_posfixo[i],"u")==0){
				v1=ex_desempilha();
				if (t>=v1){
					v1=1;
				}
				else{
					v1=0;
				}
				ex_empilha(v1);
			}
			else{
				v1=ex_desempilha();
				v2=ex_desempilha();
			//	printf("v1 = %f, v2 = %f\n",v1,v2);
			//	printf("passou 2\n");
				switch (vetor_posfixo[i][0]){				
					case'+':
						v2+=v1;
			//			ex_empilha(v2+v1);
			//			printf("empilhou = %f\n",v2+v1);
						break;
					case'-':
						v2-=v1;
			//			ex_empilha(v2-v1);
			//			printf("empilhou = %f\n",v2-v1);
						break;
					case'*':
						v2*=v1;
			//			ex_empilha(v2*v1);
			//			printf("empilhou = %f\n",v2*v1);
						break;
					case'/':
						v2/=v1;
			//			ex_empilha(v2/v1);
			//			printf("empilhou = %f\n",v2/v1);
						break;
					case'^':
						v2=pow(v2,v1);
			//			printf("empilhou = %f\n",pow(v2,v1));
						break;
				}
				//printf("v2=%f\n",v2);				
				ex_empilha(v2);
			}
			}
			}
			}
			}
		}
	}
	float resultado=ex_desempilha();
	printf("resultado = %f\n",resultado);
	return (resultado);
}

int main(){
	char entrada[100];
	char f1_posfix[100][10],f2_posfix[100][10];
	short int saidaind_f1,saidaind_f2;
	float t = 8.990;
	fgets(entrada,100,stdin);
	saidaind_f1=interpreta(entrada,f1_posfix);
	executa(f1_posfix,saidaind_f1,t);

//void interpreta(char *entrada, short int saidaind,char v[100][10])
//void executa(char vetor_posfixo[100][10],short int saidaind,float t)
}










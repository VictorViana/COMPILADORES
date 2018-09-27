#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define T 40

typedef struct Token {
	int classificacao;
	char lexema[T];
}Ttoken;

FILE * abre(char nome[]) {
	FILE * arq;
	arq = fopen(nome, "r+b");
	return arq;
}

Ttoken SCAN(FILE *arquivo);
void programa(FILE *arquivo);
void bloco(FILE *arquivo);
void comando(FILE *arquivo);
void declaracao_variavel(FILE *arquivo);
void expressao_aritmetica(FILE *arquivo);

enum aritmetico{soma=1, sub=2, Div=3, mult=4, igual=5};
enum especial{chavesA=11, chavesF=12, parenA=13, parenF=14, virg=15, pVirg=16};
enum palavrasReserv{Main=20, If=21, Else=22, While=23, Do=24, For=25, Int=26, Float=27, Char=28};
enum identificadores{ERRO=40, caracInvalido=41, carac=30, inteiro=31, flutuante=32, endof=33, identificador=34};
enum operacional{maior=50, menor=51, MAigual=52, MEigual=53, igualigual=54, diferente=55};

Ttoken token;//Token global
int C=1, L=1;//Coluna e Linha global

int main(int argc, char *argv[]) {
	int erro=0;
	FILE *arquivo = abre("scannerx.txt");
	if(arquivo==NULL){
		printf("O arquivo desejado nao existe\n");
		system("pause");
		return 0;
	}
	programa(arquivo);
	if(token.classificacao==endof){
		printf("Tipo:%d EOF\n",endof);
	}
	//system("pause");
	return 0;
}
Ttoken SCAN(FILE *arquivo){
	Ttoken aux;
	int i=0;
	static char caracterDaVez, auxCarac;
	while(1){
pega: caracterDaVez=fgetc(arquivo);
		printf("linha: %d, coluna: %d, codigo: %d\n", L, C, token.classificacao);
		if(caracterDaVez=='\n'){
			C=1;
			L+=1;
			goto pega;
		}
		else if(caracterDaVez=='\t'){
			C+=4;
		}
		else if(feof(arquivo)){
			aux.classificacao=endof;
			return aux;
		}
		if(caracterDaVez==' '){
			C+=1;
			goto pega;
		}
		if(caracterDaVez=='/'){
			caracterDaVez=fgetc(arquivo);
			C+=1;
			if(caracterDaVez=='/'){
loop: caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez!='\n' && !feof(arquivo))
					goto loop;
				if(feof(arquivo)){
					aux.classificacao=endof;
					return aux;
				}
				if(caracterDaVez=='\n'){
					C=0;
					L+=1;
					goto pega;
				}
			}
			else if(caracterDaVez=='*'){
loop1: caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez=='*'){
loop2:caracterDaVez=fgetc(arquivo);
					C+=1;
					if(caracterDaVez=='/')
						goto pega;
					else if(caracterDaVez == '*')
						goto loop2;
				}
				if(feof(arquivo)){
					printf("ERRO na linha %d, coluna %d, ultimo token lido %c: fim do arquivo dentro de comentario\n", L, C, caracterDaVez);
					aux.classificacao=ERRO;
					return aux;
				}
				else
					goto loop1;
			}
			ungetc(caracterDaVez,arquivo);
			C-=1;
			aux.classificacao=Div;
			return aux;
		}
		else if(caracterDaVez=='$' || caracterDaVez==':' || caracterDaVez=='~' || caracterDaVez==']' || caracterDaVez=='[' || caracterDaVez=='&'){
			C+=1;
			printf("\n\n\t\t----Caracter nao existente na linguagem!----\t\t\n");
			printf("ERRO na linha %d, coluna %d, ultimo token lido '%c': Caracter invalido\n", L, C, caracterDaVez);
			aux.classificacao = caracInvalido;
			return aux;
		}
		else if((ispunct(caracterDaVez) && caracterDaVez!= '_') && caracterDaVez!='\''){
			if(caracterDaVez=='.'){
				C+=1;
				goto ponto;
			}
			if(caracterDaVez==';'){
				C+=1;
				aux.classificacao = pVirg;
				return aux;
			}
			else if(caracterDaVez == ','){
				C+=1;
				aux.classificacao = virg;
				return aux;
			}
			else if(caracterDaVez == '{'){
				C+=1;
				aux.classificacao = chavesA;
				return aux;
			}
			else if(caracterDaVez == '}'){
				C+=1;
				aux.classificacao = chavesF;
				return aux;
			}
			else if(caracterDaVez == '('){
				C+=1;
				aux.classificacao = parenA;
				return aux;
			}
			else if(caracterDaVez == ')'){
				C+=1;
				aux.classificacao = parenF;
				return aux;
			}
			else if(caracterDaVez == '+'){
				C+=1;
				aux.classificacao = soma;
				return aux;
			}
			else if(caracterDaVez == '-'){
				C+=1;
				aux.classificacao = sub;
				return aux;
			}
			else if(caracterDaVez == '*'){
				C+=1;
				aux.classificacao = mult;
				return aux;
			}
			else if(caracterDaVez == '='){
				caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez == '='){
					aux.classificacao=igualigual;
					return aux;
				}
				else{
					ungetc(caracterDaVez, arquivo);
					C-=1;
					aux.classificacao=igual;
					return aux;
				}
			}
			else if(caracterDaVez == '!'){
				auxCarac=caracterDaVez;
				caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez == '='){
					aux.classificacao=diferente;
					return aux;
				}
				else{
					ungetc(caracterDaVez, arquivo);
					C-=1;
					aux.classificacao = ERRO;
					printf("\nERRO na linha %d, coluna %d, ultimo token lido '%c': excalamacao sozinha(nao sucedida por '=')\n", L, C, auxCarac);
					return aux;
				}
			}
			else if(caracterDaVez == '>'){
				caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez == '='){
					aux.classificacao=MAigual;
					return aux;
				}
				else{
					ungetc(caracterDaVez, arquivo);
					C-=1;
					aux.classificacao = maior;
					return aux;
				}
			}
			else if(caracterDaVez == '<'){
				caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez == '='){
					aux.classificacao=MEigual;
					return aux;
				}
				else{
					ungetc(caracterDaVez, arquivo);
					C-=1;
					aux.classificacao = menor;
					return aux;
				}
			}
		}
		else if(isalpha(caracterDaVez) || caracterDaVez=='_'){
			for(i=0;isalnum(caracterDaVez) || caracterDaVez=='_';i++){
				C+=1;
				aux.lexema[i]=caracterDaVez;
				caracterDaVez=fgetc(arquivo);
			}
			ungetc(caracterDaVez, arquivo);
			aux.lexema[i]='\0';
			if(strcmp(aux.lexema, "main")==0){
				aux.classificacao=Main;
				return aux;
			}
			if(strcmp(aux.lexema, "if")==0){
				aux.classificacao=If;
				return aux;
			}
			if(strcmp(aux.lexema, "else")==0){
				aux.classificacao=Else;
				return aux;
			}
			if(strcmp(aux.lexema, "while")==0){
				aux.classificacao=While;
				return aux;
			}
			if(strcmp(aux.lexema, "do")==0){
				aux.classificacao=Do;
				return aux;
			}
			if(strcmp(aux.lexema, "float")==0){
				aux.classificacao=Float;
				return aux;
			}
			if(strcmp(aux.lexema, "for")==0){
				aux.classificacao=For;
				return aux;
			}
			if(strcmp(aux.lexema, "char")==0){
				aux.classificacao=Char;
				return aux;
			}
			if(strcmp(aux.lexema, "int")==0){
				aux.classificacao=Int;
				return aux;
			}
			aux.classificacao=identificador;
			return aux;
		}
		else if(isdigit(caracterDaVez)){
			aux.lexema[0]=caracterDaVez;
			caracterDaVez=fgetc(arquivo);
			C+=1;
			for(i=1;isdigit(caracterDaVez);i++){
				aux.lexema[i]=caracterDaVez;
				caracterDaVez=fgetc(arquivo);
				C+=1;
			}
			if(caracterDaVez=='.'){
				aux.lexema[1]='.';
				auxCarac=caracterDaVez;
				caracterDaVez=fgetc(arquivo);
				C+=1;
				if(isdigit(caracterDaVez)){
					for(i=2;isdigit(caracterDaVez);i++){
						aux.lexema[i]=caracterDaVez;
						caracterDaVez=fgetc(arquivo);
						C+=1;
					}
					ungetc(caracterDaVez, arquivo);
					aux.lexema[i]='\0';
					C-=1;
					aux.classificacao=flutuante;
					return aux;
				}
				ungetc(caracterDaVez, arquivo);
				aux.lexema[2]='\0';
				C-=1;
				printf("\n\nERRO na linha %d, coluna %d, ultimo token lido '%s': float mal formado\n", L, C, aux.lexema);
				aux.classificacao=ERRO;
				return aux;
			}
			ungetc(caracterDaVez, arquivo);
			C-=1;
			aux.lexema[i]='\0';
			aux.classificacao=inteiro;
			return aux;
		}
		else if(caracterDaVez=='.'){
ponto: auxCarac=caracterDaVez;
			caracterDaVez=fgetc(arquivo);
			C+=1;
			if(isdigit(caracterDaVez)){
				aux.lexema[0]='0';
				aux.lexema[1]=auxCarac;
				for(i=2;isdigit(caracterDaVez) && caracterDaVez!='\n';i++){
					printf("caracdaVEZ: %c\n",caracterDaVez);
					aux.lexema[i]=caracterDaVez;
					caracterDaVez=fgetc(arquivo);
					C+=1;
				}
				ungetc(caracterDaVez, arquivo);
				aux.lexema[i]='\0';
				C-=1;
				aux.classificacao=flutuante;
				return aux;
			}
			else{
				ungetc(caracterDaVez, arquivo);
				C-=1;
				printf("\nERRO na linha %d, coluna %d, ultimo token lido '%c': float mal formado\n", L, C, auxCarac);
				aux.classificacao = caracInvalido;
				return aux;
			}
		}
		else if(caracterDaVez=='\''){
			caracterDaVez=fgetc(arquivo);
			C+=1;
			aux.lexema[0]=caracterDaVez;
			if(caracterDaVez=='\''){
				aux.lexema[1]='\0';
				aux.classificacao=carac;
				return aux;
			}
			else{
				aux.lexema[1]=caracterDaVez;
				caracterDaVez=fgetc(arquivo);
				C+=1;
				if(caracterDaVez!='\''){
					ungetc(caracterDaVez, arquivo);
					C-=1;
					printf("\nERRO na linha %d, coluna %d, ultimo token lido '%c': char mal formado\n", L, C, caracterDaVez);
					aux.classificacao = ERRO;
					return aux;
				}
				else{
					aux.lexema[1]='\0';
					aux.classificacao = carac;
					return aux;
				}
			}              
		}
	}
}
void fator(FILE *arquivo){
	printf("FATOR\n");
	if(token.classificacao==identificador || token.classificacao==flutuante || token.classificacao==inteiro || token.classificacao==carac){
		token=SCAN(arquivo);
		return;
	}
	else if(token.classificacao==parenA){
		token=SCAN(arquivo);
		expressao_aritmetica(arquivo);
		if(token.classificacao==parenF){
			token=SCAN(arquivo);
			return;
		}
		else{
			printf("ERRO PARSER-> FATOR faltou fecha parenteses\n");
			token.classificacao=ERRO;
			return;
		}
	}
	else{
		printf("ERRO PARSER-> FATOR mal formado\n");
		token.classificacao=ERRO;
		return;
	}
}
void termo(FILE *arquivo){
	printf("TERMO\n");
	if(token.classificacao==identificador || token.classificacao==flutuante || token.classificacao==inteiro || token.classificacao==carac || token.classificacao==parenA){
		fator(arquivo);
		if(token.classificacao!=ERRO){
			while(token.classificacao==mult || token.classificacao==Div){
				token=SCAN(arquivo);
				if(token.classificacao==identificador || token.classificacao==flutuante || token.classificacao==inteiro || token.classificacao==carac || token.classificacao==parenA){
					fator(arquivo);
				}
				else{
					printf("ERRO PARSER->Expressao aritmetica mal formada - Localizado por TERMO\n");
					token.classificacao=ERRO;
					return; 
				}
			}
			return;
		}
		return;
	}
	else{
		printf("ERRO PARSER->Expressao aritmetica mal formada - Localizado por TERMO\n");
		token.classificacao=ERRO;
		return;
	}

}
void expressao_aritmetica(FILE *arquivo){
	printf("EXPRESSAO_ARITMETICA\n");
	if(token.classificacao==identificador || token.classificacao==flutuante || token.classificacao==inteiro || token.classificacao==carac || token.classificacao==parenA){
		termo(arquivo);
		if(token.classificacao!=ERRO){
			while(token.classificacao==sub || token.classificacao==soma){
				token=SCAN(arquivo);
				if(token.classificacao==identificador || token.classificacao==flutuante || token.classificacao==inteiro || token.classificacao==carac || token.classificacao==parenA){
					termo(arquivo);
				}
				else{
					printf("ERRO PARSER->Expressao aritmetica mal formada\n");
					token.classificacao=ERRO;
					return;
				}
			}
			return;
		}
		return;
	}
	else{
		printf("ERRO PARSER->Expressao aritmetica mal formada \n");
		token.classificacao=ERRO;
		return;
	}

}
void expressao_relacional(FILE*arquivo){
	printf("EXPRESSAO_RELACIONAL\n");
	if(token.classificacao==identificador || token.classificacao==flutuante || token.classificacao==inteiro || token.classificacao==carac || token.classificacao==parenA){
		expressao_aritmetica(arquivo);
		if(token.classificacao!=ERRO){
			if(token.classificacao==maior || token.classificacao==menor || token.classificacao==MAigual || token.classificacao==MEigual || token.classificacao==igualigual || token.classificacao==diferente){
				token=SCAN(arquivo);
				expressao_aritmetica(arquivo);
				if(token.classificacao==ERRO)
					return;
			}
			return;
		}
		else
			return;
	}
	else{
		printf("ERRO PARSER->Expressao relacional mal formada \n");
		token.classificacao=ERRO;
		return;
	}
}
void iteracao(FILE *arquivo){
	printf("ITERACAO\n");
	if(token.classificacao==Do){
		token=SCAN(arquivo);
		comando(arquivo);
		if(token.classificacao==ERRO){
			return;
		}
		else if(token.classificacao==While){
			token=SCAN(arquivo);
			if(token.classificacao==parenA){
				token=SCAN(arquivo);
				expressao_relacional(arquivo);
				if(token.classificacao==ERRO){
					return;
				}
				else if(token.classificacao==parenF){
					token=SCAN(arquivo);
					if(token.classificacao==pVirg){
						token=SCAN(arquivo);
						return;
					}
					else{
						printf("ERRO PARSER->Iteracao->do-> faltou ponto e virgula no fim do while\n");
						token.classificacao=ERRO;
						return;
					}
				}
				else{
					printf("ERRO PARSER->Iteracao->do-> faltou fecha parenteses no fim do while\n");
					token.classificacao=ERRO;
					return;
				}
			}
			else{
				printf("ERRO PARSER->Iteracao->do-> faltou abre parenteses depois do while\n");
				token.classificacao=ERRO;
				return;
			}
		}
		else{
			printf("ERRO PARSER->Iteracao->do-> faltou ""while""\n");
			token.classificacao=ERRO;
			return;
		}
	}
	else if(token.classificacao==While){
		token=SCAN(arquivo);
		if(token.classificacao==parenA){
			token=SCAN(arquivo);
			expressao_relacional(arquivo);
			if(token.classificacao==ERRO){
				return;
			}
			else if(token.classificacao==parenF){
				token=SCAN(arquivo);
				comando(arquivo);
				if(token.classificacao==ERRO){
					return;
				}
			}
			else{
				printf("ERRO PARSER->Iteracao->while-> faltou fecha parenteses\n");
				token.classificacao=ERRO;
				return;
			}
		}
		else{
			printf("ERRO PARSER->Iteracao->while-> faltou abre parenteses\n");
			token.classificacao=ERRO;
			return;
		}
	}
	else{
		printf("ERRO PARSER->Iteracao-> faltou ""do"" ou um ""while""\n");
		token.classificacao=ERRO;
		return;
	}
}
void atribuicao(FILE *arquivo){
	printf("ATRIBUICAO\n");
	if(token.classificacao==identificador){
		token=SCAN(arquivo);
		if(token.classificacao==igual){
			token=SCAN(arquivo);
			expressao_aritmetica(arquivo);
			if(token.classificacao!=ERRO){
				if(token.classificacao==pVirg){
					token=SCAN(arquivo);
					return;
				}
				else{
					printf("ERRO PARSER->atribuicao-> faltou ponto e virgula\n");
					token.classificacao=ERRO;
					return;
				}
			}
			return;
		}
		else{
			printf("ERRO PARSER->atribuicao-> token igual (""="") faltando\n");
			token.classificacao=ERRO;
			return;
		}
	}
}
void comando_basico(FILE *arquivo){
	printf("COMANDO_BASICO\n");
	if(token.classificacao==identificador){
		atribuicao(arquivo);
		return;
	}
	else if(token.classificacao==chavesA){
		bloco(arquivo);
	}
	else{
		printf("ERRO PARSER ->comando_basico-> mal formado\n");
		token.classificacao=ERRO;
		return;
	}
}
void comando(FILE *arquivo){
	printf("COMANDO\n");
	if((token.classificacao!=If && (token.classificacao!=While && token.classificacao!=Do)) && token.classificacao!=identificador){
		printf("ERRO PARSER->comando-> mal formado\n");
		token.classificacao=ERRO;
	}
	else{
		if(token.classificacao==identificador || token.classificacao==chavesA){
			comando_basico(arquivo);
		}
		else if(token.classificacao==While || token.classificacao==Do){
			iteracao(arquivo);
		}
		else if(token.classificacao==If){
			token=SCAN(arquivo);
			if(token.classificacao==parenA){
				token=SCAN(arquivo);
				expressao_relacional(arquivo);
				if(token.classificacao==ERRO){
					return;
				}
				else if(token.classificacao==parenF){
					token=SCAN(arquivo);
					comando(arquivo);
					if(token.classificacao==Else){
						token=SCAN(arquivo);
						comando(arquivo);
						if(token.classificacao==ERRO){
							return;
						}
					}
					else
						return;
				}
				else{
					printf("ERRO PARSER->comando->if-> faltou fecha parenteses no fim do ""if""\n");
					token.classificacao=ERRO;
					return;
				}
			}
			else{
				printf("ERRO PARSER->comando->if-> faltou abre parenteses no fim do ""if""\n");
				token.classificacao=ERRO;
				return;
			}
		}
	}
}
void declaracao_variavel(FILE *arquivo){
	printf("DECLARACAO_VARIAVEL\n");	
	if((token.classificacao!=Int && token.classificacao!=Float) && token.classificacao!=Char){
		printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %s: Tipo declarado invalido\n", L, C, token.lexema);
		token.classificacao=ERRO;
		return;
	}
	else{
		token=SCAN(arquivo);
ident:if(token.classificacao==identificador){
		token=SCAN(arquivo);
		if(token.classificacao==virg){
			token=SCAN(arquivo);
			goto ident;
		}
		else if(token.classificacao==pVirg){
			token=SCAN(arquivo);
			return;
		}
		else{
			printf("ERRO PARSER->declaracao_variavel-> Nao foi encontrado virgula ou ponto e virgula\n");
			token.classificacao=ERRO;
			return;
		}
	  }
	  else{
		  printf("ERRO PARSER->declaracao_variavel-> Nao foi encontrado identificador\n");
		  token.classificacao=ERRO;
		  return;
	  }
	}
}
void bloco(FILE *arquivo){
	printf("BLOCO\n");
	if(token.classificacao==chavesA){
		token=SCAN(arquivo);
		if(token.classificacao==Int || token.classificacao==Float || token.classificacao==Char){
			while(token.classificacao==Int || token.classificacao==Float || token.classificacao==Char){
				declaracao_variavel(arquivo);
				if(token.classificacao==ERRO)
					return;
			}
		}
		if(token.classificacao==If || token.classificacao==While || token.classificacao==Do || token.classificacao==identificador || token.classificacao==chavesA){
			while(token.classificacao==If || token.classificacao==While || token.classificacao==Do || token.classificacao==identificador || token.classificacao==chavesA){
				comando(arquivo);
				if(token.classificacao==ERRO)
					return;
			}
		}
		if(token.classificacao==chavesF){
			token=SCAN(arquivo);
			return;
		}
		else{
			printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %c: Nao foi encontrado ""If" "While" "Do" "ou" "Identificador" "\n", L, C, token.lexema);
			token.classificacao=ERRO;
			return;				
		}
	}
	else{
		printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %c: Nao foi encontrado abre chaves\n", L, C, token.lexema);
		token.classificacao=ERRO;
		return;
	}
}
void programa(FILE *arquivo){
	printf("PROGRAMA\n");
	token=SCAN(arquivo);//SE O PRIMEIRO FOR ERRO - AJEITAR
		if(token.classificacao==Int){
			token=SCAN(arquivo);
			if(token.classificacao==Main){
				token=SCAN(arquivo);
				if(token.classificacao==parenA){
					token=SCAN(arquivo);
					if(token.classificacao==parenF){
						token=SCAN(arquivo);
						if(token.classificacao==chavesA){
							bloco(arquivo);
						}
						else{
							printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %s: Falta do abre chaves no cabecalho do codigo\n", L, C, token.lexema);
							token.classificacao=ERRO;
						}
					}
					else{
						printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %s: Falta do fecha parenteses no cabecalho do codigo\n", L, C, token.lexema);
						token.classificacao=ERRO;
					}
				}
				else{
					printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %s: Falta do abre parenteses no cabecalho do codigo\n", L, C, token.lexema);
					token.classificacao=ERRO;
				}
			}
			else{
				printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %s: Falta da palavra reservada \"main\" no cabecalho do codigo\n", L, C, token.lexema);
				token.classificacao=ERRO;
			}
		}
		else{
			printf("ERRO PARSER ->ERRO na linha %d, coluna %d, ultimo token lido %s: Falta da palavra reservada \"int\" no cabecalho do codigo\n", L, C, token.lexema);
			token.classificacao=ERRO;
		}
	return;
}

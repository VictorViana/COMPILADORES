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
 
enum aritmetico{soma=1, sub=2, Div=3, mult=4, igual=5};
enum especial{colchA=11, colchF=12, parenA=13, parenF=14, virg=15, pVirg=16};
enum palavrasReserv{Main=20, If=21, Else=22, While=23, Do=24, For=25, Int=26, Float=27, Char=28};
enum identificadores{ERRO=40, caracInvalido=41, carac=30, inteiro=31, flutuante=32, endof=33, identificador=34};
enum operacional{maior=50, menor=51, MAigual=52, MEigual=53, igualigual=54, diferente=55};
 
int main(int argc, char *argv[]) {
    Ttoken tok;
    int erro=0;
    FILE *arquivo = abre("scannerx.txt");
    if(arquivo==NULL){
        printf("O arquivo desejado nao existe\n");
		system("pause");
		return 0;
    }
    do{
        tok = SCAN(arquivo);
        if(tok.classificacao==30)
            printf("Tipo:%d char\tLexema: %s\n",carac,tok.lexema);
        else if(tok.classificacao==31)
            printf("Tipo:%d int\tLexema: %s\n",inteiro,tok.lexema);
        else if(tok.classificacao==32)
            printf("Tipo:%d float\tLexema: %s\n",flutuante,tok.lexema);
        else if(tok.classificacao==34)
            printf("Tipo:%d Identificador\tLexema: %s\n",identificador,tok.lexema);
        else if(tok.classificacao==40){
            printf("\t\t\tTipo:%d ERRO\n\n",ERRO);
            erro=1;
            break;
        }
        else if(tok.classificacao==41){
            printf("\t\t\tTipo:%d Caracter Invalido\n\n",caracInvalido);
            erro=1;
            break;
        }
        else if(tok.classificacao==1)
            printf("Tipo:%d Soma\n",soma);
        else if(tok.classificacao==2)
            printf("Tipo:%d Subtracao\n",sub);
        else if(tok.classificacao==3)
            printf("Tipo:%d Divisao\n",Div);
        else if(tok.classificacao==4)
            printf("Tipo:%d Multiplicacao\n",mult);
        else if(tok.classificacao==5)
            printf("Tipo:%d Igual\n",igual);
        else if(tok.classificacao==11)
            printf("Tipo:%d Abre colchete\n",colchA);
        else if(tok.classificacao==12)
            printf("Tipo:%d Fecha colchete\n",colchF);
        else if(tok.classificacao==13)
            printf("Tipo:%d Abre parenteses\n",parenA);
        else if(tok.classificacao==14)
            printf("Tipo:%d Fecha parenteses\n",parenF);
        else if(tok.classificacao==15)
            printf("Tipo:%d Virgula\n",virg);
        else if(tok.classificacao==16)
            printf("Tipo:%d Ponto e Virgula\n",pVirg);
        else if(tok.classificacao==20)
            printf("Tipo:%d palavra reservada 'main'\n",Main);
        else if(tok.classificacao==21)
            printf("Tipo:%d palavra reservada 'if'\n",If);
        else if(tok.classificacao==22)
            printf("Tipo:%d palavra reservada 'else'\n",Else);
        else if(tok.classificacao==23)
            printf("Tipo:%d palavra reservada 'while'\n",While);
        else if(tok.classificacao==24)
            printf("Tipo:%d palavra reservada 'do'\n",Do);
        else if(tok.classificacao==25)
            printf("Tipo:%d palavra reservada 'for'\n",For);
        else if(tok.classificacao==26)
            printf("Tipo:%d palavra reservada 'int'\n",Int);
        else if(tok.classificacao==27)
            printf("Tipo:%d palavra reservada 'float'\n",Float);
        else if(tok.classificacao==28)
            printf("Tipo:%d palavra reservada 'char'\n",Char);
        else if(tok.classificacao==50)
            printf("Tipo:%d Operacional\t\n",maior);
        else if(tok.classificacao==51)
            printf("Tipo:%d Operacional\t\n",menor);
        else if(tok.classificacao==52)
            printf("Tipo:%d Operacional\t\n",MAigual);
        else if(tok.classificacao==53)
            printf("Tipo:%d Operacional\t\n",MEigual);
        else if(tok.classificacao==54)
            printf("Tipo:%d Operacional\t\n",igualigual);
        else if(tok.classificacao==55)
            printf("Tipo:%d Operacional\t\n",diferente);
    }while(!feof(arquivo));
    if(erro!=1){
        tok.classificacao=endof;
        printf("Tipo:%d EOF\n",endof);
    }
    //system("pause");
    return 0;
}
Ttoken SCAN(FILE *arquivo){
    Ttoken aux;
	int i=0;
    static int C=1, L=1;//C==Coluna | L==Linha
    static char caracterDaVez, auxCarac;
    while(1){
pega: caracterDaVez=fgetc(arquivo);
        C+=1;
		if(caracterDaVez=='\n'){
			C=0;
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
        if(caracterDaVez==' ')
            goto pega;
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
            printf("\n\n\t\t----Caracter nao existente na linguagem!----\t\t\n");
            printf("ERRO na linha %d, coluna %d, ultimo token lido '%c': Caracter invalido\n", L, C, caracterDaVez);
            aux.classificacao = caracInvalido;
            return aux;
        }
        else if(ispunct(caracterDaVez) && caracterDaVez!='\''){
			if(caracterDaVez=='.')
				goto ponto;
            if(caracterDaVez==';'){
                aux.classificacao = pVirg;
                return aux;
            }
            else if(caracterDaVez == ','){
                aux.classificacao = virg;
                return aux;
            }
            else if(caracterDaVez == '{'){
                aux.classificacao = colchA;
                return aux;
            }
            else if(caracterDaVez == '}'){
                aux.classificacao = colchF;
                return aux;
            }
            else if(caracterDaVez == '('){
                aux.classificacao = parenA;
                return aux;
            }
            else if(caracterDaVez == ')'){
                aux.classificacao = parenF;
                return aux;
            }
            else if(caracterDaVez == '+'){
                aux.classificacao = soma;
                return aux;
            }
            else if(caracterDaVez == '-'){
                aux.classificacao = sub;
                return aux;
            }
            else if(caracterDaVez == '*'){
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
                aux.lexema[i]=caracterDaVez;
                caracterDaVez=fgetc(arquivo);
                C+=1;
            }
            ungetc(caracterDaVez, arquivo);
            C-=1;
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
                aux.lexema[1]='.';
                for(i=2;isdigit(caracterDaVez) && !caracterDaVez=='\n';i++){
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
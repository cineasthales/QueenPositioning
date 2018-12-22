#include "nqueens.h"
#include "simplegrade.h"

void test0(){
    DESCRIBE("Testes de valores inválidos");
    unsigned long long result;

    IF("Dimensão == 0");
    THEN("Resultado == 0");
    result = nqueens(0, 1);
    isEqual_long(result, 0, 1);

    IF("Dimensão Negativa");
    THEN("Resultado == 0");
    result = nqueens(-1, 1);
    isEqual_long(result, 0, 1);

    IF("Número de Rainhas == 0");
    THEN("Resultado == 0");
    result = nqueens(1, 0);
    isEqual_long(result, 0, 1);

    IF("Número Negativo de Rainhas");
    THEN("Resultado == 0");
    result = nqueens(1, -1);
    isEqual_long(result, 0, 1);
}

void test1(){
    DESCRIBE("Testes com uma rainha");
    unsigned long long result;

	IF("Dimensão == 1");
    THEN("Resultado == 1");
    result = nqueens(1, 1);
    isEqual_long(result, 1, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 15");
    result = nqueens(2, 1);
    isEqual_long(result, 15, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 511");
    result = nqueens(3, 1);
    isEqual_long(result, 511, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 65535");
    result = nqueens(4, 1);
    isEqual_long(result, 65535, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 33554431");
    result = nqueens(5, 1);
    isEqual_long(result, 33554431, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 68719476735");
    result = nqueens(6, 1);
    isEqual_long(result, 68719476735, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 562949953421311");
    result = nqueens(7, 1);
    isEqual_long(result, 562949953421311, 1);
}

void test2(){
    DESCRIBE("Testes com duas rainhas");
    unsigned long long result;

    IF("Dimensão == 1");
    THEN("Resultado == 0");
    result = nqueens(1, 2);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 0");
    result = nqueens(2, 2);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 990");
    result = nqueens(3, 2);
    isEqual_long(result, 990, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 389946");
    result = nqueens(4, 2);
    isEqual_long(result, 389946, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 401697908");
    result = nqueens(5, 2);
    isEqual_long(result, 401697908, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 1373360533740");
    result = nqueens(6, 2);
    isEqual_long(result, 1373360533740, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 16884189262692834");
    result = nqueens(7, 2);
    isEqual_long(result, 16884189262692834, 1);
}

void test3(){
    DESCRIBE("Testes com três rainhas");
    unsigned long long result;

    IF("Dimensão == 1");
    THEN("Resultado == 0");
    result = nqueens(1, 3);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 0");
    result = nqueens(2, 3);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 0");
    result = nqueens(3, 3);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 312936");
    result = nqueens(4, 3);
    isEqual_long(result, 312936, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 865055776");
    result = nqueens(5, 3);
    isEqual_long(result, 865055776, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 6264797809140");
    result = nqueens(6, 3);
    isEqual_long(result, 6264797809140, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 133259420182475128");
    result = nqueens(7, 3);
    isEqual_long(result, 133259420182475128, 1);
}

void test4(){
    DESCRIBE("Testes com quatro rainhas");
    unsigned long long result;

    IF("Dimensão == 1");
    THEN("Resultado == 0");
    result = nqueens(1, 4);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 0");
    result = nqueens(2, 4);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 0");
    result = nqueens(3, 4);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 27030");
    result = nqueens(4, 4);
    isEqual_long(result, 27030, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 454950820");
    result = nqueens(5, 4);
    isEqual_long(result, 454950820, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 7690767698250");
    result = nqueens(6, 4);
    isEqual_long(result, 7690767698250, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 337688339564454560");
    result = nqueens(7, 4);
    isEqual_long(result, 337688339564454560, 1);
}

void test5(){
    DESCRIBE("Testes com cinco rainhas");
    unsigned long long result;

    IF("Dimensão == 1");
    THEN("Resultado == 0");
    result = nqueens(1, 5);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 0");
    result = nqueens(2, 5);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 0");
    result = nqueens(3, 5);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 0");
    result = nqueens(4, 5);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 67108862");
    result = nqueens(5, 5);
    isEqual_long(result, 67108862, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 2203265494560");
    result = nqueens(6, 5);
    isEqual_long(result, 2203265494560, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 267693484922213342");
    result = nqueens(7, 5);
    isEqual_long(result, 267693484922213342, 1);
}

void test6() {
    DESCRIBE("Testes com seis rainhas");
    unsigned long long result;

    IF("Dimensão == 1");
    THEN("Resultado == 0");
    result = nqueens(1, 6);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 0");
    result = nqueens(2, 6);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 0");
    result = nqueens(3, 6);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 0");
    result = nqueens(4, 6);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 0");
    result = nqueens(5, 6);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 32980810590");
    result = nqueens(6, 6);
    isEqual_long(result, 32980810590, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 52838796563824902");
    result = nqueens(7, 6);
    isEqual_long(result, 52838796563824902, 1);
}

void test7() {
    DESCRIBE("Testes com sete rainhas");
    unsigned long long result;

    IF("Dimensão == 1");
    THEN("Resultado == 0");
    result = nqueens(1, 7);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 2");
    THEN("Resultado == 0");
    result = nqueens(2, 7);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 3");
    THEN("Resultado == 0");
    result = nqueens(3, 7);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 4");
    THEN("Resultado == 0");
    result = nqueens(4, 7);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 5");
    THEN("Resultado == 0");
    result = nqueens(5, 7);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 6");
    THEN("Resultado == 0");
    result = nqueens(6, 7);
    isEqual_long(result, 0, 1);

    IF("Dimensão == 7");
    THEN("Resultado == 2955379830685210");
    result = nqueens(7, 7);
    isEqual_long(result, 2955379830685210, 1);
}

int main(){
    DESCRIBE("CALCULADORA DE RAINHAS EM TABULEIRO QUADRADO");
    DESCRIBE("Testes do Aluno");

    test0();
    test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();

	GRADEME();

	if (grade == maxgrade)
        return 0;
	else
        return grade;
}

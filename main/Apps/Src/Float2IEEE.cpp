/*
 * Float2IEEE.cpp
 *
 *  Created on: Aug 18, 2022
 *      Author: ayato
 */




#include <stdio.h>
#include <bitset>

#include "Float2IEEE.hpp"

int oct2bin_int(unsigned int *, int, int);
int oct2bin_float(unsigned int *, float, int);


/*
 * １０進数の正の整数を２進数に変換
 * bin：２進数を格納する配列のアドレス
 * oct：２進数に変換する１０進数
 * max_bit：最大ビット数
 * return：ビット数
 */
int oct2bin_int(unsigned int *bin, int oct, int max_bit) {
    int i;

    /* 第0桁から値を算出していく */
    i = 0;
    while (oct > 0 && i < max_bit) {
        /* 剰余算で２進数の第n桁を算出 */
        bin[i] = oct % 2;

        /* 除算で第n桁目を切り捨て */
        oct = oct / 2;

        /* 次の桁へ */
        i++;
    }
    return i;
}

/*
 * １０進数の小数を２進数に変換
 * bin：２進数を格納する配列のアドレス
 * oct：２進数に変換する１０進数
 * max_bit：最大ビット数
 * return：ビット数
 */
int oct2bin_float(unsigned int *bin, float oct, int max_bit) {
    int i;
    int int_oct;
    float under_oct;

    i = 0;
    while(oct != 0 && i < max_bit) {
        /* 冪数を+1 */
        oct = oct * 2;

        /* 整数部分のみを取り出す */
        int_oct = oct;

        /* 剰余算で 2 の 0 乗の項の値を取得し、
           小数点以下第 i + 1 桁の値として決定 */
        bin[i] = int_oct % 2;

        /* 小数点以下の値のみを取り出す */
        under_oct = oct - int_oct;

        /* 次の桁へ */
        i++;

        if (under_oct == 0) {
            /* 小数点以下が 0 なら終了 */
            break;
        }
    }
    return i;
}

uint32_t Float2IEEE( float f )
{
	uint32_t *pfloatingToIntValue;
	pfloatingToIntValue = ( uint32_t* )&f;

	return (*pfloatingToIntValue);
}


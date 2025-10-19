
#include <stdio.h>        /* 地雷除去掃海艇2012 */
#include <stdlib.h>
#include <time.h>

#define MIN 575           /* 地雷の最大設置個数 */
#define HEI  24           /* 縦の最大設定マス数 */
#define WID  24           /* 横の最大設定マス数 */

double clp;               /* 除去に成功した確率 */
double fap;               /* 除去に失敗した確率 */
double rat;               /* 進行度の割合を格納 */

int clc;                  /* 除去に成功した回数 */
int fac;                  /* 除去に失敗した回数 */
int cnt;          /* 確保した安全なマス数を格納 */
int dif;          /* 各選択肢の持つ難度値を格納 */
int flg;          /* 真偽を判定するフラグを示す */

int min;                  /* 設置する地雷の個数 */
int hei;                  /* 設定する縦のマス数 */
int wid;                  /* 設定する横のマス数 */
int area[HEI][WID];       /* 表示用の二次元配列 */
int copy[HEI][WID];       /* 入力用の二次元配列 */

void check(int, int);     /*   check 関数の宣言 */
void setup(void);         /*   setup 関数の宣言 */
void display(void);       /* display 関数の宣言 */
void minelay(int, int);   /* minelay 関数の宣言 */



int main(void) {

	FILE *fp;
	int bx, by, i, j, x, y;

	srand((unsigned int)time(NULL));

	setup();               /*   setup 関数の呼出 */

	cnt = 0;               /*   cnt 変数の初期化 */
	flg = 0;               /*   flg 変数の初期化 */

                          /* 二次元配列の初期化 */
	for(y = 0; y < HEI; y++)
		for(x = 0; x < WID; x++) {
			area[y][x] = 0;
			copy[y][x] = 0;
		}

	fp = fopen("res.txt", "r");

	               /* ポインタfpがNULLだった場合 */
	if(fp == NULL) {
		clc = 0;            /*   clc 変数の初期化 */
		fac = 0;            /*   fac 変数の初期化 */
		clp = 0.0;          /*   clp 変数の初期化 */
		fap = 0.0;          /*   fap 変数の初期化 */
	}
	else           /* ポインタfpがNULLでない場合 */
		/* ファイルから入力された値を各変数に格納 */
		fscanf(fp, "%d %d %lf %lf", &clc, &fac, &clp, &fap);

	fclose(fp);

	display();             /* display 関数の呼出 */

	do {
		do {
			printf("\n");

			do {     /*  by 変数のリミットチェック */
				printf(" Accept Y: ");
				scanf("%d", &by);
			} while(by < 1 || by > hei);

			by--;

			do {     /*  bx 変数のリミットチェック */
				printf(" Accept X: ");
				scanf("%d", &bx);
			} while(bx < 1 || bx > wid);

			bx--;
		} while(copy[by][bx] == 1);

		if(flg == 0) {
			flg = 1;
			minelay(bx, by); /* minelay 関数の呼出 */
		}

		if(area[by][bx] == 0)
			check(bx, by);   /*   check 関数の呼出 */
		else {
			cnt++;
			copy[by][bx] = 1;
		}

		if(cnt + min == hei * wid) {
			clc++;
			clp = (double) clc / (clc + fac) * 100;
			fap = (double) fac / (fac + clc) * 100;
		}
		else if(area[by][bx] == 9) {
			fac++;
			clp = (double) clc / (clc + fac) * 100;
			fap = (double) fac / (fac + clc) * 100;
		}

		display();          /* display 関数の呼出 */
	} while(area[by][bx] != 9 && cnt + min != hei * wid);

	fp = fopen("res.txt", "w");

	if(fp == NULL)
		return 1;

	               /*       結果をファイルに出力 */
	fprintf(fp, "%d ", clc);
	fprintf(fp, "%d ", fac);
	fprintf(fp, "%.1f ", clp);
	fprintf(fp, "%.1f ", fap);

	                       /*   結果を画面に出力 */
	if(cnt + min == hei * wid)
		printf("\n 地雷除去に成功しました。:\n");
	else if(area[by][bx] == 9)
		printf("\n 地雷除去に失敗しました。:\n");

	fclose(fp);

	return 0;
}



                          /*   check 関数の定義 */
void check(int x, int y) {

	if((x >= 0 && x < wid) && (y >= 0 && y < hei)) {
		cnt++;
		copy[y][x] = 1;

		if(x - 1 >= 0 && y - 1 >= 0) {
			if(area[y - 1][x - 1] == 0 && copy[y - 1][x - 1] == 0)
				check(x - 1, y - 1);
			else
				if(copy[y - 1][x - 1] == 0) {
					cnt++;
					copy[y - 1][x - 1] = 1;
				}
		}

		if(y - 1 >= 0) {
			if(area[y - 1][x] == 0 && copy[y - 1][x] == 0)
				check(x, y - 1);
			else
				if(copy[y - 1][x] == 0) {
					cnt++;
					copy[y - 1][x] = 1;
				}
		}

		if(x + 1 < wid && y - 1 >= 0) {
			if(area[y - 1][x + 1] == 0 && copy[y - 1][x + 1] == 0)
				check(x + 1, y - 1);
			else
				if(copy[y - 1][x + 1] == 0) {
					cnt++;
					copy[y - 1][x + 1] = 1;
				}
		}

		if(x - 1 >= 0) {
			if(area[y][x - 1] == 0 && copy[y][x - 1] == 0)
				check(x - 1, y);
			else
				if(copy[y][x - 1] == 0) {
					cnt++;
					copy[y][x - 1] = 1;
				}
		}

		if(x + 1 < wid) {
			if(area[y][x + 1] == 0 && copy[y][x + 1] == 0)
				check(x + 1, y);
			else
				if(copy[y][x + 1] == 0) {
					cnt++;
					copy[y][x + 1] = 1;
				}
		}

		if(x - 1 >= 0 && y + 1 < hei) {
			if(area[y + 1][x - 1] == 0 && copy[y + 1][x - 1] == 0)
				check(x - 1, y + 1);
			else
				if(copy[y + 1][x - 1] == 0) {
					cnt++;
					copy[y + 1][x - 1] = 1;
				}
		}

		if(y + 1 < hei) {
			if(area[y + 1][x] == 0 && copy[y + 1][x] == 0)
				check(x, y + 1);
			else
				if(copy[y + 1][x] == 0) {
					cnt++;
					copy[y + 1][x] = 1;
				}
		}

		if(x + 1 < wid && y + 1 < hei) {
			if(area[y + 1][x + 1] == 0 && copy[y + 1][x + 1] == 0)
				check(x + 1, y + 1);
			else
				if(copy[y + 1][x + 1] == 0) {
					cnt++;
					copy[y + 1][x + 1] = 1;
				}
		}
	}
}



void setup(void) {        /*   setup 関数の定義 */

	min = 10;      /* min 変数のデフォルト値設定 */
	hei =  9;      /* hei 変数のデフォルト値設定 */
	wid =  9;      /* wid 変数のデフォルト値設定 */

	printf("\n 初期設定を行いますか？  :");

	do {           /* flg 変数のリミットチェック */
		printf(" ");
		scanf("%d", &flg);
	} while(flg < 0 || flg > 1);

	if(flg) {
		printf("\n 初期設定を行います。    :\n");

		printf("\n 縦のマス数を設定します。:");

		do {        /* hei 変数のリミットチェック */
			printf(" ");
			scanf("%d", &hei);
		} while(hei < 1 || hei > HEI);

		printf("\n 横のマス数を設定します。:");

		do {        /* wid 変数のリミットチェック */
			printf(" ");
			scanf("%d", &wid);
		} while(wid < 1 || wid > WID);

		printf("\n 地雷の個数を設定します。:");

		do {        /* min 変数のリミットチェック */
			printf(" ");
			scanf("%d", &min);
		} while(min < 0 || min > MIN || min > (hei * wid));
	}

	else {
		printf("\n 難易度設定を行いますか？:");

		do {        /* flg 変数のリミットチェック */
			printf(" ");
			scanf("%d", &flg);
		} while(flg < 0 || flg > 1);

		if(flg) {
			printf("\n 難易度設定を行います。  :\n");

			printf("\n 初級 (1)");
			printf("\n 中級 (2)");
			printf("\n 上級 (3)\n");

			printf("\n 難易度を選択して下さい。:");

			do {     /* dif 変数のリミットチェック */
				printf(" ");
				scanf("%d", &dif);
			} while(dif < 1 || dif > 3);

			switch(dif) {    /* 選択した難度の設定 */
				case 1:
					min = 10;
					hei =  9;
					wid =  9;
					printf("\n 初級を選択しました。    :\n");
					break;

				case 2:
					min = 40;
					hei = 16;
					wid = 16;
					printf("\n 中級を選択しました。    :\n");
					break;

				case 3:
					min = 99;
					hei = 23;
					wid = 23;
					printf("\n 上級を選択しました。    :\n");
					break;
			}
		}

		else {
			printf("\n デフォルトに設定します。:\n");
		}
	}
}



void display(void) {      /* display 関数の定義 */

	int n, x, y;

	system("cls");

/*                               ┌─────┐ */
/*          Minesweeper 2012 (C) │          │ */
/*                               └─────┘ */

	for(x = 0; x < wid + 2; x++)
		printf("  ");

	printf("┌─────┐\n");
	printf(" ");

	for(x = 0; x < wid + 1; x++)
		switch(x) {
			case 0:
				printf("Mi");
				break;

			case 1:
				printf("ne");
				break;

			case 2:
				printf("sw");
				break;

			case 3:
				printf("ee");
				break;

			case 4:
				printf("pe");
				break;

			case 5:
				printf("r ");
				break;

			case 6:
				printf("20");
				break;

			case 7:
				printf("12");
				break;

			case 8:
				printf(" (");
				break;

			case 9:
				printf("C)");
				break;

			default:
				printf("  ");
				break;
		}

	printf(" │");

	rat = (double) cnt / (hei * wid - min) * 100;
	rat = (int) rat / 10;

	for(x = 0; x < rat; x++)
		printf("%c", 26);

	for( ; x < 10; x++)
		printf(" ");

	printf("│\n");

	for(x = 0; x < wid + 2; x++)
		printf("  ");

	printf("└─────┘\n");

/*         ┌────┬────┐┌──┬──┐ */
/*         │■■■■■■■■■││    │    │ */
/*         │■■■■■■■■■│├──┼──┤ */
/*         │■■■■■■■■■││    │    │ */
/*         │■■■■■■■■■│├──┼──┤ */
/*         ├■■■■■■■■■┤│    │    │ */
/*         │■■■■■■■■■│├──┼──┤ */
/*         │■■■■■■■■■││    │    │ */
/*         │■■■■■■■■■│├──┼──┤ */
/*         │■■■■■■■■■││    │    │ */
/*         └────┴────┘└──┴──┘ */

	printf("┌");

	for(x = 0; x < wid; x++) {
		if(x % 5 == 4)
			printf("┬");
		else
			printf("─");
	}

	n = hei - 9;

	if(n <= 0)
		printf("┐┌──┬──┐\n");
	else
		printf("┐\n");

	for(y = 0; y < hei; y++) {
		if(y % 5 == 4)
			printf("├");
		else
			printf("│");

		for(x = 0; x < wid; x++) {
			if(copy[y][x] == 1)
				switch(area[y][x]) {
					case 0:
						printf("□");
						break;

					case 1:
						printf("１");
						break;

					case 2:
						printf("２");
						break;

					case 3:
						printf("３");
						break;

					case 4:
						printf("４");
						break;

					case 5:
						printf("５");
						break;

					case 6:
						printf("６");
						break;

					case 7:
						printf("７");
						break;

					case 8:
						printf("８");
						break;

					case 9:
						printf("×");
						break;
				}
			else
				printf("■");
		}

		if(y % 5 == 4)
			printf("┤");
		else
			printf("│");

		switch(y - n) {
			case -1:
				printf("┌──┬──┐");
				break;

			case 0:
				printf("│Cnt │%4d│", clc + fac);
				break;

			case 1:
				printf("├──┼──┤");
				break;

			case 2:
				printf("│Clc │%4d│", clc);
				break;

			case 3:
				printf("├──┼──┤");
				break;

			case 4:
				printf("│Fac │%4d│", fac);
				break;

			case 5:
				printf("├──┼──┤");
				break;

			case 6:
				printf("│Clp │%3.f%│", clp);
				break;

			case 7:
				printf("├──┼──┤");
				break;

			case 8:
				printf("│Fap │%3.f%│", fap);
				break;
		}

		printf("\n");
	}

	printf("└");

	for(x = 0; x < wid; x++) {
		if(x % 5 == 4)
			printf("┴");
		else
			printf("─");
	}

	printf("┘└──┴──┘\n");
}



                          /* minelay 関数の定義 */
void minelay(int bx, int by) {

	int x, y, i;

	for(i = 0; i < min; i++)
		do {
			y = rand() % hei;
			x = rand() % wid;

			if(area[y][x] != 0)
				area[y][x] = 2;
			else if(x != bx || y != by)
				area[y][x] = 1;
		} while(area[y][x] == 2 || (x == bx && y == by));

	for(y = 0; y < hei; y++)
		for(x = 0; x < wid; x++)
			if(area[y][x] > 0)
				area[y][x] = 9;

	for(y = 0; y < hei; y++)
		for(x = 0; x < wid; x++)
			if(area[y][x] == 9) {
				if(x - 1 >= 0 && y - 1 >= 0)
					if(area[y - 1][x - 1] != 9)
						area[y - 1][x - 1] += 1;

				if(y - 1 >= 0)
					if(area[y - 1][x] != 9)
						area[y - 1][x] += 1;

				if(x + 1 < wid && y - 1 >= 0)
					if(area[y - 1][x + 1] != 9)
						area[y - 1][x + 1] += 1;

				if(x - 1 >= 0)
					if(area[y][x - 1] != 9)
						area[y][x - 1] += 1;

				if(x + 1 < wid)
					if(area[y][x + 1] != 9)
						area[y][x + 1] += 1;

				if(x - 1 >= 0 && y + 1 < hei)
					if(area[y + 1][x - 1] != 9)
						area[y + 1][x - 1] += 1;

				if(y + 1 < hei)
					if(area[y + 1][x] != 9)
						area[y + 1][x] += 1;

				if(x + 1 < wid && y + 1 < hei)
					if(area[y + 1][x + 1] != 9)
						area[y + 1][x + 1] += 1;
			}
}

/*          ┌────────────────┐*/
/*          │生死去来棚頭傀儡一線断時落落磊磊│*/
/*  ┌───┴────────────────┤*/
/*  │本月本日以目出度死去仕候間此段広告仕候也│*/
/*  └────────────────────┘*/

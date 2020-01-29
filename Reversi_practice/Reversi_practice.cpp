#include "pch.h"
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<time.h>
#define TITLE "Reversi ver1.11.5"

int board[10][10] = {};
int turn = 1;
int last_turn = 0;
int flag_L = 0;
int flag = 1;
int flag_reverse = 0;
float str_pos_x = -1.0f;
float str_pos_y = 0.0f;
int flag_end = 0;
int cursor_color = 1;/*1:黒、2:白*/
double cursor_x = -2.0, cursor_y = 0.0;
int finish_num_black = 0, finish_num_white = 0;
int finish_count1 = 0, finish_count2 = 0;
int flag_finish_count = 0;
int flag_once1 = 1;
int flag_once2 = 1;

int temp_board[64] = {};
int delay_counter = 0;
int flag_restart = 0;
int flag_animation = 0;

double RateDisplay = 10.0 / 13.0;
int botton_state = 2;/*1=押されていない、2=押されている*/
int restart_botton_state = 1;/*1=押されていない、2=押されている*/

void RenderString(float x, float y, const char *str, int color) {	/*文字列表示*/
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	if (color == 1) {
		glColor3f(1, 1, 0);
	}
	else if (color == 0) {
		glColor3f(1, 0, 0);
	}
	else if (color == 2) {
		glColor3f(0, 0, 0);
	}
	else if (color == 3) {
		glColor3f(160.0 / 256.0, 160.0 / 256.0, 160.0 / 256.0);
	}
	glRasterPos2f(x, y);
	char *p = (char *)str;
	while (*p != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
	}
	glPopAttrib();
}

void animation(int value) {
	if (flag_animation) {
		if (str_pos_x < -0.4) {
			str_pos_x += 0.003f;
			str_pos_y = (float)(sin((str_pos_x + 1) * 3 * M_PI / (1 - 0.4)) / ((str_pos_x + 1) * 3 * M_PI / (1 - 0.4)));
			if (str_pos_y < 0) {
				str_pos_y *= -1;
			}
		}
		if (str_pos_x >= -0.4) {
			flag_restart = 1;
		}
		glutPostRedisplay();
		glutTimerFunc(1, animation, 0);
	}
}

int reverse_leftup(int i, int j, int color) {							/*ひっくり返し判定(左上)*/
	if (board[i - 1][j - 1] == 0) {/*左上が空白or端の時*/
		return 0;
	}
	if (board[i - 1][j - 1] != color && board[i - 1][j - 1] > 0) {/*左上の色が違うとき*/
		flag_reverse = 1;										/*反転フラグオン*/
		reverse_leftup(i - 1, j - 1, color);					/*再帰*/
	}
	if (flag_reverse) {
		if (board[i - 1][j - 1] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse_up(int i, int j, int color) {							/*ひっくり返し判定(上)*/
	if (board[i - 1][j] == 0) {
		return 0;
	}
	if (board[i - 1][j] != color && board[i - 1][j] > 0) {
		flag_reverse = 1;
		reverse_up(i - 1, j, color);
	}
	if (flag_reverse) {
		if (board[i - 1][j] == color) {
			board[i][j] = color;

			return 1;
		}
	}
	return 0;
}
int reverse_rightup(int i, int j, int color) {						/*ひっくり返し判定(右上)*/
	if (board[i - 1][j + 1] == 0) {
		return 0;
	}
	if (board[i - 1][j + 1] != color && board[i - 1][j + 1] > 0) {
		flag_reverse = 1;
		reverse_rightup(i - 1, j + 1, color);
	}
	if (flag_reverse) {
		if (board[i - 1][j + 1] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse_left(int i, int j, int color) {							/*ひっくり返し判定(左)*/
	if (board[i][j - 1] == 0) {
		return 0;
	}
	if (board[i][j - 1] != color && board[i][j - 1] > 0) {
		flag_reverse = 1;
		reverse_left(i, j - 1, color);
	}
	if (flag_reverse) {
		if (board[i][j - 1] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse_right(int i, int j, int color) {						/*ひっくり返し判定(右)*/
	if (board[i][j + 1] == 0) {
		return 0;
	}
	if (board[i][j + 1] != color && board[i][j + 1] > 0) {
		flag_reverse = 1;
		reverse_right(i, j + 1, color);
	}
	if (flag_reverse) {
		if (board[i][j + 1] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse_leftdown(int i, int j, int color) {						/*ひっくり返し判定(左下)*/
	if (board[i + 1][j - 1] == 0) {
		return 0;
	}
	if (board[i + 1][j - 1] != color && board[i + 1][j - 1] > 0) {
		flag_reverse = 1;
		reverse_leftdown(i + 1, j - 1, color);
	}
	if (flag_reverse) {
		if (board[i + 1][j - 1] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse_down(int i, int j, int color) {							/*ひっくり返し判定(下)*/
	if (board[i + 1][j] == 0) {
		return 0;
	}
	if (board[i + 1][j] != color && board[i + 1][j] > 0) {
		flag_reverse = 1;
		reverse_down(i + 1, j, color);
	}
	if (flag_reverse) {
		if (board[i + 1][j] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse_rightdown(int i, int j, int color) {					/*ひっくり返し判定(右下)*/
	if (board[i + 1][j + 1] == 0) {
		return 0;
	}
	if (board[i + 1][j + 1] != color && board[i + 1][j + 1] > 0) {
		flag_reverse = 1;
		reverse_rightdown(i + 1, j + 1, color);
	}
	if (flag_reverse) {
		if (board[i + 1][j + 1] == color) {
			board[i][j] = color;
			return 1;
		}
	}
	return 0;
}
int reverse(int i, int j, int color) {			/*ひっくり返しまとめ*/
	int k = reverse_leftup(i, j, color);
	flag_reverse = 0;
	int l = reverse_up(i, j, color);
	flag_reverse = 0;
	int m = reverse_rightup(i, j, color);
	flag_reverse = 0;
	int n = reverse_left(i, j, color);
	flag_reverse = 0;
	int o = reverse_right(i, j, color);
	flag_reverse = 0;
	int p = reverse_leftdown(i, j, color);
	flag_reverse = 0;
	int q = reverse_down(i, j, color);
	flag_reverse = 0;
	int r = reverse_rightdown(i, j, color);
	flag_reverse = 0;
	//	printf("%d,%d,%d,%d,%d,%d,%d,%d\n", k, l, m, n, o, p, q, r);
	if (k == 1 || l == 1 || m == 1 || n == 1 || o == 1 || p == 1 || q == 1 || r == 1) {
		return 1;	/*どれか一つでもひっくり返せたら1を返す*/
	}
	return 0;		/*ひっくり返らなかったら0を返す*/
}

int check(void) {			/*ひっくり返し可能かどうか判定する*/
	int temp[8][8] = {};
	int i, j, k, l;
	int check1 = 0, check2 = 0;
	for (i = 1; i < 9; i++) {
		for (j = 1; j < 9; j++) {
			temp[i - 1][j - 1] = board[i][j];
		}
	}
	for (i = 1; i < 9; i++) {
		for (j = 1; j < 9; j++) {
			if (board[i][j] <= 0) {
				if (turn % 2) {
					if (reverse(i, j, 1)) {		/*1つでもひっくり返せればボードを戻して0を返す*/
						for (k = 1; k < 9; k++) {
							for (l = 1; l < 9; l++) {
								board[k][l] = temp[k - 1][l - 1];
							}
						}
						return 0;
					}
				}
				else {
					if (reverse(i, j, 2)) {
						for (k = 1; k < 9; k++) {
							for (l = 1; l < 9; l++) {
								board[k][l] = temp[k - 1][l - 1];
							}
						}
						return 0;
					}
				}
			}
		}
	}
	for (k = 1; k < 9; k++) {
		for (l = 1; l < 9; l++) {
			board[k][l] = temp[k - 1][l - 1];
		}
	}
	return 1;
}

void possibility(int x, int y, int color/*x,y:指定している場所の配列での番号、color:そのターンの色*/)/*置ける場所をハイライトする*/ {
	if (board[x][y] == 1 || board[x][y] == 2) {
		return;
	}
	int i, j;
	int temp[8][8] = {};
	for (i = 1; i < 9; i++) {
		for (j = 1; j < 9; j++) {
			if (board[i][j] == -1 || board[i][j] == -2) {
				board[i][j] = 0;
			}
			temp[i - 1][j - 1] = board[i][j];
		}
	}
	if (reverse(x, y, color)) {
		for (i = 1; i < 9; i++) {
			for (j = 1; j < 9; j++) {
				board[i][j] = temp[i - 1][j - 1];
			}
		}
		if (color == 1) {
			board[x][y] = -1;
		}
		else if (color == 2) {
			board[x][y] = -2;
		}
	}
}

void finish() {								/*ゲームの終了と勝敗を判定する*/
	int i, j;
	int check1, check2;
	int stone_num[3] = {};
	for (i = 1; i < 9; i++) {
		for (j = 1; j < 9; j++) {
			if (board[i][j] == 0) {
				stone_num[0]++;
			}
			if (board[i][j] == 1) {
				stone_num[1]++;
			}
			if (board[i][j] == 2) {
				stone_num[2]++;
			}
		}
	}
	printf("(B:%d,W:%d)  ", stone_num[1], stone_num[2]);
	/*	if (!(stone_num[0] && stone_num[1] && stone_num[2])) {
			printf("game set!\n");
			finish_num_black = stone_num[1];
			finish_num_white = stone_num[2];
			if (stone_num[1] > stone_num[2]) {
				printf("Winner Black\n");/*
				for (i = 1; i < 9; i++) {/*変更部分*//*
					for (j = 1; j < 9; j++) {
						board[i][j] = 0;
					}
				}						/*変更終わり*/
				/*			flag_end = 2;
						}
						else if (stone_num[2] > stone_num[1]) {
							printf("Winner White\n");/*
							for (i = 1; i < 9; i++) {/*変更部分*//*
								for (j = 1; j < 9; j++) {
									board[i][j] = 0;
								}
							}						/*変更終わり*/
							/*			flag_end = 1;
									}
									else if (stone_num[1] == stone_num[2]) {
										printf("draw\n");/*
										for (i = 1; i < 9; i++) {/*変更部分*//*
											for (j = 1; j < 9; j++) {
												board[i][j] = 0;
											}
										}						/*変更終わり*/
										/*			flag_end = -1;
												}
												turn = 0;
											}
											else {*/
	check1 = check();
	turn++;
	check2 = check();
	turn--;
	if (check1&&check2) {
		printf("game set!\n");
		finish_num_black = stone_num[1];
		finish_num_white = stone_num[2];

		if (stone_num[1] > stone_num[2]) {
			printf("Winner Black\n");
			flag_end = 2;
		}
		else if (stone_num[2] > stone_num[1]) {
			printf("Winner White\n");
			flag_end = 1;
		}
		else if (stone_num[1] == stone_num[2]) {
			printf("draw\n");
			flag_end = -1;
		}
		turn = 0;
	}
	//	}
}

void cursor(int x, int y) {
	int IMG_X = glutGet(GLUT_WINDOW_WIDTH);
	int IMG_Y = glutGet(GLUT_WINDOW_HEIGHT);
	if (turn % 2) {
		cursor_color = 1;
	}
	else {
		cursor_color = 2;
	}
	double fixed_x = 0, fixed_y = 0;
	fixed_x = ((double)x - (double)(IMG_X / 2.0)) / (double)(IMG_X / 2.0);
	fixed_y = ((double)(500 - y) - (double)(IMG_Y / 2.0)) / (double)(IMG_Y / 2.0);
	//	printf("x=%d,y=%d,fixedx=%f,fixedy=%f\n", x, y, fixed_x, fixed_y);
	cursor_x = fixed_x;
	cursor_y = fixed_y;
	glutPostRedisplay();
}

void restart() {
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			board[i][j] = 0;
		}
	}
	turn = 1;
	last_turn = 0;
	flag_L = 0;
	flag = 1;
	flag_reverse = 0;
	str_pos_x = -1.0f;
	str_pos_y = 0.0f;
	flag_end = 0;
	cursor_color = 1;
	cursor_x = -2.0; cursor_y = 0.0;
	finish_num_black = 0; finish_num_white = 0;
	finish_count1 = 0; finish_count2 = 0;
	flag_finish_count = 0;
	flag_once1 = 1;
	flag_once2 = 1;
	for (i = 0; i < 64; i++) {
		temp_board[i] = 0;
	}
	delay_counter = 0;
	flag_restart = 0;
	flag_animation = 0;
	botton_state = 2;
	restart_botton_state = 1;
	glutPostRedisplay();
}

void mouse(int botton, int state, int x, int y) {
	int IMG_X;
	int IMG_Y;
	switch (botton)
	{
	case GLUT_LEFT_BUTTON:
		flag_L = 1;
		break;
	default:
		break;
	}
	switch (state) {
	case GLUT_UP:
		IMG_X = glutGet(GLUT_WINDOW_WIDTH);
		IMG_Y = glutGet(GLUT_WINDOW_HEIGHT);
		if (turn >= 0) {
			if (flag_L) {
				//				printf("x:%d,y:%d\n", x, y);
				for (int j = 1; j < 9; j++) {
					if (x >= (RateDisplay*IMG_X / 8)*(j - 1) && x < (RateDisplay*IMG_X / 8)*j) {
						for (int i = 1; i < 9; i++) {
							if (y >= (IMG_Y / 8)*(i - 1) && y < (IMG_Y / 8) * i) {
								/*手動ひっくり返し部分*/
								/*if (board[i][j] == 1) {
									board[i][j] = 2;
								}
								else if (board[i][j] == 2) {
									board[i][j] = 1;
								}*/
								/*手動ひっくり返し部分終わり*/
								/*else*/ if (turn % 2) {/*黒のターン*/
									if (board[i][j] == 0 || board[i][j] == -1) {	/*クリックした位置に石がないとき*/
										if (reverse(i, j, 1)) {
											turn++;
											finish();
										}
									}
								}
								else {
									if (board[i][j] == 0 || board[i][j] == -2) {/*白のターン*/
										if (reverse(i, j, 2)) {
											turn++;
											finish();
										}
									}
								}
								break;
							}
						}
						break;
					}
					if (x >= 537.5&&x <= 612.5&&y >= 225 && y <= 275) {			/*skipボタン*/
						//printf("(%d,%d)click!\n",x,y);
						if (flag_end == 0) {
							if (check()) {
								turn++;
								cursor(x, y);
								glutPostRedisplay();
							}
						}
					}/*skipボタン終わり*/
					if (x >= 537.5&&x <= 612.5&&y >= 300 && y <= 350) {			/*リスタートボタン*/
						printf("New game!\n");
						restart();
						break;
					}
					if (x >= 537.5&&x <= 612.5&&y >= 375 && y <= 425) {			/*終了ボタン*/
						exit(0);
						break;
					}
				}
				flag_L = 0;
				glutPostRedisplay();
			}
		}
		break;
	case GLUT_DOWN:
		/*		if (check() == 1) {
					if (x >= 537.5&&x <= 612.5&&y >= 225 && y <= 275) {
						botton_state = 2;
					}
					else {
						botton_state = 1;
					}
					glutPostRedisplay();
				}
				break;
			*/default:
				break;
	}
}

void BottonMotion(int x, int y) {
	int IMG_X = glutGet(GLUT_WINDOW_WIDTH);
	int IMG_Y = glutGet(GLUT_WINDOW_HEIGHT);
	if (turn % 2) {
		cursor_color = 1;
	}
	else {
		cursor_color = 2;
	}
	double fixed_x = 0, fixed_y = 0;
	fixed_x = ((double)x - (double)(IMG_X / 2.0)) / (double)(IMG_X / 2.0);
	fixed_y = ((double)(500 - y) - (double)(IMG_Y / 2.0)) / (double)(IMG_Y / 2.0);
	//	printf("x=%d,y=%d,fixedx=%f,fixedy=%f\n", x, y, fixed_x, fixed_y);
	cursor_x = fixed_x;
	cursor_y = fixed_y;
	if (check() == 1) {
		if (x >= 537.5&&x <= 612.5&&y >= 225 && y <= 275) {
			botton_state = 2;
		}
		else {
			botton_state = 1;
		}
	}
	if (x >= 537.5&&x <= 612.5&&y >= 300 && y <= 350) {
		restart_botton_state = 2;
	}
	else {
		restart_botton_state = 1;
	}
	glutPostRedisplay();
}

void DrawCircle(double r, int color, double x, double y) {
	int i;
	if (color == 1) {
		glColor3d(0, 0, 0);		/*黒*/
	}
	else if (color == 2) {
		glColor3d(1, 1, 1);		/*白*/
	}
	else if (color == 0) {
		glColor3d(0, 0.7, 0.2);	/*緑*/
	}
	else if (color == 3) {
		glColor3d(130.0 / 256.0, 130.0 / 256.0, 130.0 / 256.0);	/*黒ターン*/
	}
	else if (color == 4) {
		glColor3d(190.0 / 256.0, 190.0 / 256.0, 190.0 / 256.0);	/*白ターン*/
	}
	else if (color == -1) {
		glColor3d(0, 0.6, 0.17);	/*黒影*/
	}
	else if (color == -2) {
		glColor3d(0, 0.8, 0.23);	/*白影*/
	}
	glBegin(GL_POLYGON);
	for (i = 1; i <= 200; i++) {
		glVertex2d(x + RateDisplay * r*cos(i*M_PI / 100), y + r * sin(i*M_PI / 100));
	}
	glEnd();
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		if (flag_end == 0) {
			if (check()) {
				turn++;
				cursor(x, y);
				glutPostRedisplay();
			}
		}
		break;
	case 'r':
		if (flag_restart) {
			printf("new game!\n");
			restart();
		}
		break;
	default:
		break;
	}
}

void delay(int value) {
	if (flag_end != 0) {
		delay_counter++;
		if (delay_counter > 1) {
			glutPostRedisplay();
		}
		glutTimerFunc(300, delay, 2);
	}
}

void finish_action(int z) {
	if (delay_counter > 1) {					/*終了演出*/
		if (finish_num_black > 0) {
			temp_board[finish_count1++] = 1;
			finish_num_black--;
		}
		if (finish_num_white > 0) {
			temp_board[63 - (finish_count2++)] = 2;
			finish_num_white--;
		}
		int i, j;
		for (i = 1; i < 9; i++) {
			for (j = 1; j < 9; j++) {
				board[i][j] = temp_board[(i - 1) * 8 + (j - 1)];
			}
		}
		if (finish_num_black == 0 && finish_num_white == 0) {
			flag_finish_count = 1;
		}
		glutPostRedisplay();
		glutTimerFunc(100, finish_action, 1);
	}
}

void CreateSkipBotton(void) {
	if (botton_state == 1) {
		glColor3d(180.0 / 256.0, 180.0 / 256.0, 180.0 / 256.0);
	}
	else if (botton_state == 2) {
		glColor3d(120.0 / 256.0, 120.0 / 256.0, 120.0 / 256.0);
	}
	glBegin(GL_QUADS);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, 0.1);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.1);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.1);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, 0.1);
	glEnd();

	if (botton_state == 1) {
		glColor3d(100.0 / 256.0, 100.0 / 256.0, 100.0 / 256.0);
	}
	else if (botton_state == 2) {
		glColor3d(150.0 / 256.0, 150.0 / 256.0, 150.0 / 256.0);
	}
	glBegin(GL_LINE_LOOP);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, 0.1);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.1);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.1);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, 0.1);
	glEnd();

	if (botton_state == 1) {
		RenderString((float)((7.0 / 13.0) + (6.0 / 13.0)*1.55 / 4.0), (float)-0.02, "Skip", 2);
	}
	else if (botton_state == 2) {
		RenderString((float)((7.0 / 13.0) + (6.0 / 13.0)*1.55 / 4.0), (float)-0.02, "Skip", 3);
	}
	glutPostRedisplay();
}

void CreateRestartBotton(void) {
	if (restart_botton_state == 1) {
		glColor3d(180.0 / 256.0, 180.0 / 256.0, 180.0 / 256.0);
	}
	else if (restart_botton_state == 2) {
		glColor3d(120.0 / 256.0, 120.0 / 256.0, 120.0 / 256.0);
	}
	glBegin(GL_QUADS);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.2);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.4);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.4);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.2);
	glEnd();

	if (restart_botton_state == 1) {
		glColor3d(100.0 / 256.0, 100.0 / 256.0, 100.0 / 256.0);
	}
	else if (restart_botton_state == 2) {
		glColor3d(150.0 / 256.0, 150.0 / 256.0, 150.0 / 256.0);
	}
	glBegin(GL_LINE_LOOP);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.2);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.4);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.4);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.2);
	glEnd();

	if (restart_botton_state == 1) {
		RenderString((float)((7.0 / 13.0) + (6.0 / 13.0)*1.25 / 4.0), (float)-0.32, "Restart", 2);
	}
	else if (restart_botton_state == 2) {
		RenderString((float)((7.0 / 13.0) + (6.0 / 13.0)*1.25 / 4.0), (float)-0.32, "Restart", 3);
	}
}

void CreateFinishBotton(void) {
	glColor3d(180.0 / 256.0, 180.0 / 256.0, 180.0 / 256.0);
	glBegin(GL_QUADS);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.5);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.7);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.7);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.5);
	glEnd();

	glColor3d(100.0 / 256.0, 100.0 / 256.0, 100.0 / 256.0);
	glBegin(GL_LINE_LOOP);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.5);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0) / 4.0, -0.7);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.7);
	glVertex2d((7.0 / 13.0) + (6.0 / 13.0)*3.0 / 4.0, -0.5);
	glEnd();

	RenderString((float)((7.0 / 13.0) + (6.0 / 13.0)*1.58 / 4.0), (float)-0.62, "Exit", 2);
}

void CreateOption(void) {
	glColor3d(165.0 / 256.0, 165.0 / 256.0, 165.0 / 256.0);
	glBegin(GL_QUADS);
	glVertex2d(1.0, 1.0);
	glVertex2d(1.0, -1.0);
	glVertex2d(7.0 / 13.0, -1.0);
	glVertex2d(7.0 / 13.0, 1.0);
	glEnd();
	//	DrawCircle(0.08, 1, (7.0 / 13.0) + (6.0 / 13.0) / 4.0, 0.75);
	//	DrawCircle(0.08, 2, (7.0 / 13.0) + (6.0 / 13.0) / 4.0, 0.5);
	if (turn % 2) {
		DrawCircle(0.08, 1, (7.0 / 13.0) + (6.0 / 13.0)*1.2 / 4.0, 0.75);
		DrawCircle(0.08, 4, (7.0 / 13.0) + (6.0 / 13.0)*2.7 / 4.0, 0.75);
	}
	else {
		DrawCircle(0.08, 3, (7.0 / 13.0) + (6.0 / 13.0)*1.2 / 4.0, 0.75);
		DrawCircle(0.08, 2, (7.0 / 13.0) + (6.0 / 13.0)*2.7 / 4.0, 0.75);
	}
	CreateSkipBotton();
	CreateRestartBotton();
	CreateFinishBotton();
}

int pos_x[64] = {};
int pos_y[64] = {};

int random(int num) {
	int r;
	srand((unsigned)time(NULL));
	r = rand();
	return (r % num);
}
int com_num = 0;
void com() {
	int possib = 0;
	//	int num=0;
	int i, j;
	if (turn % 2 == 0 && turn > 0) {
		if (check() == 0) {
			//			printf("step1 OK!\n");
			for (i = 1; i < 9; i++) {
				//				printf("step2 OK!\n");
				for (j = 1; j < 9; j++) {
					//					printf("step3 OK!\n");
					if (board[i][j] == -2) {
						//						printf("step4 OK!\n");
						pos_x[com_num] = i;
						pos_y[com_num] = j;
						//						printf("pos_x=%d,pos_y=%d\n",pos_x[com_num],pos_y[com_num]);
						com_num++;
					}
				}
			}
		}
		else {
			turn++;
			glutPostRedisplay();
		}
		//		printf("%d\n", com_num);
		if (com_num > 0) {
			if (reverse(pos_x[0], pos_y[0], 2)) {
				turn++;
				com_num = 0;
				finish();
				glutPostRedisplay();
			}
		}
		else {
			return;
		}
	}
}

void display() {
	if (turn > 0 && last_turn != turn) {
		if (turn % 2) {
			printf("Black's turn\n");
		}
		else {
			printf("White's turn\n");
		}
		last_turn = turn;
	}
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(0.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINES);
	for (i = 1; i < 8; i++) {
		glVertex2d(-1 + RateDisplay * 0.25*i, -1);
		glVertex2d(-1 + RateDisplay * 0.25*i, 1);
		glVertex2d(-1, -1 + 0.25*i);
		glVertex2d(-1.0 + 2.0*RateDisplay, -1 + 0.25*i);
	}
	glEnd();
	if (flag) {
		board[4][4] = 2;
		board[5][5] = 2;
		board[4][5] = 1;
		board[5][4] = 1;
		flag = 0;
	}
	for (i = 1; i < 9; i++) {
		for (j = 1; j < 9; j++) {
			if (turn % 2) {
				possibility(i, j, 1);
			}
			else {
				possibility(i, j, 2);
			}
			//com();	/*コンピュータ用*/
			DrawCircle(0.1, board[i][j], -1 + RateDisplay * 0.125 + RateDisplay * 0.25*(j - 1), 1 - 0.125 - 0.25*(i - 1));
		}
	}
	DrawCircle(0.05, cursor_color, cursor_x, cursor_y);
	if (flag_end != 0) {
		delay(2);
		if (flag_once1&&delay_counter > 1) {
			finish_action(1);
			flag_once1 = 0;
		}
		if (flag_finish_count) {
			if (flag_end == 2) {
				RenderString(str_pos_x + (-0.005f), str_pos_y + 0.0f, "Winner Black", 1);
				RenderString(str_pos_x + 0.005f, str_pos_y + 0.0f, "Winner Black", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + 0.005f, "Winner Black", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + (-0.005f), "Winner Black", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + 0.0f, "Winner Black", 0);
				if (flag_once2) {
					flag_animation = 1;
					animation(0);
					flag_once2 = 0;
				}
			}
			else if (flag_end == 1) {
				RenderString(str_pos_x + (-0.005f), str_pos_y + 0.0f, "Winner White", 1);
				RenderString(str_pos_x + 0.005f, str_pos_y + 0.0f, "Winner White", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + (-0.005f), "Winner White", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + 0.005f, "Winner White", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + 0.0f, "Winner White", 0);
				if (flag_once2) {
					flag_animation = 1;
					animation(0);
					flag_once2 = 0;
				}
			}
			else if (flag_end == -1) {
				RenderString(str_pos_x + (-0.005f), str_pos_y + 0.0f, "Draw", 1);
				RenderString(str_pos_x + 0.005f, str_pos_y + 0.0f, "Draw", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + (-0.005f), "Draw", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + 0.005f, "Draw", 1);
				RenderString(str_pos_x + 0.0f, str_pos_y + 0.0f, "Draw", 0);
				if (flag_once2) {
					flag_animation = 1;
					animation(0);
					flag_once2 = 0;
				}
			}
		}
	}
	if (flag_restart) {
		RenderString(-0.4f + (-0.005f), -0.97f, "Please press 'R' to start a new game.", 1);
		RenderString(-0.4f + 0.005f, -0.97f, "Please press 'R' to start a new game.", 1);
		RenderString(-0.4f, -0.97f + (-0.005f), "Please press 'R' to start a new game.", 1);
		RenderString(-0.4f, -0.97f + 0.005f, "Please press 'R' to start a new game.", 1);
		RenderString(-0.4f, -0.97f, "Please press 'R' to start a new game.", 0);
	}
	CreateOption();
	if (check()) {
		botton_state = 1;
	}
	else {
		botton_state = 2;
	}

	glutSwapBuffers();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w - 0.5,
		(GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(650, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(TITLE);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	//	glutReshapeFunc(resize);

	glutPassiveMotionFunc(cursor);
	//	glutMotionFunc(cursor);
	glutMotionFunc(BottonMotion);

	glutDisplayFunc(display);
	glClearColor(0.0, 0.7, 0.2, 0.0);
	/*	if (flag_end == 2) {
			glutTimerFunc(100, finish_action, 1);
			if (flag_finish_count) {
				glutTimerFunc(1, animation, 0);
			}
		}
		else if (flag_end == 1) {
			glutTimerFunc(100, finish_action, 1);
			if (flag_finish_count) {
				glutTimerFunc(1, animation, 0);
			}
		}
		*/
	glutMainLoop();
	return 0;
}
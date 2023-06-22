#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAXSIZE 100
#define OK 1
#define ERROR 0

//��ŵ�ǰ����
struct Position {
	int x;
	int y;
};

//�������
struct Position Enter, Exit;

//������ �� �� �� �ĸ�����
struct Position Direction[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};


struct Elment {
	int count;       //�߹��Ĳ���
	Position nseat;  //��ǰλ������
	int dir;         //����
};


//�Թ�����
typedef int Maze[MAXSIZE][MAXSIZE];


//�Թ�������������
int x, y;


int step = 1;

//����������
typedef struct node {
	Elment data;
	struct node *next;
} Stacknode;



//����ջ����
typedef struct stack {
	Stacknode *top;
	Stacknode *base;
} Stack;


//ջ��ʼ��
void CreateStack(Stack &s) {
	s.base = (Stacknode *)malloc((sizeof(Stacknode)));

	if (!s.base) exit(1);

	s.top = s.base;

	s.top->next = NULL;
}


//��ջ
void PushStack(Stack &s, Elment &num) {
	Stacknode *p;
	p = (Stacknode *)malloc(sizeof(Stacknode));
	p->data = num;
	p->next = s.top->next;
	s.top->next = p;
}


//��ջ
int PopStack(Stack &s, Elment &num) {
	if (s.top->next == NULL) {
		printf("ջ�Ѿ��ǿ�ջ\n");
		return OK;
	}
	Stacknode *p;
	p = s.top->next;
	num = p->data;
	s.top->next = p->next;
	free(p);

	return 1;
}

//�ж�ջΪ��
int Isempty(Stack &s) {
	return (s.top->next == NULL);

}

void DestoryStack(Stack &s) {
	while (s.top) {
		s.base = s.top->next;
		free(s.top);
		s.top = s.base;
	}
	printf("���ٳɹ�");

}

//��ջ�����ջ
void DispStack(Stack &s) {
	Stacknode *p;
	p = s.top->next;
	while (p) {
		printf(" (%d , %d , %d)\t", p->data.nseat.x, p->data.nseat.y, p->data.dir);
		p = p->next;
	}

}

//��ӡ�Թ�
void PrintMaze(Maze &m) {
	int i, j;
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			printf("  %d  ", m[i][j]);
		}
		printf("\n");
		printf("\n");
	}

}


//�Զ����Թ�
void Diy(Maze &m) {
	int i, j;
	printf("�������Թ�������������������Χǽ��:\n");
	scanf("%d,%d", &x, &y);
	for (i = 0; i < x; i++) {
		m[i][0] = 1;
		m[i][y - 1] = 1;
	}
	for (j = 0; j < y; j++) {
		m[0][j] = 1;
		m[x - 1][j] = 1;
	}
	for (i = 1; i < x - 1; i++) {
		for (j = 1; j < y - 1; j++)
			m[i][j] = 0;
	}
	printf("�������ϰ��������:\n");
	int num;
	scanf("%d", &num);
	int a, b;
	printf("�������ϰ��������������(��ò�Ҫ��ͬһλ���ظ�����):\n");
	for (i = 0; i < num; i++) {
		scanf("%d,%d", &a, &b);
		if (!(a > 0 && a < x - 1 && b > 0 && b < y - 1)) {
			printf("��%d�����벻�淶���벻Ҫ���ϰ������Χǽ�ϻ򳬳��Թ���ΧŶ��������\n", i + 1);
			num++;
			continue;
		} else
			m[a][b] = 1;
	}
	printf("�Թ�����ʽ:\n");
	printf("\n");
	PrintMaze(m);
	printf("�������������,����:\n");
	scanf("%d,%d", &a, &b);
	int flag = 0;
	while (!flag) {
		if (!(a > 0 && a < x - 1 && b > 0 && b < y - 1)) {
			printf("���벻�淶��ע����ΧǽŶ��������\n");
			scanf("%d,%d", &a, &b);
		} else if (m[a][b] == 1) {
			printf("���벻�淶����λ�����ϰ��������\n");
			scanf("%d,%d", &a, &b);
		}

		else
			flag = 1;
	}
	Enter.x = a;
	Enter.y = b;
	printf("�������������,����:\n");
	scanf("%d,%d", &a, &b);
	int flag1 = 0;
	while (!flag1) {
		if (!(a > 0 && a < x - 1 && b > 0 && b < y - 1)) {
			printf("���벻�淶��ע����ΧǽŶ��������\n");
			scanf("%d,%d", &a, &b);
		} else if (m[a][b] == 1) {
			printf("���벻�淶����λ�����ϰ��������\n");
			scanf("%d,%d", &a, &b);
		} else
			flag1 = 1;
	}
	Exit.x = a;
	Exit.y = b;

}




//��¼·��
void FootPrint(Position p, Maze &m) {
	m[p.x][p.y] = step;

}



//���Է���
void Trypos(Position &p, int dir) {
	p.x = p.x + Direction[dir].x;
	p.y = p.y + Direction[dir].y;

}

//��ǰλ���ܷ�ͨ����0����1��
int ConPass(Position p, Maze &m) {
	if (m[p.x][p.y] == 0) {
		return 1;
	} else {
		return 0;
	}

}

void Walk(Stack &p, Maze &m, Elment &k, Position &g) {

	FootPrint(g, m);
	k.dir = 0;
	k.count = step;
	k.nseat = g;
	PushStack(p, k);
	step++;

}

void Trywalk(Stack &p, Elment &k, Position &g) {
	if (!Isempty(p)) {

		PopStack(p, k);
		step--;
		if (k.dir < 3) {
			k.dir++;
			PushStack(p, k);
			step++;
			g = k.nseat;
			Trypos(g, k.dir);
		} else {
			while (k.dir == 3 && (!Isempty(p))) {
				PopStack(p, k);
				step--;
				k.dir--;
			}
		}

	}
}


//���Թ�
int MazeThrough(Position Enter, Stack &p, Maze &m, Elment &k, Position &g) {
	g = Enter;
	do {
		if (ConPass(g, m)) {
			Walk(p, m, k, g);
			if (g.x == Exit.x && g.y == Exit.y) {
				return 1;
			} else {
				Trypos(g, k.dir);
			}
		} else {
			Trywalk(p, k, g);

		}
	} while (!Isempty(p));
	return 0;

}


//��ӡ��ջ�����Թ���ͨ·����������ջ�ĵ���ʽ
void StackPrint(Stack &s) {
	Stack t;
	Elment num;
	CreateStack(t);
	Stacknode *q;
	q = s.top->next;
	while (q) {
		PopStack(s, num);
		q = q->next;
		PushStack(t, num);
	}
	DispStack(t);
	printf("\n") ;

}

int main() {
	Maze m;
	Stack p;
	Elment k;
	Position g;
	CreateStack(p);
	Diy(m);
	if (MazeThrough(Enter, p, m, k, g)) {
		printf("�Թ�ͨ·Ϊ��\n");
		printf("\n");
		PrintMaze(m);
		printf("\n'��','��','��','��',�����Ϊ'0','1','2','3'\n") ;
		printf("\n");
		StackPrint(p);
	} else {
		PrintMaze(m);
		printf("û��ͨ·\n");
	}
	return 0;

}

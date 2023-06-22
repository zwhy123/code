#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAXSIZE 100
#define OK 1
#define ERROR 0

//存放当前坐标
struct Position {
	int x;
	int y;
};

//出口入口
struct Position Enter, Exit;

//定义右 下 左 上 四个方向
struct Position Direction[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};


struct Elment {
	int count;       //走过的步数
	Position nseat;  //当前位置坐标
	int dir;         //方向
};


//迷宫数组
typedef int Maze[MAXSIZE][MAXSIZE];


//迷宫的行数和列数
int x, y;


int step = 1;

//定义结点类型
typedef struct node {
	Elment data;
	struct node *next;
} Stacknode;



//定义栈类型
typedef struct stack {
	Stacknode *top;
	Stacknode *base;
} Stack;


//栈初始化
void CreateStack(Stack &s) {
	s.base = (Stacknode *)malloc((sizeof(Stacknode)));

	if (!s.base) exit(1);

	s.top = s.base;

	s.top->next = NULL;
}


//进栈
void PushStack(Stack &s, Elment &num) {
	Stacknode *p;
	p = (Stacknode *)malloc(sizeof(Stacknode));
	p->data = num;
	p->next = s.top->next;
	s.top->next = p;
}


//出栈
int PopStack(Stack &s, Elment &num) {
	if (s.top->next == NULL) {
		printf("栈已经是空栈\n");
		return OK;
	}
	Stacknode *p;
	p = s.top->next;
	num = p->data;
	s.top->next = p->next;
	free(p);

	return 1;
}

//判断栈为空
int Isempty(Stack &s) {
	return (s.top->next == NULL);

}

void DestoryStack(Stack &s) {
	while (s.top) {
		s.base = s.top->next;
		free(s.top);
		s.top = s.base;
	}
	printf("销毁成功");

}

//从栈顶输出栈
void DispStack(Stack &s) {
	Stacknode *p;
	p = s.top->next;
	while (p) {
		printf(" (%d , %d , %d)\t", p->data.nseat.x, p->data.nseat.y, p->data.dir);
		p = p->next;
	}

}

//打印迷宫
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


//自定义迷宫
void Diy(Maze &m) {
	int i, j;
	printf("请输入迷宫的行数和列数（包括围墙）:\n");
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
	printf("请输入障碍物的数量:\n");
	int num;
	scanf("%d", &num);
	int a, b;
	printf("请输入障碍物的行数和列数(最好不要在同一位置重复输入):\n");
	for (i = 0; i < num; i++) {
		scanf("%d,%d", &a, &b);
		if (!(a > 0 && a < x - 1 && b > 0 && b < y - 1)) {
			printf("第%d次输入不规范，请不要把障碍物放在围墙上或超出迷宫范围哦，请重试\n", i + 1);
			num++;
			continue;
		} else
			m[a][b] = 1;
	}
	printf("迷宫的样式:\n");
	printf("\n");
	PrintMaze(m);
	printf("请输入入口行数,列数:\n");
	scanf("%d,%d", &a, &b);
	int flag = 0;
	while (!flag) {
		if (!(a > 0 && a < x - 1 && b > 0 && b < y - 1)) {
			printf("输入不规范，注意有围墙哦，请重试\n");
			scanf("%d,%d", &a, &b);
		} else if (m[a][b] == 1) {
			printf("输入不规范，该位置是障碍物，请重试\n");
			scanf("%d,%d", &a, &b);
		}

		else
			flag = 1;
	}
	Enter.x = a;
	Enter.y = b;
	printf("请输入出口行数,列数:\n");
	scanf("%d,%d", &a, &b);
	int flag1 = 0;
	while (!flag1) {
		if (!(a > 0 && a < x - 1 && b > 0 && b < y - 1)) {
			printf("输入不规范，注意有围墙哦，请重试\n");
			scanf("%d,%d", &a, &b);
		} else if (m[a][b] == 1) {
			printf("输入不规范，该位置是障碍物，请重试\n");
			scanf("%d,%d", &a, &b);
		} else
			flag1 = 1;
	}
	Exit.x = a;
	Exit.y = b;

}




//记录路径
void FootPrint(Position p, Maze &m) {
	m[p.x][p.y] = step;

}



//尝试方向
void Trypos(Position &p, int dir) {
	p.x = p.x + Direction[dir].x;
	p.y = p.y + Direction[dir].y;

}

//当前位置能否通过（0还是1）
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


//走迷宫
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


//打印链栈，即迷宫的通路，用两个链栈的的形式
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
		printf("迷宫通路为：\n");
		printf("\n");
		PrintMaze(m);
		printf("\n'右','下','左','上',方向记为'0','1','2','3'\n") ;
		printf("\n");
		StackPrint(p);
	} else {
		PrintMaze(m);
		printf("没有通路\n");
	}
	return 0;

}

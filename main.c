#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cnt;

typedef struct Date
{
	int year, month, day;
	char eventlist[5][50]; // 최대 50자 길이의 이벤트 최대 5개 입력받기
	struct Date *prev;
	struct Date *next;
} Date;

void printMonthName(int month) // month를 전달 받아 해당 달의 영어 이름을 출력
{
	switch (month)
	{
	case 1:
		printf("January");
		break;
	case 2:
		printf("February");
		break;
	case 3:
		printf("March");
		break;
	case 4:
		printf("April");
		break;
	case 5:
		printf("May");
		break;
	case 6:
		printf("June");
		break;
	case 7:
		printf("July");
		break;
	case 8:
		printf("August");
		break;
	case 9:
		printf("September");
		break;
	case 10:
		printf("October");
		break;
	case 11:
		printf("November");
		break;
	case 12:
		printf("December");
		break;
	}
}

void printMonthTitle(int year, int month) // 월과 년도를 출력
{
	printMonthName(month);
	printf(" %d\n", year);
	printf("-----------------------------\n");
	printf(" Sun Mon Tue Wed Thu Fri Sat\n");
}

int isLeapYear(int year) // year를 전달받아 윤년인지 확인
{
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
		return 1;
	else
		return 0;
}

int getNumberOfDaysInMonth(int year, int month) // year와 month를 전달 받아 해당월에 몇일이 있는지
{
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		return 31;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		return 30;
	if (month == 2)
	{
		if (isLeapYear(year) == 1)
			return 29;
		else
			return 28;
	}
}

int getTotalNumberOfDays(int year, int month) // 1800년 1월 1일 부터 며칠이 되는지 계산
{
	int total = 0;
	for (int i = 1800; i < year; i++)
	{
		if (isLeapYear(i))
			total += 366;
		else
			total += 365;
	}
	for (int i = 1; i < month; i++)
		total = total + getNumberOfDaysInMonth(year, i);
	return total;
}

int getStartDay(int year, int month) // 해당 월의 시작하는 날이 몇 요일인지 찾는 함수
{
	int startday1800 = 3;
	int totalnumberofdays = getTotalNumberOfDays(year, month);
	return (startday1800 + totalnumberofdays) % 7;
}

void printMonthBody(int year, int month) // year와 month를 전달 받아 요일과 일을 출력
{
	int startday = getStartDay(year, month);
	int numberofdaysinmonth = getNumberOfDaysInMonth(year, month);

	for (int i = 0; i < startday; i++)
		printf("    ");
	for (int i = 1; i <= numberofdaysinmonth; i++)
	{
		printf("%4d", i);
		if ((i + startday) % 7 == 0)
			printf("\n");
	}
}

void printMonth(int year, int month) // year와 month를 전달 받아 해당 월을 출력 -> title, body
{
	printMonthTitle(year, month);
	printMonthBody(year, month);
}

void init(Date *head)
{							 // 헤드노드 초기화
	head->prev = head; // 헤드노드의 next를 head노드 주소로 저장
	head->next = head; // 헤드노드의 prev를 head노드 주소로 저장
}

void swapDate(Date *a, Date *b)
{
	a->next = b->next;
	b->prev = a->prev;
	a->prev = b;
	b->next = a;
}

void sortDate(Date *list, int size)
{
	Date *cur;
	cur = list;

	cur->year = 0;
	cur->month = 0;
	cur->day = 0;
	cur = cur->next;

	for (int i = 0; i < size + 1; i++)
	{
		if (cur->next == NULL)
			break;
		for (int j = 0; j < size - 1 - i; j++)
		{
			if (cur->year > cur->next->year)
				swapDate(cur, cur->next);
			else if (cur->year == cur->next->year)
			{
				if (cur->month > cur->next->month)
					swapDate(cur, cur->next);
				else if (cur->month == cur->next->month)
				{
					if (cur->day > cur->next->day)
						swapDate(cur, cur->next);
					// else if (cur->day == cur->next->day)
					// {

					// }
				}
			}
			cur = cur->next;
		}
		cur = list->next;
	}
}

void dayInsert(Date *before, char *event, int year, int month, int day)
{																// before 다음노드로 삽입
	Date *newday = (Date *)malloc(sizeof(Date)); // 동적할당
	strcpy(newday->eventlist[0], event);
	newday->year = year;
	newday->month = month;
	newday->day = day;
	
	newday->prev = before;		  // 이전노드 주소값 저장
	newday->next = before->next; // 이전노드의 next값 저장
	before->next->prev = newday; // 이전노드의 넥스트 노드의 prev값을 새로운 노드 주소값으로 저장
	before->next = newday;		  // 이전노드의 next값을 새로운 노드 주소값으로 저장;
}

Date *searchNode(Date *head, char *name)
{
	Date *temp = head;
	int m = 0, n = 0;
	while (m <= cnt)
	{
		for (int i = 0; i < 5; i++)
		{
			if (strcmp(temp->eventlist[i], name) == 0)
			{
				printf("\n%04d.%02d.%02d\n", temp->year, temp->month, temp->day);
				printf("    %s", name);
				n++;
			}
		}
		temp = temp->next; /*다음 주소로 이동*/
		m++;
	}
	if (n == 0)
		return NULL;
}

void eventDelete(Date *head, Date *removed)
{
	if (removed == head)
		return;
	removed->prev->next = removed->next; // 삭제할 노드의 next값으로 저장;
	removed->next->prev = removed->prev; // 삭제노드의 prev값으로 저장;
	free(removed);
}

void printDlist(Date *head)
{
	Date *p;
	for (p = head->next; p != head; p = p->next)
	{ // 초기값:head->next, head위치가 아니면 출력
		printf("\n%04d.%02d.%02d  %s", p->year, p->month, p->day, p->eventlist[0]);
	}
	printf("\n");
}

void menu()
{
	printf("\n-----------------------------\n");
	printf("\n\t1 : 달력 출력\n");
	printf("\n\t2 : 이벤트 추가\n");
	printf("\n\t3 : 이벤트 삭제\n");
	printf("\n\t4 : 이벤트 검색\n");
	printf("\n\t5 : 이벤트 전체 조회\n");
	printf("\n\t6 : 종료\n");
	printf("\n-----------------------------\n");
	printf("\n메뉴 입력 >> ");
}

int main()
{
	Date *head = (Date *)malloc(sizeof(Date)); // 헤드노드 동적할당
	init(head);											 // 헤드노드 초기화
	int year, month, day, choice;
	char *eventname;
	eventname = (char *)calloc(50, sizeof(char));
	while (1)
	{
		menu();
		scanf(" %d", &choice);
		switch (choice)
		{
		case 1: // 달력출력
			printf("\t[달력 출력]\n");
			printf(" year : ");
			scanf("%d", &year); // 년도 입력
			printf("month : ");
			scanf("%d", &month); // 월 입력
			printMonth(year, month);
			break;

		case 2: // 이벤트 추가
			printf("\t[이벤트 추가]\n");
			printf(" year : ");
			scanf("%d", &year); // 년도 입력
			printf("month : ");
			scanf("%d", &month); // 월 입력
			printf("  day : ");
			scanf("%d", &day); // 월 입력
			printf("event : ");
			scanf("%s", eventname); // 이벤트 명 입력
			dayInsert(head, eventname, year, month, day);
			cnt++;
			printDlist(head);
			break;

		case 3: // 이벤트 삭제
			printf("삭제할 이벤트를 입력하세요 : ");
			break;

		case 4: // 이벤트 검색 (이벤트 이름 입력)
			printf("검색할 이벤트를 입력하세요 : ");
			scanf("%s", eventname);
			// Date *p = searchNode(head,eventname);
			if (searchNode(head, eventname) == NULL)
				printf("\n그딴건 없다");
			break;

		case 5: // 이벤트 전체 조회
			// sortDate(head, cnt);
			printDlist(head);
			break;

		case 6:
			return 0;

		default:
			printf("없는 메뉴입니다. 메뉴를 다시 선택하세요!\n");
			break;
		}
	}
	return 0;
}
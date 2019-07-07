#include <stdio.h>
#include "display.h"

void display_init(int num)
{
	printf("*********欢迎使用学生信息管理系统（当前共有%02d名学生）*********\n", num);
	printf("*                                                            *\n");
	printf("*            ------------------------------                  *\n");
	printf("*            | Powered By c.biancheng.net |                  *\n");
	printf("*            ------------------------------                  *\n");
	printf("*                                                            *\n");
	printf("*            1)添加学生信息                                  *\n");
	printf("*            2)删除学生信息                                  *\n");
	printf("*            3)修改学生信息                                  *\n");
	printf("*            4)查询学生信息                                  *\n");
	printf("*            5)显示所有学生信息以及统计信息                  *\n");
	printf("*            0)退出软件                                      *\n");
	printf("*                                                            *\n");
	printf("**************************************************************\n");
}

void display_add_stu(STUDENT *stu)
{
	printf("%s", "输入学号：");
	scanf("%d", &stu->id);
	printf("%s", "输入姓名：");
	scanf("%s", stu->name);
	again:
	printf("%s", "输入性别：");
	scanf("%s", stu->sex);
	if(!strcmp(stu->sex, "男") || !strcmp(stu->sex, "女")) {
	}
	else {
		printf("错误：性别只能是男或女！\n");
		goto again;
	}
	printf("%s", "输入年龄：");
	scanf("%d", &stu->age);
	again_1:
	printf("%s", "数学成绩：");
	scanf("%f", &stu->grade.math);
	if(stu->grade.math > 150.0 || stu->grade.math < 0.0) {
		fprintf(stderr, "错误：数学成绩的取值范围为0~150！\n");
		goto again_1;
	}
	again_2:
	printf("%s", "语文成绩：");
	scanf("%f", &stu->grade.chinese);
	if(stu->grade.chinese > 150.0 || stu->grade.chinese < 0.0) {
		fprintf(stderr, "错误：语文成绩的取值范围为0~150！\n");
		goto again_2;
	}
	again_3:
	printf("%s", "英语成绩：");
	scanf("%f", &stu->grade.english);
	if(stu->grade.english > 150.0 || stu->grade.english < 0.0) {
		fprintf(stderr, "错误：英语成绩的取值范围为0~150！\n");
		goto again_3;
	}
}

void display_error_info(void)
{
	printf("错误：该学生信息已存在，无需重复添加！按任意键返回...\n");
}

void dispaly_success_info(void)
{
	printf("提示：添加成功！按任意键返回...");
	//pause();
	do
	{
		getchar();
	}while(getchar() != 10);//man ascii
}

void display_info(int sum)
{
	printf("共有%02d条学生信息，按任意键返回...", sum);
	do
	{
		getchar();
	}while(getchar() != 10);//man ascii
}





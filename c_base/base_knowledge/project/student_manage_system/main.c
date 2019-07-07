#include "display.h"
#include "database.h"
#include "file.h"

#include <unistd.h>

int main(int argc, char const *argv[])
{
	int selection;
	STUDENT *stu = NULL;
	int sum = 0;

	init_list();
	display_init(sum);
	scanf("%d", &selection);

	while(1){
		switch(selection) {
		case 1:
			stu = (STUDENT *)malloc(sizeof(STUDENT));
			if(!stu) {
				fprintf(stderr, "%s: malloc error\n", __func__);
				return -1;
			}
			display_add_stu(stu);
			add_stu(stu);
			dispaly_success_info();
			sum += 1;
			system("clear");
			display_init(sum);
			scanf("%d", &selection);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			system("clear");
			display_all_stu(stu);
			display_info(sum);
			system("clear");
			display_init(sum);
			scanf("%d", &selection);
			break;
		case 0:
			exit(0);
			break;
		default:
			printf("%s\n", "对不起，不支持此项服务");
			break;
		}
	}

	pause();

	return 0;
}



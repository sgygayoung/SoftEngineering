#include<stdio.h>
#include<string.h>
#include<time.h>
#include<Windows.h>

#define MAX_INK 3000
#define MAX_PAPER 100
char* Userdata = "User.txt"; //User database text file
char* AmountOfPaper = "paper.txt";
char* AmountOfInk = "ink.txt";

typedef struct {
	char* user; // user name
	int RP; // required paper
	int RI; // required Ink
	int now; // amount of printed papar;
	char* path; // file path
}PDATA; // print data struct
PDATA Queue[5];
int top_queue = 0; //Top on Queue
int end_queue = 0; //end on Queue

struct STATUS
{
	int UM;
	int PM;
	int RM;
	int CP;
	int SUR;
	int isPrintRunning;
	int isCharging;
};

struct STATUS status;

//Display Interface
typedef struct {
	char message[1000];
	int ink;
	int paper;
	char* printData;
	char time[10];
	char* status;
	char* content[5]; //waited print data max 5
	char* userList[5];
}DDATA;
DDATA now_display;
typedef enum{ PRINTING = 0, WAITING, RECHARGING } STATUS;

void Display(){
	int i;
	//while (1){
		//system("alias clear='printf "\033c"'");
		system("clear");
		time_t timer;
		struct tm *t;
		timer = time(NULL);
		t = localtime(&timer);
		sprintf(now_display.time, "%2d:%2d", t->tm_hour, t->tm_min);
		printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
		printf("                    Display                \n");
		printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
		printf("  Remain Paper  弛%3d              \n", now_display.paper);
		printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
		printf("  Remain Ink    弛%4d             \n", now_display.ink);
		printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
		printf("  Time          弛%s            \n", now_display.time);
		printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
		printf("  Status        弛%s\n", now_display.status);
		if (strcmp("printing", now_display.status) == 0){
			printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
			printf("                    Print                \n");
			printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
			printf("  Printing      弛");
			printf("%s %d/%d   \n", Queue[top].user, Queue[top].now, Queue[top].ep);
			printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
			printf("  Wait Queue    弛");
			if (end - 1 > top + 1){ //wait more than one
				for (i= top + 1; i < end - 1; i++)
					printf("%s %d,", Queue[i].user, Queue[i].ep);
				printf("%s %d\n", Queue[end - 1].user, Queue[end - 1].ep);

			}
			else if (end - 1 == top + 1){ //waiting only one
				printf("%s %d\n", Queue[end - 1].user, Queue[end - 1].ep);
			}
			else //no wait
				printf("\n");
			printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
		}
		else{
			printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
		}
		printf("    %s\n", now_display.message);
		//Sleep(1000); //1min
	//}
}
int CommandDistributor(char* command){

}//Print Manager
void PreProcess(char* command[2]) {
	status.PM = 1;
	FILE* cFile = NULL;
	cFile = fopen(command[1], "r");
	int RP = CountPaper(cFile);
	int RI = CountInk(cFile, RP);
	//printf("Preprocess ep: %d, ei: %d\n", ep, ei);

	PDATA pd;
	pd.user = command[0];
	if (RP % 10 == 0) {
		pd.RP = RP / 10;
	}
	else {
		pd.RP = RP / 10 + 1;
	}
	pd.RI = RI;
	pd.now = 0;
	pd.path = command[1];
	CheckPrintData(pd);
}
int CountPaper(FILE* cFile) {
	int count = 0;
	char strBuf[255];
	if (cFile != NULL) {
		while (fgets(strBuf, sizeof(strBuf), cFile)) {
			count++;
		}
	}
	return count;
}
int CountInk(FILE* cFile, int pline) {
	int ink = 0;
	int i = 0;
	int j = 0;
	char strBuf[255];

	for (i = 0; i < pline; i++) {
		if (fgets(strBuf, sizeof(strBuf), cFile)) {
			for (j = 0; j < 31; j++) {
				if (strBuf[j] == 13 || strBuf[j] == 10 || strBuf[j] == '\0') {
					break;
				}
				if (strBuf[j] != ' ') {
					ink++;
				}
			}
		}
	}
	fclose(cFile);
	return ink;
}
void CheckPrintData(PDATA pd) {
	int EP;
	int EI;
	FILE* iFile = NULL;
	FILE* pFile = NULL;
	iFile = fopen(AmountOfInk, "r");
	pFile = fopen(AmountOfPaper, "r");
	fscanf(pFile, "%d", &EP);
	fscanf(iFile, "%d", &EI);
	fclose(pFile);
	fclose(iFile);
	if (EP >= pd.RP && EI >= pd.RI) {
		if (end_queue < 5) { // this file can be printed
			Queue[end_queue++] = pd;
			// Process and Data Lobby part

			// Process and Data Lobby part
		}
		else {
			printf("Print Queue is full");
		}
	}
	else if (EP < pd.RP && EI < pd.RI) {
		printf("paper and ink low");
	}
	else if (EP < pd.RP) {
		printf("paper low");
	}
	else if (EI < pd.RI) {
		printf("ink low");
	}
}
void DataLobby(){

}
int RejectPrint(){

}

//User Manager
int AddUser(char* id){
	FILE* uFile = NULL;
	uFile = fopen(Userdata, "a");
	if (uFile == NULL){
		sprintf(now_display.message,"%s is unknown.", Userdata);
	}
	else{
		fprintf(uFile, id);
		sprintf(now_display.message, "%s is succesfully registered.", id);
	}
	fclose(uFile);
	return 1;
}
int DelUser(char* id){
	FILE* uFile = NULL, *copy_file = NULL;
	char userid[255];
	uFile = fopen(Userdata, "r");
	copy_file = fopen("copy_userdata.txt", "w");
	if (uFile == NULL){
		sprintf(now_display.message, "%s is unknown.", Userdata);
	}
	else{
		while (!feof(uFile)){
			fscanf(uFile, "%s\n", userid);
			if (strcmp(userid, id) != 0){
				fprintf(copy_file, "%s\n", userid);
			}
		}
	}
	fclose(uFile);
	fclose(copy_file);
	copy_file = fopen("copy_userdata.txt", "r");
	uFile = fopen(Userdata, "w");
	while (!feof(copy_file)){
		fscanf(copy_file, "%s\n", userid);
		fprintf(uFile, "%s\n", userid);
	}
	sprintf(now_display.message, "%s is succesfully deleted.", id);
	fclose(uFile);
	fclose(copy_file);
	return 1;
}
int RejectUser(){
	printf("Reject command!");
	return 1;
}
void ShowUserList(){
	FILE* uFile = NULL;
	uFile = fopen(Userdata, "r");
	int count = 1;
	char userid[255];
	char display[1000] = "  *UserList";
	if (uFile == NULL){
		sprintf(now_display.message, "%s is unknown.", Userdata);
	}
	else{
		while (!feof(uFile)){
			fscanf(uFile, "%s\n", userid);
			sprintf(display,"%s\n   %d : %s", display,count++,userid);
		}
		sprintf(now_display.message, display);
	}
	
	fclose(uFile);
}
int UserManageInterface(char* command[2]){
	FILE* uFile = NULL;
	int hasUserList = 0;
	char userid[255];
	if (strcmp(command[0], "Reg") == 0 || strcmp(command[0], "Del") == 0){
		uFile = fopen(Userdata, "r");
		if (uFile == NULL){
			sprintf(now_display.message, "%s is unknown.", Userdata);
			return 0;
		}
		else{
			while (!feof(uFile)){
				fscanf(uFile, "%s\n", userid);
				if (strcmp(userid, command[1]) == 0){
					hasUserList = 1;
					break;
				}
			}
		}
		fclose(uFile);
	}
	if (strcmp(command[0], "Reg") == 0){
		if (hasUserList == 1){
			sprintf(now_display.message, "%s is already registered.\n", command[1]);
			RejectUser();
			return 0;
		}
		AddUser(command[1]);
	}
	else if (strcmp(command[0], "Del") == 0){
		if (hasUserList == 0){
			sprintf(now_display.message, "%s is not exist in User database.\n", command[1]);
			RejectUser();
			return 0;
		}
		DelUser(command[1]);
	}
	else if (strcmp(command[0], "Show") == 0 && strcmp(command[1], "UserList") == 0){
		ShowUserList();
	}
	else{
		sprintf(now_display.message, "Wrong Command.Try Again.\n(ex.\"Admin Reg UserA\" or \"Admin Del UserA\"");
	}
}

//Recharge Manager
void RechargeManageInterface(char* command){

}
int AddPaper(int p){

}
int AddInk(int i){

}
int RejectRecharge(){

}

//Print Interface
//void PrintInterface(char* filename);
int main(void){
	int i;
	now_display.ink = 3000;
	now_display.paper = 100;
	now_display.printData = "";
	now_display.status = "waiting";
	Display();
	Sleep(1000);
	//int a = pthread_create(&Dthread, NULL, Display, NULL);
	//pthread_join(re, &s);

	now_display.status = "printing";
	Queue[0].ep = 5;
	Queue[0].now = 0;
	Queue[0].user = "user1";
	top = 0;
	end = 1;
	for (i = 1; i < 4; i++){
		Queue[i].ep = i+5;
		Queue[i].now = 0;
		Queue[i].user = "user";
		end++;
	}
	char* test_command[2];
	test_command[0] = "Show";
	test_command[1] = "UserList";
	UserManageInterface(test_command);
	Display();
	Sleep(1000);
	test_command[0] = "Reg";
	test_command[1] = "User5";
	UserManageInterface(test_command);
	Display();
	Sleep(1000);
	test_command[0] = "Del";
	test_command[1] = "User5";
	UserManageInterface(test_command);
	Display();
	Sleep(1000);
	
	return;
}

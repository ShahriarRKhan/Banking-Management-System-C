#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#define MAX_ACCOUNT_NUMBER 9999
#define MIN_ACCOUNT_NUMBER 1000

struct user_account {
    char usr_name[30] ;
    int id;
    float balance;
    char password[8];
} user, check;

struct Admin {
    int id_admin;
    char pass_admin[8] ;
} admin,ad_check;

char* timeDate() {
    static char buffer[100];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    snprintf(buffer, sizeof(buffer), "- %02d:%02d:%02d - %02d/%02d/%d",local->tm_hour, local->tm_min, local->tm_sec,local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
    return buffer;
}

void delay(int number_of_seconds){
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

int accountExists(int newID) {
    struct user_account user;
    FILE *info;

    info = fopen("info.txt", "r");
    if (info == NULL) {
        printf("\n\t\tCould not open file.\n");
        return 0;
    }

    while (fscanf(info, "%s %d %s %f", user.usr_name, &user.id, user.password, &user.balance) != EOF) {
        if (newID == user.id) {
            fclose(info);
            return 1;
        }
    }
    fclose(info);
    return 0;
}

int generateRandomAccountNumber() {
    int accountNumber;
    do {
        accountNumber = (rand() % (MAX_ACCOUNT_NUMBER - MIN_ACCOUNT_NUMBER + 1)) + MIN_ACCOUNT_NUMBER;
    } while (accountExists(accountNumber));
    return accountNumber;
}

void createAccount() {
    FILE *info;
    struct user_account new_user;
    info = fopen("info.txt", "a");

    if (info == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n\t       Add record     \n");
    printf("\tEnter name: ");
    getchar();
    scanf("%s", new_user.usr_name);

    new_user.id = generateRandomAccountNumber();
    printf("\n\tGenerated Account Number: %d\n", new_user.id);

    while (1) {
        printf("\tEnter Password, max 8 characters: ");
        scanf("%s", new_user.password);
        int len = strlen(new_user.password);

        if (len <= 8) {
            new_user.balance = 0;
            fprintf(info, "%s %d %s %.2f\n", new_user.usr_name, new_user.id, new_user.password, new_user.balance);

            printf("\n\t\tAccount Created Successfully\n");
            break;
        } else {
            printf("\n\t\tPassword exceeded maximum length! Please set within 8 characters.\n");
        }
    }

    fclose(info);
    delay(2);
}


void depositMoney(int id,float balance){
    char trans_type[20] = "DIPOSIT-";
    FILE *info;
    FILE *newinfo;
    FILE *trans;
    trans = fopen("transaction.txt","a");
    info = fopen("info.txt","r");
    newinfo = fopen("newinfo.txt","w");
                while(fscanf(info,"%s %d %s %f",user.usr_name,&user.id,&user.password,&user.balance)!= EOF){
                    if(id == user.id){
                        user.balance += balance;
                        fprintf(trans,"%s %d %s %.2f %s\n",user.usr_name,user.id,trans_type,balance,timeDate());
                    }
                    fprintf(newinfo,"%s %d %s %.2f\n",user.usr_name,user.id,user.password,user.balance);

                }
    fclose(trans);
    fclose(newinfo);
    fclose(info);

    remove("info.txt");
    rename("newinfo.txt", "info.txt");
}

void withdrawMoney(int id,double balance){
    struct user_account user;
    char trans_type[20] = "WITHDRAW";
    FILE *info,*newinfo,*trans;
    trans = fopen("transaction.txt","a");
    info = fopen("info.txt","r");
    newinfo = fopen("newinfo.txt","w");

    while(fscanf(info,"%s %d %s %f",user.usr_name,&user.id,&user.password,&user.balance)!= EOF){
        if(id == user.id){
            if(balance > user.balance){
                printf("\n\t\t\tInsufficient Balance!!\n\n");
            }else{
                user.balance -= balance;
                fprintf(trans,"%s %d %s %.2f %s\n",user.usr_name,user.id,trans_type,balance,timeDate());
            }
        }
        fprintf(newinfo,"%s %d %s %.2f\n",user.usr_name,user.id,user.password,user.balance);
    }
    fclose(trans);
    fclose(newinfo);
    fclose(info);

    remove("info.txt");
    rename("newinfo.txt","info.txt");
}

void sendMoney(int sen_id, int rec_id, float balance) {

    struct user_account user;
    FILE *info;
    info = fopen("info.txt", "r");
    while (fscanf(info, "%s %d %s %f", user.usr_name, &user.id, &user.password, &user.balance) != EOF) {
            if(sen_id == user.id){
                fclose(info);
                if(balance > user.balance){
                    printf("\n\t\t\tInsufficient Balance!!\n\n");
                }else{
                    if(accountExists(rec_id)){
                        withdrawMoney(sen_id,balance);
                        depositMoney(rec_id,balance);
                        printf("\n\n\t\tSuccessfully Sent money...\n\n");
                    }
                    else{
                        printf("\n\t\tRecipient account not found.\n\n");
                    }
                }
            }
    }
}

void checkBalance(int id){

    struct user_account user;
    FILE *info;
    info = fopen("info.txt","r");
    printf("\n\t\tCheck Balance\t\n");

    while(fscanf(info,"%s %d %s %f",user.usr_name,&user.id,&user.password,&user.balance)!= EOF){
        if(id == user.id){
            printf("\n\t\t%s,Your current Balance is : %.2f\n\n\n",user.usr_name,user.balance);
            break;
        }
    }
    fclose(info);
}

void accountDetails(int id) {
    struct user_account user;
    FILE *info, *trans;
    if(accountExists(id) == 0){
        printf("\n\n\t\tAccount Doesnot Exist....\n");
        delay(2);
    }else{

    info = fopen("info.txt", "r");
    while (fscanf(info, "%s %d %s %f", user.usr_name, &user.id, &user.password, &user.balance) != EOF) {
        if (id == user.id) {
            printf("\n\n\t\t  Account Details :\n\n\t\t\t  Name           : %s\n\t\t\t  Account Number : %d\n\t\t\t  Account Balance: %.2f\n",user.usr_name,user.id,user.balance);
            break;
        }
    }
    fclose(info);

    char time[50], name[20];
    float balance;
    int p_id, flag = 1;
    char trans_type[20];

    trans = fopen("transaction.txt", "r");
    printf("\n\n\t\t  Transaction Details:\n\n");
    while (fscanf(trans, "%s %d %s %f", name, &p_id, trans_type, &balance) != EOF) {
        fgets(time, sizeof(time), trans);
        time[strcspn(time, "\n")] = 0;
        if (id == p_id) {
            printf("\t\t%-10s %.2f at %s\n", trans_type, balance, time);
            flag = 0;
        }
    }
    if(flag){
        printf("\n\t\t\tNo transaction yet.\n");
    }
    fclose(trans);
    }
    printf("\n\t Press Enter to exit.");
    getch();
    system("cls");
}

void login(struct user_account user){

        FILE *info;
        struct user_account check;
        int m,id;
        float balance;

        login_menu:
            system("cls");
            printf("       \n\t\t\t\tBanking Management System     \n\n");
            printf("\t\t\t1. Deposit Money\n");
            printf("\t\t\t2. Withdraw Money\n");
            printf("\t\t\t3. Send Money\n");
            printf("\t\t\t4. Check Balance\n");
            printf("\t\t\t5. Account Details\n");
            printf("\t\t\t6. Logout\n");
            printf("\n\tEnter your choice: ");
            scanf("%d", &m);

        switch(m){

            case 1:
                printf("\t\tEnter money to deposit : ");
                scanf("%f",&balance);
                depositMoney(user.id,balance);
                printf("\n\t\tDiposit successfull.\n\n");
                delay(2);
                goto login_menu;
                break;

            case 2:
                printf("\n\n\tWithdraw money :\n");
                printf("\n\tEnter money to withdraw : ");
                scanf("%f",&balance);
                withdrawMoney(user.id,balance);
                printf("\n\n\t\tSuccessfully Withdrawed money !\n");
                delay(2);
                goto login_menu;
                break;

            case 3:
                printf("\n\n\tSend money\n");
                printf("\t\tEnter Account number : ");
                scanf("%d",&id);
                printf("\t\tEnter money to send : ");
                scanf("%f",&balance);

                sendMoney(user.id, id, balance);

                delay(2);
                goto login_menu;
                break;

            case 4:
                checkBalance(user.id);
                delay(2);
                goto login_menu;
                break;

            case 5:
                accountDetails(user.id);
                goto login_menu;
                break;

            case 6:
                printf("\n\t\t\t\tExiting...\n");
                delay(2);
                user_menu();
                break;

            default:
                printf("\tInvalid choice! Please try again.\n");
                delay(1);
                goto login_menu ;
                break;
            }
}

void user_menu(){

        int m;
        FILE *info;
        struct user_account user,check;
                system("cls");
                printf("       \n\t\t\t\tBanking Management System     \n\n");
                printf("\n\t\t\t1. Login");
                printf("\n\t\t\t2. Create Account\n");
                printf("\t\t\t3. Exit \n");

            printf("\n\tEnter your choice: ");
            scanf("%d", &m);

            switch (m) {
            case 1:
            usr_menu:
                info = fopen("info.txt","r");
                printf("\n\t\tEnter Account number: ");
                scanf("%d",&check.id);
                fflush(stdin);
                printf("\n\t\tEnter Password :");
                scanf("%s",check.password);
                while(fscanf(info,"%s %d %s %f",user.usr_name,&user.id,&user.password,&user.balance)!= EOF){
                    if( (check.id == user.id) && (strcmp(check.password, user.password) == 0)){
                        printf("\n\t\tLogin successful! Welcome, %s.\n\n", user.usr_name);
                        fclose(info);
                        delay(2);
                        login(user);
                    }
                }
                printf("\n\t\tLogin Failed! Re-enter ID and Password.\n\n");
                fclose(info);
                delay(2);
                goto usr_menu;
                break;

            case 2:
                createAccount();
                user_menu();
                break;

            case 3:
                printf("\n\t\tExiting ....\n\n");
                delay(2);
                system("cls");
                main();
                break;

            default:
                printf("\n\t\t\tInvalid choice! Please try again.\n");
                delay(2);
                user_menu();
            }
}

void updateInfo() {
    struct user_account user, check;
    FILE *info, *newinfo;
    int flag = 1;
    info = fopen("info.txt", "r");
    newinfo = fopen("newinfo.txt", "w");

    printf("\n\nEnter the account number of the user whose info you want to change: ");
    scanf("%d", &check.id);


    while (fscanf(info, "%s %d %s %f", user.usr_name, &user.id, user.password, &user.balance) != EOF) {
        if (check.id == user.id) {
            flag = 0;
            printf("\n\n\t\tEnter account holder's correct name : ");

            getchar();
            fgets(check.usr_name, sizeof(check.usr_name), stdin);
            if (check.usr_name[strlen(check.usr_name) - 1] == '\n') {
                check.usr_name[strlen(check.usr_name) - 1] = '\0';
            }
            strcpy(user.usr_name, check.usr_name);

            printf("\n\t\tName changed successfully.\n\n");
        }
        fprintf(newinfo, "%s %d %s %.2f\n", user.usr_name, user.id, user.password, user.balance);

    }
    if(flag){
        printf("\n\t\tAccount doesn't exists....\n\n");
        delay(2);
    }

    fclose(newinfo);
    fclose(info);
    remove("info.txt");
    rename("newinfo.txt", "info.txt");
}

void deleteAccount() {
    struct user_account user;
    int id;
    FILE *info, *newinfo;
    info = fopen("info.txt", "r");
    newinfo = fopen("newinfo.txt", "w");

    printf("\n\tEnter User Account number : ");
    scanf("%d", &id);

    int accountFound = 1;

    while (fscanf(info, "%s %d %s %f", user.usr_name, &user.id, user.password, &user.balance) != EOF) {
        if (id == user.id) {
            accountFound = 0;
            continue;
        }
        fprintf(newinfo, "%s %d %s %.2f\n", user.usr_name, user.id, user.password, user.balance);
    }
    if(accountFound){
        printf("\n\n\t\tAccount doesn't Exist.\n");
    }
    else{
        printf("\n\n\t\tSuccessfully Deleted Account!\n");
    }


    fclose(newinfo);
    fclose(info);
    remove("info.txt");
    rename("newinfo.txt", "info.txt");
    remove("newinfo.txt");

    delay(2);
}

void report(){
    FILE *trans;
    char trans_type[20],time[70],name[20];
    float balance;
    int id;
    trans = fopen("transaction.txt","r");
    printf("\n\t\t Transaction report :\n");
    printf("\n\tName\tAc.No \tTrans.Type   \tMoney        Time\n\n");
    while(fscanf(trans,"%s %d %s %f",name,&id ,trans_type, &balance) != EOF){
        fgets(time, sizeof(time), trans);
        time[strcspn(time, "\n")] = 0;
        printf("\t%s   %d        %s        %.2f       %s\n",name,id,trans_type, balance, time);
    }
    fclose(trans);
    printf("\n\t\tEnter to exit.");
    getch();
}

void adminMenu(){
    int m,id;
    while(1){
        printf("       \n\t\t\t\tBanking Management System     \n\n");
        printf("\t\t\tAdmin Menu \n\n");
        printf("\t\t1. See User Information\n");
        printf("\t\t2. Update User Information\n");
        printf("\t\t3. Delete User account\n");
        printf("\t\t4. Transaction Report \n");
        printf("\t\t5. Logout \n");
        printf("\nEnter your choice: ");
        scanf("%d", &m);
        switch(m){
            case 1:
                printf("\n\t\tEnter account number : ");
                scanf("%d",&id);
                //printf("       \n\t\t\t\tBanking Management System     \n\n");
                accountDetails(id);
                //getch();
                system("cls");
                break;

            case 2:
                updateInfo();
                delay(2);
                system("cls");
                break;

            case 3:
                deleteAccount();
                system("cls");
                break;

            case 4:
                system("cls");
                printf("       \n\t\t\t\tBanking Management System     \n\n");
                report();
                break;

            case 5:
                printf("\n\t\tExiting ...\n\n");
                delay(2);
                system("cls");
                main();
                break;

            default:
                printf("\t\tInvalid choice! Please try again.\n");
                delay(2);
                system("cls");
                adminMenu();
                break;
        }
        system("cls");
    }
}
//void histogram(){
//    FILE *trans;
//    char trans_type[20],time[70],name[20];
//    float balance,dep_total = 0.0;
//    int id;
//    trans = fopen("transaction.txt");
//    while(fscanf(trans,"%s %d %s %f",name,&id ,trans_type, &balance,time)){
//        if(trans_type == "WITHDRAW"){
//            wd_total = wd_total + balance;
//        }
//        else if(trans_type == "DIPOSIT-"){
//            dep_total = dep_total + balance;
//        }
//    }
//
//
//}

int main() {
    system("color e0");
    FILE *info;
    struct user_account user, check;
    struct Admin admin,ad_check;
    int n,m;

    while (1) {
        printf("       \n\t\t\t\tBanking Management System     \n\n");
        printf("\t\t\t1.User\n\t\t\t2.Admin\n");
        printf("\n\tEnter your choice: ");
        if (scanf("%d", &n) != 1) {
            printf("\n\tInvalid input! Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }
        switch(n){
            case 1:
                user_menu();
                break;

            case 2:
                printf("\n\tEnter Admin passsword: ");
                scanf("%s", &ad_check.pass_admin);
                if((strcmp(ad_check.pass_admin , "pass") == 0)){
                    system("cls");
                    adminMenu();
                }
                else{
                    printf("\n\t\tInvalid Password,try again.");
                    delay(2);
                    system("cls");
                }
                break;

            default :
                printf("\n\n\t\t\t\tInvalid option.\n\t\t\t\t\tTry again.....\n");
                break;
        }
    }
    return 0;
}

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>


struct currentValidID{
    int year;
    char branch[6];
    int totalVoters;
};
typedef struct {
    int cid;
    char cname[20];
    int votes;
}CANDIDATE;

// Global
struct currentValidID currentVID;
int numberOfCandidates;
CANDIDATE candidates[5];
char studentVotes[500];

void encode()
{
    printf("\x1B[40m" "\x1B[30m");
}

void decode()
{
    printf("\x1B[0m");
}


int extractYear(char userID[15])
{
    int year=0;
    char tmp;
    for(int i=0;i<4;i++){
        tmp=userID[i];
		year=(year*10)+(tmp-48);
    }
    return year;
}

int extractRollNo(char userID[15])
{
    int rollno=0;
    char tmp;
    for(int i=9;i<14;i++){
        tmp=userID[i];
		rollno=(rollno*10)+(tmp-48);
    }
    return rollno;
}
int checkBranchCode(char userID[15])
{
    char branchCode[6];
    for(int i=4;i<9;i++){
        branchCode[i-4]=userID[i];
    }
    branchCode[5]='\0';
    if(strcmp(branchCode,currentVID.branch)==0)
        return 1;
    else
        return 0;
}

int authenticationCheck()
{
    char username[15], password[6];
    
    printf("\nEnter username: ");
    scanf("%s",username);
    if((strcmp(username,"Admin"))!=0)
        return 0;
    else
    {
        printf("Enter Password: ");
        encode();
        scanf("%s",password);
        decode();
        if((strcmp(password,"Admin"))!=0)
            return 0;
    }
    return 1;
}

void createCandidateFiles(){
    printf("\nCreating candidate files...\n");
    FILE *fp;
	char filename[20];
    for(int i = 1;i <= numberOfCandidates; i++){
        sprintf(filename,"candidate%d.txt",i);
		fp=fopen(filename,"w");
        fprintf(
            fp,"0\n%s",
            candidates[i-1].cname
        );
		fclose(fp);
    }
    printf("Created Files successfully\n");
}

int getWinner(){
    int maxV = -1;
    int winnerCid;
    for(int i = 0;i < numberOfCandidates; i++){
        if(candidates[i].votes > maxV) {
            winnerCid = candidates[i].cid;
            maxV = candidates[i].votes;      
        }
        else if(candidates[i].votes == maxV) {
            return -1;
        }
    }
    return winnerCid;
}

void saveElectionInfoInFile(){
    printf("Saving Election Info in File...\n");
    FILE *fp = fopen("ElectionInfo.txt", "w");
    if(fp==NULL)
    {
        printf("\nError in file creation\n");
        fclose(fp);
        return;
    }
    fprintf(
        fp,"%d\n%s\n%d\n%d",
        currentVID.year,
        currentVID.branch,
        currentVID.totalVoters,
        numberOfCandidates
    );
    fclose(fp);
    printf("Saved Successfully : )");
}

void loadElectionInfoFromFile()
{
    FILE *f1,*f2,*f3;
    f1=fopen("ElectionInfo.txt","r");
    if(f1==NULL)
    {
        printf("\nNo Campaign Exists");
        return;
    }
    fscanf(f1,"%d",&currentVID.year);
    fseek(f1,2,SEEK_CUR);
    fscanf(f1,"%s",currentVID.branch);
    fseek(f1,2,SEEK_CUR);
    fscanf(f1,"%d",&currentVID.totalVoters);
    fseek(f1,2,SEEK_CUR);
    fscanf(f1,"%d",&numberOfCandidates);
    fclose(f1);
   
    //load candidates info and student votes
    for (int i = 0; i < currentVID.totalVoters; i++)
    {
        studentVotes[i] = '0';
    }
    for(int i=1;i<=numberOfCandidates;i++)
    {
        int location;
        char filename[20];
        sprintf(filename,"candidate%d.txt",i);
        f2=fopen(filename,"r+");
        candidates[i-1].cid=i;
        fscanf(f2,"%d",&candidates[i-1].votes);
        fscanf(f2,"%s",candidates[i-1].cname);
        while(!feof(f2)){
            fscanf(f2,"%d",&location);
            studentVotes[location-1] = i+48;
        }
        fclose(f2);
    }

}

int isValid(char userID[15])
{
    if(strlen(userID)!=14)
        return 0;
    
    int inputedYear=extractYear(userID);
    int inputedRollNo = extractRollNo(userID);
    
    if(inputedYear!=currentVID.year || checkBranchCode(userID)!=1 || inputedRollNo>currentVID.totalVoters)
        return 0;

    return 1;
}

int isVoted(char userID[15])
{
    int location=extractRollNo(userID);
    if(studentVotes[location-1]=='0')
        return 0;
    else
        return 1;
}

void saveVote(char userID[15],char voteInput)
{
    char filename[20];
    sprintf(filename,"candidate%d.txt",voteInput-48);
    FILE *fp = fopen(filename,"r+");
    int location=extractRollNo(userID);
    studentVotes[location-1]=voteInput;
    candidates[voteInput-49].votes++;
    fseek(fp, 0, SEEK_SET);
    fprintf(fp,"%d\n",candidates[voteInput-49].votes);
    fseek(fp, 0, SEEK_END);
    fprintf(fp,"\n%d",location);
    fclose(fp);
}


void studentPanel()
{
    char userID[15];
    char voteInput;
    while(1)
	{
		printf("\n\n  To exit press 0");
        printf("\n  Enter user ID:");
        scanf("%s",userID);
        if(strcmp(userID, "0")==0)
				return;
        if(isValid(userID)!=1)
        {
            printf("\n  Invalid User ID(Press Enter)");
            getchar();
            continue;
        }
        if(isVoted(userID)!=0)
        {
            printf("\n  Your PRN entered is already voted\n  Contact Admin for furthur query...(Press Enter to continue");
            getchar();
            continue;
        }
        printf("\n\n  Candidates for election:");
		for (int i = 0; i < numberOfCandidates; i++)
        {
            printf("\n  %d. %s",i+1,candidates[i].cname);
        }
        printf("\n\n  Your Vote(Enter Number):");
        fflush(stdin);
        encode();
        scanf("%c",&voteInput);
        decode();
        if(voteInput-48 < 1 || voteInput-48 > numberOfCandidates)
        {
            printf("\nInvalid Vote\nPress Enter to Try Again...");
            getchar();
            continue;
        }
        saveVote(userID,voteInput);
        printf("\n\nThanks for your precious vote(Press Enter)");
        getchar();
    }
}

void initiateNewElection()
{
    printf("\n New Election Creation \n");
    
    printf("\nElection for year: ");
    scanf("%d",&currentVID.year);
    printf("\nElection for branch: ");
    scanf("%s",currentVID.branch);
    printf("\nTotal Students (in the year and branch): ");
    scanf("%d",&currentVID.totalVoters);

    for(int i=0;i<currentVID.totalVoters;i++)
    {
        studentVotes[i] = '0';
    }

    do
    {
        printf("\nEnter the number of Candidates (Min: 2,Max: 5): ");
        scanf("%d",&numberOfCandidates);
    }while(numberOfCandidates<2 || numberOfCandidates>5);
    
    for(int i=0;i<numberOfCandidates;i++)
    {
        candidates[i].cid = i+1;
        printf("Enter the name of the Candidate %d: ",i+1);
        scanf("%s",candidates[i].cname);
        candidates[i].votes = 0;
    }
    
}

void adminPanel()
{
    while(1)
    {
        if(authenticationCheck()!=1)
        {
            printf("Invalid Username/Password! Try Again!");
            break;
        }
        
        printf("Login Successful! You can continue... (Press Enter)");
        getchar();
        
        while(1)
        {
            char input;
            fflush(stdin);
            printf("\n1. New Election\n2. Current Election\n3. Result\n4. Logout\nOption: ");
            scanf("%c",&input);
            
            switch(input)
            {
                case '1':
                    initiateNewElection();
                    saveElectionInfoInFile();
                    createCandidateFiles();
                    break;
                case '2':
                    loadElectionInfoFromFile();
                    break;
                case '3':
                    int totalVoted=0;
                    int WinnerCid = getWinner();
                    if(WinnerCid != -1){
                        printf("\nWinner is %s with %d votes\n",candidates[WinnerCid-1].cname,candidates[WinnerCid-1].votes);
                    }
                    else{
                        printf("\nIts A TIE");
                    }
                    printf("\nFull Result\n");
                    for(int i=0;i<numberOfCandidates;i++){
                        totalVoted+=candidates[i].votes;
                        printf("%d. %s -> %d votes\n",candidates[i].cid,candidates[i].cname,candidates[i].votes);
                    }
                    printf("\nVoting Percentage: %d %%\n\n",(totalVoted*100)/currentVID.totalVoters);
                    break;
                case '4':
                    return;
                    break;
                default:
                    printf("Invalid Option! Press Enter to continue...");
                    getchar();
            }
        }
        
    }
    
}



int main(){
    while(1){
        printf("\n\t\t\t   1.Student panel \n\t\t\t   2.Admin panel \n\t\t\t   3.Exit \n\t\t\t   Option:");
		char input;
        scanf("%c",&input);

        switch(input){
            case '1':
                studentPanel();
                break;
            case '2':
                adminPanel();
                break;
            case '3':
                return 0;
            default:
                printf("\nInvalid option...(Press Enter to continue)");
                getchar();
        }
    }
    return 0;
}

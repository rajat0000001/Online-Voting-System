#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define maxV 100000
#define maxC 5

struct V{
    int id;
    char name[100];
    char mail[100];
    char ps[100];
    bool voted;
};

struct C{
    int id;
    char name[100];
    char mail[100];
    char ps[100];
    int votes;
};

struct V voters[maxV];
struct C candidates[maxC];
int vCount=0, cCount=0;
bool loggedIn=false;
int currentVoter=-1;
bool electionEnded=false;
int totalVotes=0;
int maxVotes=0;
int winner=-1;
int runnerUp=-1;
int secondRunnerUp=-1;
int totalCandidates=0;
int totalVoters=0;
void login(){
    char mail[100], ps[100];
    printf("Enter Email: ");
    scanf("%s", mail);
    printf("Enter Password: ");
    scanf("%s", ps);
    for(int i=0; i<vCount; i++){
        if(strcmp(voters[i].mail, mail)==0 && strcmp(voters[i].ps, ps)==0){
            loggedIn=true;
            currentVoter=i;
            printf("Login Successful!\n");
            return;
        }
    }
    printf("Invalid Email or Password!\n");
}
void registerVoter(){
    if(vCount>=maxV){
        printf("Voter limit reached!\n");
        return;
    }
    struct V newVoter;
    newVoter.id=vCount+1;
    printf("Enter Name: ");
    scanf(" %[^\n]", newVoter.name);
    printf("Enter Email: ");
    scanf("%s", newVoter.mail);
    for(int i=0; i<vCount; i++){
        if(strcmp(voters[i].mail, newVoter.mail)==0){
            printf("Email already registered!\n");
            return;
        }
    }
    printf("Enter Password: ");
    scanf("%s", newVoter.ps);
    newVoter.voted=false;
    voters[vCount]=newVoter;
    vCount++;
    totalVoters++;
    printf("Registration Successful! Your Voter ID is %d\n", newVoter.id);
}
void registerCandidate(){
    if(cCount>=maxC){
        printf("Candidate limit reached!\n");
        return;
    }
    struct C newCandidate;
    newCandidate.id=cCount+1;
    printf("Enter Name: ");
    scanf(" %[^\n]", newCandidate.name);
    printf("Enter Email: ");
    scanf("%s", newCandidate.mail);
    for(int i=0; i<cCount; i++){
        if(strcmp(candidates[i].mail, newCandidate.mail)==0){
            printf("Email already registered!\n");
            return;
        }
    }
    printf("Enter Password: ");
    scanf("%s", newCandidate.ps);
    newCandidate.votes=0;
    candidates[cCount]=newCandidate;
    cCount++;
    totalCandidates++;
    printf("Registration Successful! Your Candidate ID is %d\n", newCandidate.id);
}
void castVote(){
    if(!loggedIn){
        printf("Please login first!\n");
        return;
    }
    if(electionEnded){
        printf("Election has ended! You cannot vote now.\n");
        return;
    }
    if(voters[currentVoter].voted){
        printf("You have already voted!\n");
        return;
    }
    printf("Candidates:\n");
    for(int i=0; i<cCount; i++){
        printf("%d. %s\n", candidates[i].id, candidates[i].name);
    }
    int choice;
    printf("Enter Candidate ID to vote: ");
    scanf("%d", &choice);
    if(choice<1 || choice>cCount){
        printf("Invalid Candidate ID!\n");
        return;
    }
    candidates[choice-1].votes++;
    voters[currentVoter].voted=true;
    totalVotes++;
    printf("Vote casted successfully!\n");
}
void endElection(){
    if(electionEnded){
        printf("Election has already ended!\n");
        return;
    }
    electionEnded=true;
    for(int i=0; i<cCount; i++){
        if(candidates[i].votes>maxVotes){
            maxVotes=candidates[i].votes;
            winner=i;
        }
    }
    for(int i=0; i<cCount; i++){
        if(i!=winner && (runnerUp==-1 || candidates[i].votes>candidates[runnerUp].votes)){
            runnerUp=i;
        }
    }
    for(int i=0; i<cCount; i++){
        if(i!=winner && i!=runnerUp && (secondRunnerUp==-1 || candidates[i].votes>candidates[secondRunnerUp].votes)){
            secondRunnerUp=i;
        }
    }
    printf("Election Ended!\n");
    printf("Winner: %s with %d votes\n", candidates[winner].name, candidates[winner].votes);
    if(runnerUp!=-1)
        printf("Runner Up: %s with %d votes\n", candidates[runnerUp].name, candidates[runnerUp].votes);
    if(secondRunnerUp!=-1)
        printf("Second Runner Up: %s with %d votes\n", candidates[secondRunnerUp].name, candidates[secondRunnerUp].votes);
    printf("Total Votes Casted: %d\n", totalVotes);
}
void logout(){
    if(!loggedIn){
        printf("No user is logged in!\n");
        return;
    }
    loggedIn=false;
    currentVoter=-1;
    printf("Logged out successfully!\n");
}
void displayMenu(){
    if(!electionEnded&&loggedIn){
        printf("\n--- Voting System Menu ---\n");
        printf("1. Cast Vote\n");
        printf("2. End Election\n");
        printf("3. Logout\n");
        printf("4. Exit\n");
        
    } else if(!electionEnded){
        printf("\n--- Voting System Menu ---\n");
        printf("1. Register as Voter\n");
        printf("2. Register as Candidate\n");
        printf("3. Login\n");
        printf("4. Exit\n");
    } else {
        printf("\n--- Election Ended ---\n");
        printf("1. View Results\n");
        printf("2. Exit\n");
    }
    
}
int main(){
    int choice;
    while(1){
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(!electionEnded&&loggedIn){
            switch(choice){
                case 1:
                    castVote();
                    break;
                case 2:
                    endElection();
                    break;
                case 3:
                    logout();
                    break;
                case 4:
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice! Please try again.\n");
            }
                
        } else if(!electionEnded){
            switch(choice){
                case 1:
                    registerVoter();
                    break;
                case 2:
                    registerCandidate();
                    break;
                case 3:
                    login();
                    break;
                case 4:
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } else {
            switch(choice){
                case 1:
                    printf("Winner: %s with %d votes\n", candidates[winner].name, candidates[winner].votes);
                    if(runnerUp!=-1)
                        printf("Runner Up: %s with %d votes\n", candidates[runnerUp].name, candidates[runnerUp].votes);
                    if(secondRunnerUp!=-1)
                        printf("Second Runner Up: %s with %d votes\n", candidates[secondRunnerUp].name, candidates[secondRunnerUp].votes);
                    printf("Total Votes Casted: %d\n", totalVotes);
                    break;
                case 2:
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        }
    }
    return 0;
}
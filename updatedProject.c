#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 60
#define MAX_CANDIDATES 5

// Structure for Candidate Information
typedef struct {
    char name[50];
    int rollNo;
    int votes;
    char gender;
    float attendancePercentage; // New field for attendance percentage
} Candidate;

Candidate maleCandidates[MAX_CANDIDATES];
Candidate femaleCandidates[MAX_CANDIDATES];
int maleCount = 0, femaleCount = 0;
int hasVoted[MAX_STUDENTS] = {0}; // Tracks if a student has voted or is a candidate

// Function Declarations
void candidateMenu();
void voterMenu();
void voteMale();
void voteFemale();
void displayResults();
void viewCandidates();
void loadCandidates();  // Load data from file
void saveCandidates();  // Save data to file
void sortByRollNo(Candidate candidates[], int count);  // Sort candidates by roll number
void inputAttendanceForTiedCandidates(Candidate *candidates[], int count); // Input attendance for tied candidates

// Main Function
int main() {
    loadCandidates();  // Load data from files on startup
    int choice;
    
    while (1) {
        printf("\n--- Class Representative Election System ---\n");
        printf("1. Stand as a Candidate\n");
        printf("2. View Applied Candidates\n");
        printf("3. Vote \n");  
        printf("4. Display Results\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                candidateMenu();
                break;
            case 2:
                viewCandidates();
                break;
            case 3:
                voterMenu();
                break;
            case 4:
                displayResults();
                break;
            case 5:
                saveCandidates();  // Save data before exiting
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Candidate Menu: Register as a candidate
void candidateMenu() {
    int rollNo;
    char name[50], gender;

    printf("Enter your roll number (1-60): ");
    scanf("%d", &rollNo);

    if (hasVoted[rollNo - 1] == 1) {
        printf("You cannot stand as a candidate and vote!\n");
        return;
    }

    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your gender (M/F): ");
    scanf(" %c", &gender);

    // Register male candidate
    if (gender == 'M' || gender == 'm') {
        if (maleCount < MAX_CANDIDATES) {
            maleCandidates[maleCount].rollNo = rollNo;
            strcpy(maleCandidates[maleCount].name, name);
            maleCandidates[maleCount].gender = 'M';
            maleCandidates[maleCount].votes = 0;
            maleCount++;
            printf("Successfully registered as a male candidate!\n");
        } else {
            printf("Maximum male candidates reached!\n");
        }
    }
    // Register female candidate
    else if (gender == 'F' || gender == 'f') {
        if (femaleCount < MAX_CANDIDATES) {
            femaleCandidates[femaleCount].rollNo = rollNo;
            strcpy(femaleCandidates[femaleCount].name, name);
            femaleCandidates[femaleCount].gender = 'F';
            femaleCandidates[femaleCount].votes = 0;
            femaleCount++;
            printf("Successfully registered as a female candidate!\n");
        } else {
            printf("Maximum female candidates reached!\n");
        }
    } else {
        printf("Invalid gender!\n");
    }

    hasVoted[rollNo - 1] = 1; // Candidate cannot vote
    saveCandidates();  // Save updated candidate list after registration
}

// Voter Menu: Voting process for male and female representatives
void voterMenu() {
    int rollNo;

    printf("Enter your roll number (1-60): ");
    scanf("%d", &rollNo);

    if (hasVoted[rollNo - 1] == 1) {
        printf("You have already voted or are a candidate!\n");
        return;
    }

    // Vote for male representative
    if (maleCount == 0) {
        printf("No male candidates available for election.\n");
    } else if (maleCount == 1) {
        printf("Male Representative %s (Roll No: %d) is automatically elected!\n",
               maleCandidates[0].name, maleCandidates[0].rollNo);
        maleCandidates[0].votes++; // Auto-elect
    } else {
        voteMale();
    }

    // Vote for female representative
    if (femaleCount == 0) {
        printf("No female candidates available for election.\n");
    } else if (femaleCount == 1) {
        printf("Female Representative %s (Roll No: %d) is automatically elected!\n",
               femaleCandidates[0].name, femaleCandidates[0].rollNo);
        femaleCandidates[0].votes++; // Auto-elect
    } else {
        voteFemale();
    }

    hasVoted[rollNo - 1] = 1; // Mark student as having voted
    saveCandidates();  // Save votes after voting
}

// Voting function for male candidates
void voteMale() {
    int choice;

    printf("\n--- Male Candidates ---\n");
    for (int i = 0; i < maleCount; i++) {
        printf("%d. Name: %s, Roll No: %d\n", i + 1, maleCandidates[i].name, maleCandidates[i].rollNo);
    }

    printf("Choose your preferred male candidate: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= maleCount) {
        maleCandidates[choice - 1].votes++;
        printf("You voted for %s!\n", maleCandidates[choice - 1].name);
    } else {
        printf("Invalid choice!\n");
    }
}

// Voting function for female candidates
void voteFemale() {
    int choice;

    printf("\n--- Female Candidates ---\n");
    for (int i = 0; i < femaleCount; i++) {
        printf("%d. Name: %s, Roll No: %d\n", i + 1, femaleCandidates[i].name, femaleCandidates[i].rollNo);
    }

    printf("Choose your preferred female candidate: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= femaleCount) {
        femaleCandidates[choice - 1].votes++;
        printf("You voted for %s!\n", femaleCandidates[choice - 1].name);
    } else {
        printf("Invalid choice!\n");
    }
}

// Display Results: Show all candidates with their votes and declare winners
void displayResults() {
    printf("\n--- Election Results ---\n");

    // Male Candidates' Votes
    if (maleCount == 0) {
        printf("No male candidates stood for election.\n");
    } else {
        printf("\n--- Male Candidates ---\n");
        for (int i = 0; i < maleCount; i++) {
            printf("Name: %s, Roll No: %d, Votes: %d\n", maleCandidates[i].name, maleCandidates[i].rollNo, maleCandidates[i].votes);
        }

        // Determine the male representative(s) with the highest votes
        int maxVotes = 0;
        for (int i = 0; i < maleCount; i++) {
            if (maleCandidates[i].votes > maxVotes) {
                maxVotes = maleCandidates[i].votes;
            }
        }

        printf("\nMale Representative(s):\n");

        // Collect all candidates with max votes (possible tie)
        int tieCount = 0;
        Candidate *tiedMaleCandidates[MAX_CANDIDATES];
        for (int i = 0; i < maleCount; i++) {
            if (maleCandidates[i].votes == maxVotes) {
                tiedMaleCandidates[tieCount++] = &maleCandidates[i];
            }
        }

        // Handle tie by attendance percentage
        if (tieCount > 1) {
            printf("There is a tie between the following male candidates:\n");
            for (int i = 0; i < tieCount; i++) {
                printf("%d. %s (Roll No: %d)\n", i + 1, tiedMaleCandidates[i]->name, tiedMaleCandidates[i]->rollNo);
            }

            // Input attendance for tied candidates
            inputAttendanceForTiedCandidates(tiedMaleCandidates, tieCount);

            // Find the candidate with the highest attendance
            Candidate *winningMale = tiedMaleCandidates[0];
            for (int i = 1; i < tieCount; i++) {
                if (tiedMaleCandidates[i]->attendancePercentage > winningMale->attendancePercentage) {
                    winningMale = tiedMaleCandidates[i];
                }
            }

            // Display winner after tie-breaker
            printf("The winner based on attendance is %s (Roll No: %d) with %.2f%% attendance!\n", 
                    winningMale->name, winningMale->rollNo, winningMale->attendancePercentage);
        } else {
            // No tie, only one winner
            printf("The winner is %s (Roll No: %d) with %d votes!\n", tiedMaleCandidates[0]->name, tiedMaleCandidates[0]->rollNo, maxVotes);
        }
    }

    // Repeat the same logic for female candidates
    if (femaleCount == 0) {
        printf("No female candidates stood for election.\n");
    } else {
        printf("\n--- Female Candidates ---\n");
        for (int i = 0; i < femaleCount; i++) {
            printf("Name: %s, Roll No: %d, Votes: %d\n", femaleCandidates[i].name, femaleCandidates[i].rollNo, femaleCandidates[i].votes);
        }

        // Determine the female representative(s) with the highest votes
        int maxVotes = 0;
        for (int i = 0; i < femaleCount; i++) {
            if (femaleCandidates[i].votes > maxVotes) {
                maxVotes = femaleCandidates[i].votes;
            }
        }

        printf("\nFemale Representative(s):\n");

        // Collect all candidates with max votes (possible tie)
        int tieCount = 0;
        Candidate *tiedFemaleCandidates[MAX_CANDIDATES];
        for (int i = 0; i < femaleCount; i++) {
            if (femaleCandidates[i].votes == maxVotes) {
                tiedFemaleCandidates[tieCount++] = &femaleCandidates[i];
            }
        }

        // Handle tie by attendance percentage
        if (tieCount > 1) {
            printf("There is a tie between the following female candidates:\n");
            for (int i = 0; i < tieCount; i++) {
                printf("%d. %s (Roll No: %d)\n", i + 1, tiedFemaleCandidates[i]->name, tiedFemaleCandidates[i]->rollNo);
            }

            // Input attendance for tied candidates
            inputAttendanceForTiedCandidates(tiedFemaleCandidates, tieCount);

            // Find the candidate with the highest attendance
            Candidate *winningFemale = tiedFemaleCandidates[0];
            for (int i = 1; i < tieCount; i++) {
                if (tiedFemaleCandidates[i]->attendancePercentage > winningFemale->attendancePercentage) {
                    winningFemale = tiedFemaleCandidates[i];
                }
            }

            // Display winner after tie-breaker
            printf("The winner based on attendance is %s (Roll No: %d) with %.2f%% attendance!\n", 
                    winningFemale->name, winningFemale->rollNo, winningFemale->attendancePercentage);
        } else {
            // No tie, only one winner
            printf("The winner is %s (Roll No: %d) with %d votes!\n", tiedFemaleCandidates[0]->name, tiedFemaleCandidates[0]->rollNo, maxVotes);
        }
    }
}

// Function to input attendance for tied candidates
void inputAttendanceForTiedCandidates(Candidate *candidates[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Enter attendance percentage for %s (Roll No: %d): ", candidates[i]->name, candidates[i]->rollNo);
        scanf("%f", &candidates[i]->attendancePercentage);
    }
}

// Function to sort candidates by roll number in ascending order
void sortByRollNo(Candidate candidates[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (candidates[j].rollNo > candidates[j + 1].rollNo) {
                // Swap candidates
                Candidate temp = candidates[j];
                candidates[j] = candidates[j + 1];
                candidates[j + 1] = temp;
            }
        }
    }
}

// Function to view candidates sorted by roll number
void viewCandidates() {
    printf("\n--- Applied Candidates ---\n");

    // Sort and display male candidates
    if (maleCount == 0) {
        printf("No male candidates have applied yet.\n");
    } else {
        sortByRollNo(maleCandidates, maleCount); // Sort male candidates by roll number
        printf("\n--- Male Candidates ---\n");
        for (int i = 0; i < maleCount; i++) {
            printf("%d. Name: %s, Roll No: %d\n", i + 1, maleCandidates[i].name, maleCandidates[i].rollNo);
        }
    }

    // Sort and display female candidates
    if (femaleCount == 0) {
        printf("No female candidates have applied yet.\n");
    } else {
        sortByRollNo(femaleCandidates, femaleCount); // Sort female candidates by roll number
        printf("\n--- Female Candidates ---\n");
        for (int i = 0; i < femaleCount; i++) {
            printf("%d. Name: %s, Roll No: %d\n", i + 1, femaleCandidates[i].name, femaleCandidates[i].rollNo);
        }
    }
}

// Function to load candidates and votes from a file
void loadCandidates() {
    FILE *file = fopen("candidates.txt", "r");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }

    fread(&maleCount, sizeof(int), 1, file);
    fread(maleCandidates, sizeof(Candidate), maleCount, file);
    fread(&femaleCount, sizeof(int), 1, file);
    fread(femaleCandidates, sizeof(Candidate), femaleCount, file);
    fclose(file);
}

// Function to save candidates and votes to a file
void saveCandidates() {
    FILE *file = fopen("candidates.txt", "w");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    fwrite(&maleCount, sizeof(int), 1, file);
    fwrite(maleCandidates, sizeof(Candidate), maleCount, file);
    fwrite(&femaleCount, sizeof(int), 1, file);
    fwrite(femaleCandidates, sizeof(Candidate), femaleCount, file);
    fclose(file);
}

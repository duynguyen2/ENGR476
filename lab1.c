/* ENGR 476 Lab 1
 * Name: Duy Nguyen
 * Student ID: 917446249
 * Professor Hamid Shahnasser, Zaid Syed
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMSTUDENTS 7 //defined variable that can be changed easily

//struct to hold all the information for one student
typedef struct studentInfo {
	char studentName[10];
	int studentNum;
	float subjectAScore;
	float subjectBScore;
} studentInfo;

//prototypes for methods
//the methods take an array of students to avoid needing a global variable
void printHelp();
void diploma(studentInfo []);
void arrangeSubjectA(studentInfo []);
void calculateAvgAndStd(studentInfo []);
float calculateAvgA(studentInfo []);
float calculateAvgB(studentInfo []);
float calculateStdA(studentInfo [], float);
float calculateStdB(studentInfo [], float);
int storeData(studentInfo []);


int main() {

	FILE* fp; //file to open

	studentInfo students[NUMSTUDENTS]; //array of all the students

	//input program
	char fileName[100]; //var to hold file name
	printf("Please enter input file name: ");
	scanf("%s", fileName);

	//check to make sure the file could be opened
	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("ERROR: Cannot open input file!/n");
		exit(1);
	}

	char line[35]; //skipping first line(i.e. STUDENT NAME, STUDENT NO., etc)
	fscanf(fp, "%s %s %s %s %s %s %s %s\n", line, line, line, line, line, line, line, line);

	//printing and storing the information from the file
	int studentIndex = 0;
	printf("%s:\n", fileName);
	printf("STUDENT NAME\tSTUDENT NO.\tSUBJECT A\tSUBJECT B\n");
	while (fscanf(fp, "%s %d %f %f\n", students[studentIndex].studentName, &students[studentIndex].studentNum, &students[studentIndex].subjectAScore, &students[studentIndex].subjectBScore) != EOF) {
		printf("%s\t\t%d\t\t%.1f\t\t%.1f\n", students[studentIndex].studentName, students[studentIndex].studentNum, students[studentIndex].subjectAScore, students[studentIndex].subjectBScore);
		studentIndex++;
	}

	//at this point, recieved everything we need from file so close it
	if (fclose(fp) != 0) {
		printf("ERROR: Cannot close input file!/n");
		exit(1);
	}

	//while the recieved char input is not e for exit, run our input terminal
	char c[1000];
	do {
		printf("\nPlease enter a command (enter h for help):");
		scanf("%s", &c);

		if (strcmp(c, "1") == 0 || strcmp(c, "a") == 0) {
			diploma(students);
		}

		else if (strcmp(c, "2") == 0 || strcmp(c, "b") == 0) {
			arrangeSubjectA(students);
		}

		else if (strcmp(c, "3") == 0 || strcmp(c, "c") == 0) {
			float avgA = calculateAvgA(students), avgB = calculateAvgB(students);
			float stdA = calculateStdA(students, avgA), stdB = calculateStdB(students, avgB);

			printf("Average for Subject A: %.1f\nAverage for Subject B: %.1f\n", avgA, avgB);
			printf("Standard Deviation for Subject A: %.1f\nStandard Deviation for Subject B: %.1f\n", stdA, stdB);
		}

		else if (strcmp(c, "4") == 0 || strcmp(c, "d") == 0) {
			if (storeData(students) == -1) {
				printf("ERROR: Cannot store results!/n");
				exit(1);
			}
			else {
				printf("Results successfully store!\n");
			}
		}

		else if (strcmp(c, "h") == 0) {
			printHelp();
		}

	} while (strcmp(c, "e") != 0);

	return 0;
}

void printHelp() {

	printf("a/1 to obtain all the students that got diploma\n");
	printf("b/2 to arrange subject A in ascending order\n");
	printf("c/3 to calculate the average and standard deviation\n");
	printf("d/4 to save all the above results in an output file\n");
	printf("e to exit\n");

}

//take the array of students and only print the ones who have high enough scores for a diploma
void diploma(studentInfo students[]) {
	printf("All the students that got diplomas:\n");
	printf("STUDENT NAME\tSUBJECT A\tSUBJECT B\n");

	for (int i = 0; i < NUMSTUDENTS; i++) {
		if (students[i].subjectAScore >= 50 && students[i].subjectBScore >= 50)
			printf("%s\t\t%.1f\t\t%.1f\n", students[i].studentName, students[i].subjectAScore, students[i].subjectBScore);
	}
}

//take the array of students and swap the smallest index
void arrangeSubjectA(studentInfo students[]) {
	studentInfo tempArr[NUMSTUDENTS];
	for (int i = 0; i < NUMSTUDENTS; i++) { //copying variable
		tempArr[i] = students[i];
	}

	int smallestIndex;
	for (int i = 0; i < NUMSTUDENTS - 1; i++) {

		smallestIndex = i;
		for (int j = i + 1; j < NUMSTUDENTS; j++) {
			if (tempArr[smallestIndex].subjectAScore > tempArr[j].subjectAScore) { //look for index with worst score
				smallestIndex = j;
			}
		}

		if (smallestIndex != i) { //swap
			studentInfo temp = tempArr[i];
			tempArr[i] = tempArr[smallestIndex];
			tempArr[smallestIndex] = temp;
		}
	}
	
	printf("STUDENT NAME\tSUBJECT A\tSUBJECT B\n");
	for (int i = 0; i < NUMSTUDENTS; i++) {
		printf("%s\t\t%.1f\t\t%.1f\n", tempArr[i].studentName, tempArr[i].subjectAScore, tempArr[i].subjectBScore);
	}
}

//take the array of students and calculate the average
float calculateAvgA(studentInfo students[]) {
	float avgA = 0;
	for (int i = 0; i < NUMSTUDENTS; i++) {
		avgA += students[i].subjectAScore;
	}
	avgA /= NUMSTUDENTS;
	return avgA;
}

float calculateAvgB(studentInfo students[]) {
	float avgB = 0;
	for (int i = 0; i < NUMSTUDENTS; i++) {
		avgB += students[i].subjectBScore;
	}
	avgB /= NUMSTUDENTS;
	return avgB;
}

//take the array of students and the average and calculate the standard deviation
float calculateStdA(studentInfo students[], float avg) {
	float stdev = 0;
	for (int i = 0; i < NUMSTUDENTS; i++) {
		stdev += pow(students[i].subjectAScore - avg, 2);
	}
	stdev = sqrt(stdev / NUMSTUDENTS);
	return stdev;
}

float calculateStdB(studentInfo students[], float avg) {
	float stdev = 0;
	for (int i = 0; i < NUMSTUDENTS; i++) {
		stdev += pow(students[i].subjectBScore - avg, 2);
	}
	stdev = sqrt(stdev / NUMSTUDENTS);
	return stdev;
}

//this will write all the results in an external file, it does what all the other methods do but with small modifications to write them
int storeData(studentInfo students[]) { // will return 0 on success, return -1 otherwise
	FILE* output;
	if ((output = fopen("output.txt", "w")) == NULL) {
		printf("ERROR: Could not create/open output file!\n");
		return -1;
	}

	fprintf(output, "All the students with diplomas:\n");
	fprintf(output, "STUDENT NAME\tSTUDENT NO.\tSUBJECT A\tSUBJECT B\n");

	for (int i = 0; i < NUMSTUDENTS; i++) {
		fprintf(output, "%s\t\t%.1f\t\t%.1f\n", students[i].studentName, students[i].subjectAScore, students[i].subjectBScore);
	}

	fprintf(output, "\nSubject A in ascending order\n");
	fprintf(output, "STUDENT NAME\t\tSUBJECT A\tSUBJECT B\n");
	studentInfo tempArr[NUMSTUDENTS];
	for (int i = 0; i < NUMSTUDENTS; i++) { //copying variable
		tempArr[i] = students[i];
	}

	int smallestIndex;
	for (int i = 0; i < NUMSTUDENTS - 1; i++) {

		smallestIndex = i;
		for (int j = i + 1; j < NUMSTUDENTS; j++) {
			if (tempArr[smallestIndex].subjectAScore > tempArr[j].subjectAScore) { //look for index with worst score
				smallestIndex = j;
			}
		}

		if (smallestIndex != i) { //swap
			studentInfo temp = tempArr[i];
			tempArr[i] = tempArr[smallestIndex];
			tempArr[smallestIndex] = temp;
		}
	}

	for (int i = 0; i < NUMSTUDENTS; i++) {
		fprintf(output, "%s\t\t%.1f\t\t%.1f\n", tempArr[i].studentName, tempArr[i].subjectAScore, tempArr[i].subjectBScore);
	}

	float avgA = calculateAvgA(students), avgB = calculateAvgB(students);
	float stdA = calculateStdA(students, avgA), stdB = calculateStdB(students, avgB);

	fprintf(output, "\nAverage for Subject A: %.1f\nAverage for Subject B: %.1f\n", avgA, avgB);
	fprintf(output, "Standard Deviation for Subject A: %.1f\nStandard Deviation for Subject B: %.1f\n", stdA, stdB);

	if (fclose(output) != 0) {
		printf("ERROR: Cannot close the output file!\n");
		return -1;
	}

	return 0;
}
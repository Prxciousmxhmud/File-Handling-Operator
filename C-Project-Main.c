#include <stdio.h>
#include <string.h>
// function for SEARCH operation
int searchForString(char *fname, char *str) {
    // https://www.codingunit.com/c-tutorial-searching-for-strings-in-a-text-file#:~:text=With%20fgets()%20we%20get,we%20increase%20find_result%20with%20one.
	int line_num = 1;
	int find_result = 0;
	char temp[512]; // holds a line of the file
    FILE *fptr1 = fopen(fname, "r");
    //checks file exists, returning if it doens't
	if (fptr1 == NULL){
        printf("File not found\n");
        return -1;
    }
	// checks file line by line
	while(fgets(temp, 512, fptr1) != NULL) {
        // checks if the current line has the string to be found 
		if((strstr(temp, str)) != NULL) {
            // outputs when it is found
			printf("A match found on line: %d\n", line_num);
            printf("Context: ");
			printf("\n%s\n", temp);
			find_result++;
		}
        // goes to next line
		line_num++;
	}
    printf("Number of times string appeared: %d\n", find_result);

	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
	}
    if(fptr1) {
        fclose(fptr1);
	}
   	return(0);
}

int numOfLines(const char *filename) {
    // reusable so that it can be used in log and LINES operation
    //https://www.geeksforgeeks.org/c-program-count-number-lines-file/
    int lines = 0; // line counter
    char c; //holds each character in the file

    FILE *fptr1 = fopen(filename, "r"); // opens file
    // checks if file exists
    if (fptr1 == NULL){
        printf("File not found\n");
        return 0;
    } else {
        while((c= fgetc(fptr1)) !=EOF){ // repeats until the end of the file is reached
            if (c == '\n'){ // if character is a new line character increase count
                lines++;
            }
        }
        fclose(fptr1);
        return lines;// returns number
    }
}

// log function
// take an input of what the command is, the file this happend to and number of lines in that file
// append to log file
void logFile(const char *filename, const char *operation, int numLines) {
    FILE *logF = fopen("logfile.txt", "a");
    fprintf(logF, "FILE: %s " " Operation: %s  Number of lines: %d\n", filename, operation, numLines);
    fclose(logF);

    // log file can then be shown using show
    // in the log file there will be FILE: OPERATION: NUMBER OF LINES:
}
// function to show contents of a specific file
// reusable so it can be used for log and showing generic files
void showFile(char *fileName, char *command) {
    FILE *fptr1 = fopen(fileName, "r");
    if (fptr1 == NULL) {
        printf("Error: Could not open the file for reading.\n");
        return;
    }

    char stringArray[255]; // used to hold a line in the file

    if (strcmp(fileName, "logfile.txt") == 0){
        printf("Log:\n");
    }else{
        printf("File contents:\n");
    }
    

    // read and print file line by line
    while (fgets(stringArray, sizeof(stringArray), fptr1)) {
        printf("%s", stringArray);
    }
    fclose(fptr1);
    // adds to the log 
    logFile(fileName, command, numOfLines(fileName));
}

int main(int argc, char *argv[]) {
    FILE *fptr1, *fptr2, *tempF,*logF; // tempF pointer is for deleting lines and inserting lines
    char sFileName[] = "File1.txt";
    char sCommand[50]; // To store user input
    char OPERATION[50];
    int characters; 
    // create log file, not needed if already created
    //logF = fopen("logfile.txt", "w");
    //fclose(logF);
    while(1){
        printf("\nEnter a mode (FILE OPERATIONS, LINE OPERATIONS, GENERAL OPERATIONS, EXIT): ");
        fgets(sCommand, sizeof(sCommand), stdin);
        sCommand[strcspn(sCommand, "\n")] = '\0'; // gets rid of new line character so command is read correctly


        if (strcmp(sCommand, "FILE OPERATIONS") == 0){
            printf("Enter the name of the file you'd like to operate on(without .txt): ", sFileName);
            fgets(sFileName, sizeof(sFileName), stdin);
            sFileName[strcspn(sFileName, "\n")] = '\0'; // gets rid of new line character
            strcat(sFileName, ".txt"); // adds .txt to their file name
            
            while (1) { // infinite loop for repeated input
                printf("\nEnter a command (CREATE, SHOW FILE, DELETE, COPY, RENAME and BACK): ");
                fgets(sCommand, sizeof(sCommand), stdin);
    
                sCommand[strcspn(sCommand, "\n")] = '\0';
    
                if (strcmp(sCommand, "CREATE") == 0) {
                    // checks if the file already exists
                    fptr1 = fopen(sFileName, "r");
                    if (fptr1 != NULL) {
                        fclose(fptr1);
                        printf("Error: A file with the name '%s' already exists.\n", sFileName);
                        break;
                    }

                    fptr1 = fopen(sFileName, "w");
                    if (fptr1 == NULL) {
                        printf("Error: Could not create the file.\n");
                        break;
                    }
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                    fprintf(fptr1, "File created successfully.\n");
                    fclose(fptr1);
                    printf("File created successfully.\n");
                    break;
                }
                else if (strcmp(sCommand, "SHOW FILE") == 0) {
                    showFile(sFileName, sCommand);
                    
                }
                else if (strcmp(sCommand, "DELETE") == 0) {
                    // https://www.geeksforgeeks.org/c-program-delete-file/
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                    remove(sFileName); // deletes specific file
                    
                } else if (strcmp(sCommand, "RENAME") == 0) {
                    char newName[255];
                    while (1) {
                        fptr1 = fopen(sFileName, "r");
                    // checks if the file already exists if it doesn't ask for a name
                        if (fptr1 == NULL) {
                            printf("Error: A file with the name '%s' doesn't exist. Please try again.\n", sFileName);
                            printf("Enter the name of the file you'd like to operate on(without .txt): ", sFileName);
                            fgets(sFileName, sizeof(sFileName), stdin);
                            sFileName[strcspn(sFileName, "\n")] = '\0'; // gets rid of new line character
                            strcat(sFileName, ".txt");
                            continue;
                        }
                        fclose(fptr1);
                        break;
                    }

                    while (1) {
                        printf("Enter the new name for the file (without .txt): ");
                        fgets(newName, sizeof(newName), stdin);
                        newName[strcspn(newName, "\n")] = '\0'; // remove newline
                        strcat(newName, ".txt");

                        fptr2 = fopen(newName, "r");
                        if (fptr2 != NULL) {
                            fclose(fptr2);
                            printf("Error: A file with the name '%s' already exists. Please try again.\n", newName);
                            printf("Enter the new name for the file (without .txt):  ", newName);
                            fgets(newName, sizeof(newName), stdin);
                            newName[strcspn(newName, "\n")] = '\0'; // gets rid of new line character
                            strcat(newName, ".txt");
                        }
                        break;
                    }

                   
                    if (rename(sFileName, newName) == 0) {
                        printf("File renamed");
                        logFile(sFileName, sCommand, numOfLines(sFileName));

                    } else {
                        printf("Error: Unable to rename the file.\n");
                    }
                } else if (strcmp(sCommand, "COPY") == 0) {
                    
                    char newName[255];
                    while (1) {
                        fptr1 = fopen(sFileName, "r");
                    // checks if the file already exists if it doesn't ask for a name
                        if (fptr1 == NULL) {
                            printf("Error: A file with the name '%s' doesn't exist. Please try again.\n", sFileName);
                            printf("Enter the name of the file you'd like to operate on(without .txt): ", sFileName);
                            fgets(sFileName, sizeof(sFileName), stdin);
                            sFileName[strcspn(sFileName, "\n")] = '\0'; // gets rid of new line character
                            strcat(sFileName, ".txt");
                            continue;
                        }
                        fclose(fptr1);
                        break;
                    }

                    while (1) {
                        printf("Enter the name you would like the copy to have(without .txt): ");
                        fgets(newName, sizeof(newName), stdin);
                        newName[strcspn(newName, "\n")] = '\0'; // remove newline
                        strcat(newName, ".txt");

                        tempF = fopen(newName, "r"); // checks if there is a file with the copies name
                        if (tempF != NULL) {
                            fclose(tempF);
                            printf("Error: A file with the name '%s' already exists. Please try again.\n", newName);
                            continue;
                        }
                        break;
                    }
                    
                    fptr1 = fopen(sFileName, "r");
                    fptr2 = fopen(newName, "a");

                    if ((fptr1 == NULL) || (fptr2 == NULL))
                    {
                        printf("Error: Could not open one or both of the files.\n");
                        continue;
                    }
                    //reads characters from orignal, writing them to the copy
                    int character;
                    while ((character = fgetc(fptr1)) != EOF) {
                        fputc(character, fptr2);
                    }
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                    logFile(newName, "COPIED", numOfLines(newName));
                    fclose(fptr1);
                    fclose(fptr2);
                }
                else if (strcmp(sCommand, "BACK") == 0) {
                    printf("Going back to main menu.\n");
                    break; // Exit the loop
                } 
                else {
                    printf("1Error: Invalid command. Please try again.\n");
              }
            }
        }


            
        else if(strcmp(sCommand, "LINE OPERATIONS") == 0){
             // line operations here
            printf("Enter the name of the file you'd like to operate on(without .txt): ", sFileName);
            fgets(sFileName, sizeof(sFileName), stdin);
            sFileName[strcspn(sFileName, "\n")] = '\0'; // gets rid of new line character
            strcat(sFileName, ".txt"); // adds .txt to file name
            while(1){
                printf("Enter a line command (APPEND, DELETE, INSERT, SHOW and BACK): ");
                //overwriting user input variable
                fgets(sCommand, sizeof(sCommand), stdin);
                sCommand[strcspn(sCommand, "\n")] = '\0';
                
                if (strcmp(sCommand, "APPEND") == 0) {
                    char sAppendText[255]; // used to hold the text to be added
                    printf("Enter text to append: ");
                    fgets(sAppendText, sizeof(sAppendText), stdin);
                    sAppendText[strcspn(sAppendText, "\n")] = '\0'; // Remove newline
    
                    fptr1 = fopen(sFileName, "a");
                    if (fptr1 == NULL) {
                        printf("Error: Could not open the file for appending.\n");
                        continue;
                    }
                    fprintf(fptr1, "%s\n", sAppendText);
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                    fclose(fptr1);
                    printf("Text appended successfully.\n");
                }
                else if (strcmp(sCommand, "SHOW") == 0) {
                    // https://www.quora.com/How-do-I-read-nth-line-of-a-file-in-C-using-File-handling 
                    int i = 0; // holds the line number to be shown
                    int current_line = 0; 
                    printf("Enter line to show: ");
                    scanf("%d", &i);
                    getchar(); // gets rid of the newline character
                    fptr1 = fopen(sFileName, "r");
                    if (fptr1 == NULL) {
                        printf("Error: Could not open the file for reading.\n");
                        continue;
                    }
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                    char stringArray[255]; // holds the line of the file to be shown
                    while (fgets(stringArray, sizeof(stringArray), fptr1) != NULL) { 
                        current_line++;
                        if (current_line == i) { // if the current line number is the same as the number requested
                            printf("Line: %d: %s", i, stringArray); 
                            break; 
                        } 
                    }
                    if (current_line < i) {
                        printf("Error: Line %d does not exist in the file.\n", i);
                    }
                    fclose(fptr1);
                } 
                else if (strcmp(sCommand, "DELETE")== 0)
                {
                    char *name = sFileName;
                    int line; // holds the line number to be deleted
                    char stringArray[255];

                    printf("Enter which line you want to delete: ");
                    scanf("%d", &line);
                    fptr1 = fopen(sFileName, "r");
                    tempF = fopen("temp.txt", "w"); // opens a temporary file to hold text

                    int count = 1;
                    while (fgets(stringArray, sizeof(stringArray), fptr1)){
                        if (count != line) {
                            // if the line wanting to be deleted isnt the current line, add that line to the temp file
                            fputs(stringArray, tempF);
                        }
                        count ++;
                    }

                    fclose(fptr1);
                    fclose(tempF);
                    remove(name); // deletes old file
                    rename("temp.txt", name); // renames the temp file to the old files name
                    logFile(sFileName, "DELETED LINE", numOfLines(sFileName));
                    
                }
                else if (strcmp(sCommand, "INSERT") == 0)
                {
                    char newText[255]; // holds text to be inserted
                    char stringArray[255]; // holds lines so they can be copied from one file to another
                    // checks file exists
                    fptr1 = fopen(sFileName, "r");
                    if (fptr1 == NULL) {
                        printf("Error: Could not open the file for reading.\n");
                        continue;
                    }
                    
                    int i = 0; // holds the number the insert will take place to
                    int current_line = 0;
                    printf("Enter line number you'd like to insert this text to: ");
                    scanf("%d", &i);
                    getchar(); // removes new line character
                    printf("Enter the text to insert: ");
                    fgets(newText, sizeof(newText), stdin);
                    tempF = fopen("temp.txt", "w"); // opens a temporary file to hold the old file content and the new line

                    
                    while (fgets(stringArray, sizeof(stringArray), fptr1) != NULL) {
                        // write to a new file until specific line
                        current_line++; 
                        if (current_line == i) { 
                            fprintf(tempF, "%s", newText); 
                            // add new line to new file
                        } else {
                            fprintf(tempF, "%s", stringArray);
                            // write old file to to new file
                        }
                    }
                        
                    if (i > current_line) {
                        fprintf(tempF, "%s\n", newText);
                    }
                    fclose(fptr1);
                    fclose(tempF);
                    // delete old file
                    // rename new file
                    remove(sFileName);
                    rename("temp.txt", sFileName);
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                } else if (strcmp(sCommand, "BACK") == 0){
                    printf("Going back");
                    break;
                }
                else
                { // handles if command isnt correct
                    printf("Error: Invalid command. Please try again.\n");
                }
            }
        }    
        else if (strcmp(sCommand, "GENERAL OPERATIONS") == 0){
            ; // general operations here
            while(1){
                printf("Enter a line command (LOG, LINES, SEARCH and BACK): ");
                fgets(sCommand, sizeof(sCommand), stdin);
                sCommand[strcspn(sCommand, "\n")] = '\0';
                if (strcmp(sCommand, "LOG") == 0)
                { // uses show function to show log file
                    showFile("logfile.txt", sCommand);
                }
                else if (strcmp(sCommand, "LINES")== 0){
                    printf("Enter the name of the file you'd like to know the lines of", sFileName);
                    fgets(sFileName, sizeof(sFileName), stdin);
                    sFileName[strcspn(sFileName, "\n")] = '\0'; // gets rid of new line character
                    strcat(sFileName, ".txt");
                    // calls numOfLines function to count how many lines there are in that specfic file
                    int lineCount = numOfLines(sFileName);
                    printf("Number of lines in the file: %d\n", lineCount);   
                    logFile(sFileName, sCommand, numOfLines(sFileName));
                    
                } else if (strcmp(sCommand, "SEARCH")== 0){
                    char stringItem[255], Leave[255]; // Leave used to determin if the user wants to go back wihtout 
                                                        //preventing them from searching a file named BACK
                                                        // stringItem holds what they are searching for 
                    
                    while(1){
                        printf("Do you want to go back? Type BACK else press enter: ", Leave);
                        fgets(Leave, sizeof(Leave), stdin);
                        Leave[strcspn(Leave, "\n")] = '\0'; // removes new line character
                        if(strcmp(Leave, "BACK")== 0){
                            break;
                        }
                        printf("Enter the name of the file you'd like to search: ", sFileName);
                        fgets(sFileName, sizeof(sFileName), stdin);
                        sFileName[strcspn(sFileName, "\n")] = '\0'; // gets rid of new line character
                        strcat(sFileName, ".txt");
                        printf("Enter the string you'd like to search for: ", stringItem);
                        fgets(stringItem, sizeof(stringItem), stdin);
                        stringItem[strcspn(stringItem, "\n")] = '\0'; // gets rid of new line character
                        
                        int result = searchForString(sFileName, stringItem);
                        if (result != -1) { // prevents the function from being returned twice while still adding it to log
                            logFile(sFileName, sCommand, numOfLines(sFileName));
                        }
                    }                    
                }
                else if (strcmp(sCommand, "BACK")== 0)
                {
                    break;
                }
                else
                {
                    printf("Error: Invalid command. Please try again.\n");
                }
            }
        }
        else if (strcmp(sCommand, "EXIT") == 0){
            printf("Exiting program.\n");
            break;
        }
        else{
           printf("Error: Invalid command. Please try againn.\n"); 
        }
    }
    return 0;
}

//gcc -o C-Project C-Project-Main.c
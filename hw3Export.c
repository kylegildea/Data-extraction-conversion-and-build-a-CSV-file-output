/*------------------------------------------------------------------
// University of Central Florida
// COP3502C - Fall 2016
// Program Author: Kyle Gildea

“The program is entirely my own work and I have neither developed my code together with any another person, nor copied program code from any
other person, nor permitted my code to be copied or otherwise used by any other person, nor have I
copied, modified, or otherwise used program code that you have found in any external source, including but
not limited to, online sources”.

//------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 500

typedef struct airPdata{
char *LocID; //Airport’s ‘‘Short Name’’, ie MCO
char *fieldName; //Airport Name
char *city; //Associated City
float longitude; //Longitude
float latitude; //Latitude
} airPdata;

//Function declarations
void errorPrint(char *error);
void print(airPdata *airP);
void freeLineBuffer(char *line);
void freeAirport(airPdata *airP);
void readFile(char *fileName);
void storeLocID(airPdata *airP, char *input);
void storeFieldName(airPdata *airP, char *input);
void storeCity(airPdata *airP, char *input);
void storeLatitude(airPdata *airP, char *input);
void storeLongitude(airPdata *airP, char *input);
float sexag2decimal(char *degreeString);


//Main Function
int main(int argc, char **argv)
{
  readFile(argv[1]); 
  return 0;
}

//Prints error string
void errorPrint(char *error) 
{ 
  fprintf(stderr, "%s", error);
}


/*Reads in file with name given by command line
If the file cannot be opened, prints an error
Otherwise opens file, prints column names and a divider
Then reads file line by line, using fgets
Once a line has been read, strtok() is used to
tokenize the first fields from the string, then
goes into a while loop, tokenizing the rest of the line.
The first 7 fields are always in the same location, and so a counter.
fieldRead is used to determine if the token acquired is one we're
looking for. If it is, switch statement is used to store the value
in a struct. The final field controlTower is not always in the same place but is
always the 13th field or later. so once the first 7 values have been found, 
the counter continues to go up until 13, but any text parsed is discarded.
Upon ariving at the 13th field, the counter stops and text is processed until
a "Y" or "N" is found. Once the struct is filled out, the print function
is called to print the line and the process repeats until EOF.
*/
void readFile(char *fileName)
{
  FILE *inputFile = fopen(fileName, "r");
  airPdata *airport;
  char *comma = ",";
  char *line;
  char *token;
  char *error;
  char *dashPtr;
  int fieldRead = 0;
  
  if (inputFile == NULL)
  {
    error = malloc(BUFFERSIZE * sizeof(char));
    strcpy(error, "ERROR: could not open file ");
    strcat(error, fileName);
    strcat(error, "\n\n");
    errorPrint(error);
    free(error);
  }
  else
  {
    printf("code,name,city,lat,lon\n"); 
      
   airport = malloc(sizeof(airPdata));
   if(airport == NULL)
   {
     errorPrint("Airport Struct Malloc Failed!\n");   
   }
   
   line = malloc(BUFFERSIZE * sizeof(char));
   fgets(line, BUFFERSIZE, inputFile);
   
   while(fgets(line, BUFFERSIZE, inputFile) != NULL)
   {
     token = strtok(line,comma);  
     while(token != NULL)  
     {
       if((strlen(token) == 3 && fieldRead == 0) || (strlen(token) == 4 && fieldRead == 0))
       {
         storeLocID(airport, token);
         fieldRead++;
       }            
           
       else if(fieldRead == 1)
       {
         storeFieldName(airport, token);
         fieldRead++;      
       }      
     
       else if(fieldRead == 2)
       { 
         storeCity(airport, token);
         fieldRead++;
       }               
       
       else if((fieldRead == 3 && strchr(dashPtr, '-')!=NULL) || (fieldRead == 3 && strtof(token,NULL) != 0.0F))
       {
           storeLatitude(airport, token);
           fieldRead++;          
       }
       else if(fieldRead == 4 && strchr(dashPtr, '-')!=NULL || (fieldRead == 4 && strtof(token,NULL) != 0.0F))
       {
           storeLongitude(airport, token);
           fieldRead++;
       }     
       token = strtok(NULL, comma);
     }
     fieldRead = 0;
     print(airport);
   };  
    freeAirport(airport);
    free(line);
    fclose(inputFile);
  }
}

/*The next several functions take a pointer to struct, 
and an input string, allocate memory for the member of the 
struct and copies the input string into
the struct. There is a function for each member
of the struct. The longitude and latitude functions
store the value if it's decimal, if it's DMS, they
call the conversion function and store the returned
decimal
*/

void storeLocID(airPdata *airP, char *input)
{
  airP->LocID = malloc(BUFFERSIZE * sizeof(char));
  
  if (airP->LocID == NULL) 
  {   
      errorPrint("LocID Malloc Failed!\n");   
  }
  else
    strcpy(airP->LocID, input);
}

void storeFieldName(airPdata *airP, char *input)
{
  
  airP->fieldName = malloc(BUFFERSIZE * sizeof(char));
  
  if (airP->fieldName == NULL) 
  {   
      errorPrint("fieldName Malloc Failed!\n");   
  }
  else
    strcpy(airP->fieldName, input);
}

void storeCity(airPdata *airP, char *input)
{
  airP->city = malloc(BUFFERSIZE * sizeof(char));
  
  if (airP->city == NULL) 
  {   
    errorPrint("storeState Malloc Failed!\n");
  }
  else
    strcpy(airP->city, input);
}


void storeLatitude(airPdata *airP, char *input)
{
  char *dotPtr = strchr(input, '.');
  
  if(input[strlen(input) - 1] == 'N' || input[strlen(input) - 1] == 'S')
    airP->latitude = sexag2decimal(input);
  else if(dotPtr!=NULL)
    airP->latitude = strtof(input, NULL);
  else
    airP->latitude = 0.0;
}

void storeLongitude(airPdata *airP, char *input)
{
  char *dotPtr = strchr(input, '.');
  
  if(input[strlen(input) - 1] == 'E' || input[strlen(input) - 1] == 'W')
      airP->longitude = sexag2decimal(input);
  else if(dotPtr !=NULL)
    airP->longitude = strtof(input, NULL);
  else
    airP->longitude = 0.0;
}

//Prints the struct
void print (airPdata *airP)
{ 
  printf("%s ",airP->LocID);
  printf("%s ",airP->fieldName);
  printf("%s ",airP->city);
  printf("%.4f ",airP->latitude);
  printf("%.4f\n",airP->longitude);
}

/*Frees the stuff inside the struct before
going on to free the struct itself*/
void freeAirport(airPdata *airP)
{
  free(airP->LocID);
  free(airP->fieldName);
  free(airP->city);
  free(airP);
}

/*
  Converts Degrees DD-MM-SS to Decimal by getting rid of any dashes
  and using string to float to convert each field and then doing the math
  as described in the homework instruction file. Returns 0.0 if the result is
  invalid. Checks the last character in the degreeString and if it's west or south
  makes the float negative
*/
float sexag2decimal(char *degreeString)
{
  char *dashPtr;
  char *degreeStringPtr;
  char direction;
  float degrees, minutes, seconds, sixty = 60.0, sixtySquared = 3600.0;
  int field = 0;
  
  if(degreeString == NULL)
    return 0.0;
  else
  {
    direction = degreeString[strlen(degreeString) - 1];
    degreeString[strlen(degreeString) - 1] = '\0';
    dashPtr = strchr(degreeString, '-');
    while(dashPtr != NULL)
    {
      *dashPtr = ' ';
      dashPtr = strchr(dashPtr + 1, '-');
    }
    
    degrees = strtof(degreeString, &degreeStringPtr);
    minutes = strtof(degreeStringPtr, &degreeStringPtr);
    seconds = strtof(degreeStringPtr, NULL);
    
    //printf("degrees: %f, minutes: %f, seconds: %f\n", degrees,minutes,seconds);
    
    if(degrees < 0.0 || degrees > 180.0 || minutes < 0.0 || minutes > 59.0 || seconds > 59.9999 || seconds < 0.0)
      return 0.0;
    else
    { 
      degrees+= ((minutes/sixty) + (seconds/sixtySquared));
      if(direction == 'S' || direction == 'W')
      {
        degrees = -(degrees);
      }
    }
  }
  return degrees;
}